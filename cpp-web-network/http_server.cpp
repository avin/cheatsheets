// ============================================
// 📌 Simple HTTP Server
// ============================================

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <regex>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <expected>
#include <chrono>

// --- Парсинг HTTP запроса ---
struct HttpRequest {
    std::string method;           // GET, POST, PUT, DELETE
    std::string path;             // /api/users
    std::string version;          // HTTP/1.1
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> query_params;
    std::string body;
    
    // Парсинг query параметров из URL
    static void parse_query_string(const std::string& query, 
                                   std::unordered_map<std::string, std::string>& params) {
        std::istringstream stream(query);
        std::string pair;
        
        while (std::getline(stream, pair, '&')) {
            auto eq_pos = pair.find('=');
            if (eq_pos != std::string::npos) {
                auto key = pair.substr(0, eq_pos);
                auto value = pair.substr(eq_pos + 1);
                params[url_decode(key)] = url_decode(value);
            }
        }
    }
    
    // URL декодирование (%20 -> пробел)
    static std::string url_decode(const std::string& str) {
        std::string result;
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == '%' && i + 2 < str.length()) {
                int value;
                std::istringstream hex(str.substr(i + 1, 2));
                if (hex >> std::hex >> value) {
                    result += static_cast<char>(value);
                    i += 2;
                }
            } else if (str[i] == '+') {
                result += ' ';
            } else {
                result += str[i];
            }
        }
        return result;
    }
    
    // Парсинг HTTP запроса из строки
    static std::expected<HttpRequest, std::string> parse(const std::string& data) {
        HttpRequest req;
        std::istringstream stream(data);
        std::string line;
        
        // Парсинг Request Line: GET /path HTTP/1.1
        if (!std::getline(stream, line)) {
            return std::unexpected("Empty request");
        }
        
        std::istringstream request_line(line);
        std::string path_with_query;
        request_line >> req.method >> path_with_query >> req.version;
        
        // Разделение пути и query параметров
        auto query_pos = path_with_query.find('?');
        if (query_pos != std::string::npos) {
            req.path = path_with_query.substr(0, query_pos);
            auto query = path_with_query.substr(query_pos + 1);
            parse_query_string(query, req.query_params);
        } else {
            req.path = path_with_query;
        }
        
        // Парсинг заголовков
        while (std::getline(stream, line) && line != "\r" && !line.empty()) {
            auto colon = line.find(':');
            if (colon != std::string::npos) {
                auto key = line.substr(0, colon);
                auto value = line.substr(colon + 2); // Пропуск ": "
                
                // Удаление \r в конце
                if (!value.empty() && value.back() == '\r') {
                    value.pop_back();
                }
                
                req.headers[key] = value;
            }
        }
        
        // Чтение тела запроса
        std::string body_line;
        while (std::getline(stream, body_line)) {
            req.body += body_line + "\n";
        }
        
        return req;
    }
};

// --- MIME типы для статических файлов ---
class MimeTypes {
public:
    static std::string get_mime_type(const std::string& path) {
        static const std::unordered_map<std::string, std::string> mime_map = {
            {".html", "text/html"},
            {".htm", "text/html"},
            {".css", "text/css"},
            {".js", "application/javascript"},
            {".json", "application/json"},
            {".png", "image/png"},
            {".jpg", "image/jpeg"},
            {".jpeg", "image/jpeg"},
            {".gif", "image/gif"},
            {".svg", "image/svg+xml"},
            {".ico", "image/x-icon"},
            {".txt", "text/plain"},
            {".pdf", "application/pdf"},
            {".zip", "application/zip"},
            {".xml", "application/xml"},
        };
        
        auto ext = std::filesystem::path(path).extension().string();
        auto it = mime_map.find(ext);
        return it != mime_map.end() ? it->second : "application/octet-stream";
    }
};

// ============================================
// 📌 Routing System
// ============================================

// --- Route Handler ---
using RouteHandler = std::function<void(const HttpRequest&, class HttpResponse&)>;

// --- Маршрут с параметрами ---
struct Route {
    std::string method;        // GET, POST, PUT, DELETE
    std::regex path_regex;     // Регулярное выражение для пути
    std::vector<std::string> param_names; // Имена параметров
    RouteHandler handler;      // Функция-обработчик
    
    // Проверка соответствия запроса маршруту
    bool matches(const std::string& method, const std::string& path, 
                std::unordered_map<std::string, std::string>& params) const {
        if (this->method != method) return false;
        
        std::smatch match;
        if (!std::regex_match(path, match, path_regex)) return false;
        
        // Извлечение параметров из пути
        for (size_t i = 0; i < param_names.size(); ++i) {
            params[param_names[i]] = match[i + 1].str();
        }
        
        return true;
    }
};

// --- Роутер ---
class Router {
private:
    std::vector<Route> routes;
    
public:
    // Добавление статического маршрута
    void add_route(const std::string& method, const std::string& path, RouteHandler handler) {
        Route route;
        route.method = method;
        route.path_regex = std::regex("^" + path + "$");
        route.handler = handler;
        routes.push_back(route);
    }
    
    // Добавление динамического маршрута с параметрами
    // Пример: "/users/:id/posts/:post_id"
    void add_route_with_params(const std::string& method, const std::string& pattern, 
                              RouteHandler handler) {
        Route route;
        route.method = method;
        
        // Преобразование шаблона в регулярное выражение
        // :id -> ([^/]+) и сохранение имени параметра
        std::string regex_pattern = "^";
        std::regex param_regex(":([a-zA-Z_][a-zA-Z0-9_]*)");
        
        auto words_begin = std::sregex_iterator(pattern.begin(), pattern.end(), param_regex);
        auto words_end = std::sregex_iterator();
        
        size_t last_pos = 0;
        for (auto it = words_begin; it != words_end; ++it) {
            std::smatch match = *it;
            
            // Добавляем текст до параметра
            regex_pattern += pattern.substr(last_pos, match.position() - last_pos);
            
            // Добавляем регулярку для параметра
            regex_pattern += "([^/]+)";
            
            // Сохраняем имя параметра
            route.param_names.push_back(match[1].str());
            
            last_pos = match.position() + match.length();
        }
        
        regex_pattern += pattern.substr(last_pos) + "$";
        route.path_regex = std::regex(regex_pattern);
        route.handler = handler;
        routes.push_back(route);
    }
    
    // Wildcard маршруты для статических файлов
    void add_static_route(const std::string& prefix, const std::string& root_dir) {
        std::string pattern = "^" + prefix + "/(.*)$";
        Route route;
        route.method = "GET";
        route.path_regex = std::regex(pattern);
        route.param_names = {"filepath"};
        route.handler = [root_dir](const HttpRequest& req, HttpResponse& res) {
            // Обработчик будет определен позже
        };
        routes.push_back(route);
    }
    
    // Поиск подходящего маршрута
    std::optional<std::pair<RouteHandler, std::unordered_map<std::string, std::string>>>
    find_route(const std::string& method, const std::string& path) const {
        for (const auto& route : routes) {
            std::unordered_map<std::string, std::string> params;
            if (route.matches(method, path, params)) {
                return std::make_pair(route.handler, params);
            }
        }
        return std::nullopt;
    }
    
    // Удобные методы регистрации
    void get(const std::string& path, RouteHandler handler) {
        if (path.find(':') != std::string::npos) {
            add_route_with_params("GET", path, handler);
        } else {
            add_route("GET", path, handler);
        }
    }
    
    void post(const std::string& path, RouteHandler handler) {
        if (path.find(':') != std::string::npos) {
            add_route_with_params("POST", path, handler);
        } else {
            add_route("POST", path, handler);
        }
    }
    
    void put(const std::string& path, RouteHandler handler) {
        if (path.find(':') != std::string::npos) {
            add_route_with_params("PUT", path, handler);
        } else {
            add_route("PUT", path, handler);
        }
    }
    
    void del(const std::string& path, RouteHandler handler) {
        if (path.find(':') != std::string::npos) {
            add_route_with_params("DELETE", path, handler);
        } else {
            add_route("DELETE", path, handler);
        }
    }
};

// ============================================
// 📌 Request/Response Objects
// ============================================

// --- Request Object (расширенный) ---
class HttpRequestEx {
private:
    HttpRequest raw_request;
    std::unordered_map<std::string, std::string> path_params; // :id из /users/:id
    std::unordered_map<std::string, std::string> cookies;
    std::string client_ip;
    
public:
    HttpRequestEx(const HttpRequest& req) : raw_request(req) {
        parse_cookies();
    }
    
    const std::string& method() const { return raw_request.method; }
    const std::string& path() const { return raw_request.path; }
    const std::string& body() const { return raw_request.body; }
    
    // Получение заголовка (case-insensitive)
    std::optional<std::string> header(const std::string& name) const {
        for (const auto& [key, value] : raw_request.headers) {
            if (strcasecmp(key.c_str(), name.c_str()) == 0) {
                return value;
            }
        }
        return std::nullopt;
    }
    
    // Query параметр
    std::optional<std::string> query(const std::string& key) const {
        auto it = raw_request.query_params.find(key);
        return it != raw_request.query_params.end() ? 
               std::optional(it->second) : std::nullopt;
    }
    
    // Path параметр (/users/:id)
    std::optional<std::string> param(const std::string& key) const {
        auto it = path_params.find(key);
        return it != path_params.end() ? std::optional(it->second) : std::nullopt;
    }
    
    void set_path_params(const std::unordered_map<std::string, std::string>& params) {
        path_params = params;
    }
    
    // Cookie
    std::optional<std::string> cookie(const std::string& name) const {
        auto it = cookies.find(name);
        return it != cookies.end() ? std::optional(it->second) : std::nullopt;
    }
    
    const std::string& get_client_ip() const { return client_ip; }
    void set_client_ip(const std::string& ip) { client_ip = ip; }
    
private:
    void parse_cookies() {
        auto cookie_header = header("Cookie");
        if (!cookie_header) return;
        
        std::istringstream stream(*cookie_header);
        std::string pair;
        
        while (std::getline(stream, pair, ';')) {
            auto eq = pair.find('=');
            if (eq != std::string::npos) {
                auto key = trim(pair.substr(0, eq));
                auto value = trim(pair.substr(eq + 1));
                cookies[key] = value;
            }
        }
    }
    
    static std::string trim(const std::string& str) {
        auto start = str.find_first_not_of(" \t");
        auto end = str.find_last_not_of(" \t");
        return start == std::string::npos ? "" : str.substr(start, end - start + 1);
    }
};

// --- Response Object ---
class HttpResponse {
private:
    int status_code = 200;
    std::unordered_map<std::string, std::string> headers;
    std::string body_content;
    bool sent = false;
    
public:
    // Установка статус кода
    HttpResponse& status(int code) {
        status_code = code;
        return *this;
    }
    
    // Установка заголовка
    HttpResponse& set_header(const std::string& key, const std::string& value) {
        headers[key] = value;
        return *this;
    }
    
    // Отправка текста
    HttpResponse& send(const std::string& text) {
        body_content = text;
        if (headers.find("Content-Type") == headers.end()) {
            headers["Content-Type"] = "text/plain";
        }
        sent = true;
        return *this;
    }
    
    // Отправка JSON
    HttpResponse& json(const std::string& json_str) {
        body_content = json_str;
        headers["Content-Type"] = "application/json";
        sent = true;
        return *this;
    }
    
    // Отправка HTML
    HttpResponse& html(const std::string& html_content) {
        body_content = html_content;
        headers["Content-Type"] = "text/html";
        sent = true;
        return *this;
    }
    
    // Отправка файла
    HttpResponse& send_file(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::binary);
        if (!file) {
            return status(404).send("File not found");
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        body_content = buffer.str();
        
        headers["Content-Type"] = MimeTypes::get_mime_type(filepath);
        sent = true;
        return *this;
    }
    
    // Редирект
    HttpResponse& redirect(const std::string& url, int code = 302) {
        status_code = code;
        headers["Location"] = url;
        sent = true;
        return *this;
    }
    
    // Установка cookie
    HttpResponse& set_cookie(const std::string& name, const std::string& value,
                            int max_age = -1, const std::string& path = "/") {
        std::string cookie = name + "=" + value + "; Path=" + path;
        if (max_age > 0) {
            cookie += "; Max-Age=" + std::to_string(max_age);
        }
        headers["Set-Cookie"] = cookie;
        return *this;
    }
    
    // Генерация HTTP ответа
    std::string build() const {
        std::ostringstream response;
        
        // Status line
        response << "HTTP/1.1 " << status_code << " " << get_status_text() << "\r\n";
        
        // Headers
        for (const auto& [key, value] : headers) {
            response << key << ": " << value << "\r\n";
        }
        
        // Content-Length
        response << "Content-Length: " << body_content.size() << "\r\n";
        response << "\r\n";
        
        // Body
        response << body_content;
        
        return response.str();
    }
    
private:
    std::string get_status_text() const {
        static const std::unordered_map<int, std::string> status_texts = {
            {200, "OK"},
            {201, "Created"},
            {204, "No Content"},
            {301, "Moved Permanently"},
            {302, "Found"},
            {304, "Not Modified"},
            {400, "Bad Request"},
            {401, "Unauthorized"},
            {403, "Forbidden"},
            {404, "Not Found"},
            {500, "Internal Server Error"},
        };
        
        auto it = status_texts.find(status_code);
        return it != status_texts.end() ? it->second : "Unknown";
    }
};

// ============================================
// 📌 Middleware System
// ============================================

// --- Middleware тип ---
using Middleware = std::function<bool(HttpRequestEx&, HttpResponse&)>;

// --- Middleware Manager ---
class MiddlewareChain {
private:
    std::vector<Middleware> middlewares;
    
public:
    // Добавление middleware
    void use(Middleware mw) {
        middlewares.push_back(mw);
    }
    
    // Выполнение цепочки middleware
    // Возвращает true если все middleware пропустили запрос дальше
    bool execute(HttpRequestEx& req, HttpResponse& res) {
        for (auto& mw : middlewares) {
            if (!mw(req, res)) {
                return false; // Middleware прервал обработку
            }
        }
        return true;
    }
};

// --- Логирование запросов ---
Middleware logging_middleware() {
    return [](HttpRequestEx& req, HttpResponse& res) -> bool {
        auto start = std::chrono::steady_clock::now();
        
        std::cout << "[" << current_time() << "] "
                  << req.method() << " " << req.path() 
                  << " from " << req.get_client_ip() << std::endl;
        
        // Продолжаем обработку
        return true;
    };
}

// --- CORS middleware ---
Middleware cors_middleware(const std::string& allowed_origin = "*") {
    return [allowed_origin](HttpRequestEx& req, HttpResponse& res) -> bool {
        res.set_header("Access-Control-Allow-Origin", allowed_origin)
           .set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS")
           .set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        
        // Обработка preflight запросов
        if (req.method() == "OPTIONS") {
            res.status(204).send("");
            return false; // Останавливаем дальнейшую обработку
        }
        
        return true;
    };
}

// --- Аутентификация через Bearer token ---
Middleware auth_middleware(const std::string& secret_token) {
    return [secret_token](HttpRequestEx& req, HttpResponse& res) -> bool {
        auto auth_header = req.header("Authorization");
        
        if (!auth_header) {
            res.status(401).json("{\"error\": \"Missing authorization header\"}");
            return false;
        }
        
        if (!auth_header->starts_with("Bearer ")) {
            res.status(401).json("{\"error\": \"Invalid authorization format\"}");
            return false;
        }
        
        auto token = auth_header->substr(7); // Убираем "Bearer "
        if (token != secret_token) {
            res.status(401).json("{\"error\": \"Invalid token\"}");
            return false;
        }
        
        return true; // Токен валиден, продолжаем
    };
}

// --- Rate Limiting ---
class RateLimiter {
private:
    std::unordered_map<std::string, std::vector<std::chrono::steady_clock::time_point>> requests;
    int max_requests;
    std::chrono::seconds window;
    
public:
    RateLimiter(int max_req, std::chrono::seconds win) 
        : max_requests(max_req), window(win) {}
    
    bool allow(const std::string& client_ip) {
        auto now = std::chrono::steady_clock::now();
        auto& timestamps = requests[client_ip];
        
        // Удаление старых запросов
        timestamps.erase(
            std::remove_if(timestamps.begin(), timestamps.end(),
                [this, now](const auto& ts) {
                    return now - ts > window;
                }),
            timestamps.end()
        );
        
        if (timestamps.size() >= max_requests) {
            return false; // Превышен лимит
        }
        
        timestamps.push_back(now);
        return true;
    }
};

Middleware rate_limit_middleware(int max_requests, std::chrono::seconds window) {
    auto limiter = std::make_shared<RateLimiter>(max_requests, window);
    
    return [limiter](HttpRequestEx& req, HttpResponse& res) -> bool {
        if (!limiter->allow(req.get_client_ip())) {
            res.status(429)
               .set_header("Retry-After", "60")
               .json("{\"error\": \"Too many requests\"}");
            return false;
        }
        return true;
    };
}

// --- Compression middleware (gzip) ---
Middleware compression_middleware() {
    return [](HttpRequestEx& req, HttpResponse& res) -> bool {
        auto accept_encoding = req.header("Accept-Encoding");
        if (accept_encoding && accept_encoding->find("gzip") != std::string::npos) {
            res.set_header("Content-Encoding", "gzip");
            // В реальности здесь нужно сжать body с помощью zlib
        }
        return true;
    };
}

// --- Request ID tracking ---
Middleware request_id_middleware() {
    return [](HttpRequestEx& req, HttpResponse& res) -> bool {
        // Генерация уникального ID для запроса
        auto request_id = generate_uuid();
        res.set_header("X-Request-ID", request_id);
        return true;
    };
}

// Вспомогательная функция для текущего времени
std::string current_time() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&time_t));
    return buffer;
}

// Вспомогательная функция генерации UUID (упрощенная)
std::string generate_uuid() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    
    const char* hex = "0123456789abcdef";
    std::string uuid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
    
    for (auto& c : uuid) {
        if (c == 'x') {
            c = hex[dis(gen)];
        } else if (c == 'y') {
            c = hex[(dis(gen) & 0x3) | 0x8];
        }
    }
    
    return uuid;
}

// ============================================
// 📌 Modern Web Framework Structure
// ============================================

// --- Application Class ---
class HttpServer {
private:
    Router router;
    MiddlewareChain middleware_chain;
    int server_socket = -1;
    bool running = false;
    
public:
    // Регистрация middleware
    void use(Middleware mw) {
        middleware_chain.use(mw);
    }
    
    // RESTful методы
    void get(const std::string& path, RouteHandler handler) {
        router.get(path, handler);
    }
    
    void post(const std::string& path, RouteHandler handler) {
        router.post(path, handler);
    }
    
    void put(const std::string& path, RouteHandler handler) {
        router.put(path, handler);
    }
    
    void del(const std::string& path, RouteHandler handler) {
        router.del(path, handler);
    }
    
    // Обслуживание статических файлов
    void static_files(const std::string& url_prefix, const std::string& directory) {
        get(url_prefix + "/*", [directory](const HttpRequestEx& req, HttpResponse& res) {
            auto filepath_param = req.param("0"); // Wildcard захват
            if (!filepath_param) {
                res.status(400).send("Invalid path");
                return;
            }
            
            std::filesystem::path full_path = std::filesystem::path(directory) / *filepath_param;
            
            // Защита от path traversal атак
            auto canonical = std::filesystem::weakly_canonical(full_path);
            auto root = std::filesystem::weakly_canonical(directory);
            
            if (canonical.string().find(root.string()) != 0) {
                res.status(403).send("Access denied");
                return;
            }
            
            if (!std::filesystem::exists(canonical)) {
                res.status(404).send("File not found");
                return;
            }
            
            res.send_file(canonical.string());
        });
    }
    
    // Обработка запроса
    void handle_request(int client_socket) {
        // Чтение данных от клиента
        char buffer[8192];
        ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_read <= 0) {
            close(client_socket);
            return;
        }
        
        buffer[bytes_read] = '\0';
        std::string request_data(buffer);
        
        // Парсинг запроса
        auto parse_result = HttpRequest::parse(request_data);
        if (!parse_result) {
            std::string error_response = "HTTP/1.1 400 Bad Request\r\n\r\nBad Request";
            send(client_socket, error_response.c_str(), error_response.size(), 0);
            close(client_socket);
            return;
        }
        
        HttpRequestEx req(*parse_result);
        HttpResponse res;
        
        // TODO: Получение IP клиента из sockaddr
        req.set_client_ip("127.0.0.1");
        
        // Выполнение middleware
        if (!middleware_chain.execute(req, res)) {
            // Middleware прервал обработку, отправляем ответ
            std::string response = res.build();
            send(client_socket, response.c_str(), response.size(), 0);
            close(client_socket);
            return;
        }
        
        // Поиск маршрута
        auto route_result = router.find_route(req.method(), req.path());
        
        if (!route_result) {
            res.status(404).send("Not Found");
        } else {
            auto [handler, path_params] = *route_result;
            req.set_path_params(path_params);
            
            try {
                handler(req, res);
            } catch (const std::exception& e) {
                res.status(500).send("Internal Server Error: " + std::string(e.what()));
            }
        }
        
        // Отправка ответа
        std::string response = res.build();
        send(client_socket, response.c_str(), response.size(), 0);
        close(client_socket);
    }
    
    // Запуск сервера
    void listen(int port) {
        // Создание сокета
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0) {
            throw std::runtime_error("Failed to create socket");
        }
        
        // Разрешение повторного использования адреса
        int opt = 1;
        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        // Привязка к порту
        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);
        
        if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            throw std::runtime_error("Failed to bind to port " + std::to_string(port));
        }
        
        // Начало прослушивания
        if (::listen(server_socket, 128) < 0) {
            throw std::runtime_error("Failed to listen");
        }
        
        std::cout << "Server listening on port " << port << std::endl;
        running = true;
        
        // Основной цикл обработки подключений
        while (running) {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            
            int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
            if (client_socket < 0) {
                if (!running) break;
                continue;
            }
            
            // В реальном приложении здесь нужен thread pool
            std::thread([this, client_socket]() {
                handle_request(client_socket);
            }).detach();
        }
    }
    
    // Остановка сервера
    void stop() {
        running = false;
        if (server_socket >= 0) {
            close(server_socket);
        }
    }
};

// --- Пример использования ---
void example_http_server() {
    HttpServer app;
    
    // Middleware
    app.use(logging_middleware());
    app.use(cors_middleware("*"));
    app.use(rate_limit_middleware(100, std::chrono::seconds(60)));
    
    // Маршруты
    app.get("/", [](const HttpRequestEx& req, HttpResponse& res) {
        res.html("<h1>Welcome to C++ Web Server</h1>");
    });
    
    app.get("/api/users/:id", [](const HttpRequestEx& req, HttpResponse& res) {
        auto user_id = req.param("id");
        res.json("{\"id\": \"" + *user_id + "\", \"name\": \"John Doe\"}");
    });
    
    app.post("/api/users", [](const HttpRequestEx& req, HttpResponse& res) {
        // Парсинг JSON из body
        auto body = req.body();
        res.status(201).json("{\"message\": \"User created\"}");
    });
    
    // Статические файлы
    app.static_files("/static", "./public");
    
    // Запуск
    app.listen(8080);
}

// ============================================
// 📌 JSON API Server
// ============================================

// --- JSON Request/Response Helpers ---
class JsonApi {
public:
    // Парсинг JSON из строки (упрощенная версия)
    static std::unordered_map<std::string, std::string> parse_json(const std::string& json) {
        // В реальности использовать nlohmann/json или RapidJSON
        std::unordered_map<std::string, std::string> result;
        // Простой парсинг пар "key": "value"
        std::regex pair_regex(R"("([^"]+)"\s*:\s*"([^"]*)")");
        
        auto begin = std::sregex_iterator(json.begin(), json.end(), pair_regex);
        auto end = std::sregex_iterator();
        
        for (auto it = begin; it != end; ++it) {
            result[(*it)[1].str()] = (*it)[2].str();
        }
        
        return result;
    }
    
    // Создание JSON ответа
    static std::string to_json(const std::unordered_map<std::string, std::string>& data) {
        std::ostringstream json;
        json << "{";
        
        bool first = true;
        for (const auto& [key, value] : data) {
            if (!first) json << ", ";
            json << "\"" << key << "\": \"" << value << "\"";
            first = false;
        }
        
        json << "}";
        return json.str();
    }
    
    // Создание массива JSON
    static std::string array_to_json(const std::vector<std::unordered_map<std::string, std::string>>& items) {
        std::ostringstream json;
        json << "[";
        
        for (size_t i = 0; i < items.size(); ++i) {
            json << to_json(items[i]);
            if (i < items.size() - 1) json << ", ";
        }
        
        json << "]";
        return json.str();
    }
};

// --- Типизированные endpoints ---
struct User {
    int id;
    std::string name;
    std::string email;
    
    std::string to_json() const {
        return "{\"id\": " + std::to_string(id) + 
               ", \"name\": \"" + name + 
               "\", \"email\": \"" + email + "\"}";
    }
    
    static std::optional<User> from_json(const std::string& json) {
        auto data = JsonApi::parse_json(json);
        
        User user;
        try {
            user.id = std::stoi(data["id"]);
            user.name = data["name"];
            user.email = data["email"];
            return user;
        } catch (...) {
            return std::nullopt;
        }
    }
};

// --- REST API Server пример ---
void create_rest_api_server() {
    HttpServer app;
    
    // Хранилище пользователей (in-memory)
    static std::vector<User> users = {
        {1, "Alice", "alice@example.com"},
        {2, "Bob", "bob@example.com"}
    };
    static int next_id = 3;
    
    // GET /api/users - получить всех пользователей
    app.get("/api/users", [](const HttpRequestEx& req, HttpResponse& res) {
        std::ostringstream json;
        json << "[";
        for (size_t i = 0; i < users.size(); ++i) {
            json << users[i].to_json();
            if (i < users.size() - 1) json << ", ";
        }
        json << "]";
        
        res.json(json.str());
    });
    
    // GET /api/users/:id - получить пользователя
    app.get("/api/users/:id", [](const HttpRequestEx& req, HttpResponse& res) {
        auto id_str = req.param("id");
        if (!id_str) {
            res.status(400).json("{\"error\": \"Missing id\"}");
            return;
        }
        
        int id = std::stoi(*id_str);
        auto it = std::find_if(users.begin(), users.end(),
            [id](const User& u) { return u.id == id; });
        
        if (it == users.end()) {
            res.status(404).json("{\"error\": \"User not found\"}");
        } else {
            res.json(it->to_json());
        }
    });
    
    // POST /api/users - создать пользователя
    app.post("/api/users", [](const HttpRequestEx& req, HttpResponse& res) {
        auto user_opt = User::from_json(req.body());
        
        if (!user_opt) {
            res.status(400).json("{\"error\": \"Invalid JSON\"}");
            return;
        }
        
        User new_user = *user_opt;
        new_user.id = next_id++;
        users.push_back(new_user);
        
        res.status(201)
           .set_header("Location", "/api/users/" + std::to_string(new_user.id))
           .json(new_user.to_json());
    });
    
    // PUT /api/users/:id - обновить пользователя
    app.put("/api/users/:id", [](const HttpRequestEx& req, HttpResponse& res) {
        auto id_str = req.param("id");
        int id = std::stoi(*id_str);
        
        auto it = std::find_if(users.begin(), users.end(),
            [id](const User& u) { return u.id == id; });
        
        if (it == users.end()) {
            res.status(404).json("{\"error\": \"User not found\"}");
            return;
        }
        
        auto user_opt = User::from_json(req.body());
        if (!user_opt) {
            res.status(400).json("{\"error\": \"Invalid JSON\"}");
            return;
        }
        
        user_opt->id = id;
        *it = *user_opt;
        
        res.json(it->to_json());
    });
    
    // DELETE /api/users/:id - удалить пользователя
    app.del("/api/users/:id", [](const HttpRequestEx& req, HttpResponse& res) {
        auto id_str = req.param("id");
        int id = std::stoi(*id_str);
        
        auto it = std::remove_if(users.begin(), users.end(),
            [id](const User& u) { return u.id == id; });
        
        if (it == users.end()) {
            res.status(404).json("{\"error\": \"User not found\"}");
        } else {
            users.erase(it, users.end());
            res.status(204).send("");
        }
    });
    
    app.listen(8080);
}

// --- API Versioning ---
// Версионирование через URL: /api/v1/users, /api/v2/users
void setup_versioned_api(HttpServer& app) {
    // API v1
    app.get("/api/v1/users", [](const HttpRequestEx& req, HttpResponse& res) {
        res.json("[{\"id\": 1, \"name\": \"Alice\"}]");
    });
    
    // API v2 - добавлено новое поле
    app.get("/api/v2/users", [](const HttpRequestEx& req, HttpResponse& res) {
        res.json("[{\"id\": 1, \"name\": \"Alice\", \"created_at\": \"2024-01-01\"}]");
    });
}

// ============================================
// 📌 WebSocket Support
// ============================================

// --- WebSocket Handshake ---
class WebSocketHandshake {
public:
    static std::string compute_accept_key(const std::string& client_key) {
        // Sec-WebSocket-Accept = base64(SHA1(Sec-WebSocket-Key + GUID))
        const std::string GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        std::string combined = client_key + GUID;
        
        // Вычисление SHA-1 (упрощенно, в реальности использовать OpenSSL)
        // unsigned char hash[20];
        // SHA1((unsigned char*)combined.c_str(), combined.size(), hash);
        // return base64_encode(hash, 20);
        
        return "dummy_accept_key"; // Заглушка
    }
    
    static std::optional<std::string> parse_websocket_key(const HttpRequest& req) {
        auto it = req.headers.find("Sec-WebSocket-Key");
        return it != req.headers.end() ? std::optional(it->second) : std::nullopt;
    }
    
    static std::string create_handshake_response(const std::string& accept_key) {
        std::ostringstream response;
        response << "HTTP/1.1 101 Switching Protocols\r\n"
                 << "Upgrade: websocket\r\n"
                 << "Connection: Upgrade\r\n"
                 << "Sec-WebSocket-Accept: " << accept_key << "\r\n"
                 << "\r\n";
        return response.str();
    }
};

// --- WebSocket Frame ---
struct WebSocketFrame {
    bool fin;              // Последний фрагмент
    uint8_t opcode;        // 0x1=text, 0x2=binary, 0x8=close, 0x9=ping, 0xA=pong
    bool masked;           // Маскировка (клиент всегда маскирует)
    uint64_t payload_len;  // Длина данных
    uint8_t masking_key[4]; // Ключ маскировки
    std::vector<uint8_t> payload; // Данные
    
    enum Opcode {
        CONTINUATION = 0x0,
        TEXT = 0x1,
        BINARY = 0x2,
        CLOSE = 0x8,
        PING = 0x9,
        PONG = 0xA
    };
    
    // Демаскировка данных
    void unmask() {
        if (masked) {
            for (size_t i = 0; i < payload.size(); ++i) {
                payload[i] ^= masking_key[i % 4];
            }
            masked = false;
        }
    }
    
    // Парсинг фрейма из буфера
    static std::optional<WebSocketFrame> parse(const uint8_t* data, size_t len) {
        if (len < 2) return std::nullopt;
        
        WebSocketFrame frame;
        frame.fin = (data[0] & 0x80) != 0;
        frame.opcode = data[0] & 0x0F;
        frame.masked = (data[1] & 0x80) != 0;
        
        uint64_t payload_len = data[1] & 0x7F;
        size_t offset = 2;
        
        // Расширенная длина
        if (payload_len == 126) {
            if (len < offset + 2) return std::nullopt;
            payload_len = (data[offset] << 8) | data[offset + 1];
            offset += 2;
        } else if (payload_len == 127) {
            if (len < offset + 8) return std::nullopt;
            payload_len = 0;
            for (int i = 0; i < 8; ++i) {
                payload_len = (payload_len << 8) | data[offset + i];
            }
            offset += 8;
        }
        
        frame.payload_len = payload_len;
        
        // Ключ маскировки
        if (frame.masked) {
            if (len < offset + 4) return std::nullopt;
            memcpy(frame.masking_key, data + offset, 4);
            offset += 4;
        }
        
        // Payload
        if (len < offset + payload_len) return std::nullopt;
        frame.payload.assign(data + offset, data + offset + payload_len);
        frame.unmask();
        
        return frame;
    }
    
    // Создание фрейма для отправки
    static std::vector<uint8_t> create(Opcode opcode, const std::string& data, bool fin = true) {
        std::vector<uint8_t> frame;
        
        // Первый байт: FIN + opcode
        frame.push_back((fin ? 0x80 : 0x00) | opcode);
        
        // Второй байт: MASK + length
        uint64_t len = data.size();
        if (len <= 125) {
            frame.push_back(len);
        } else if (len <= 65535) {
            frame.push_back(126);
            frame.push_back((len >> 8) & 0xFF);
            frame.push_back(len & 0xFF);
        } else {
            frame.push_back(127);
            for (int i = 7; i >= 0; --i) {
                frame.push_back((len >> (i * 8)) & 0xFF);
            }
        }
        
        // Payload (сервер не маскирует)
        frame.insert(frame.end(), data.begin(), data.end());
        
        return frame;
    }
};

// --- WebSocket Connection ---
class WebSocketConnection {
private:
    int socket_fd;
    bool connected = true;
    
public:
    WebSocketConnection(int fd) : socket_fd(fd) {}
    
    // Отправка текстового сообщения
    void send_text(const std::string& message) {
        auto frame = WebSocketFrame::create(WebSocketFrame::TEXT, message);
        send(socket_fd, frame.data(), frame.size(), 0);
    }
    
    // Отправка бинарных данных
    void send_binary(const std::vector<uint8_t>& data) {
        std::string str_data(data.begin(), data.end());
        auto frame = WebSocketFrame::create(WebSocketFrame::BINARY, str_data);
        send(socket_fd, frame.data(), frame.size(), 0);
    }
    
    // Ping
    void ping() {
        auto frame = WebSocketFrame::create(WebSocketFrame::PING, "");
        send(socket_fd, frame.data(), frame.size(), 0);
    }
    
    // Pong (ответ на ping)
    void pong() {
        auto frame = WebSocketFrame::create(WebSocketFrame::PONG, "");
        send(socket_fd, frame.data(), frame.size(), 0);
    }
    
    // Закрытие соединения
    void close(uint16_t code = 1000, const std::string& reason = "") {
        std::string payload;
        payload.push_back((code >> 8) & 0xFF);
        payload.push_back(code & 0xFF);
        payload += reason;
        
        auto frame = WebSocketFrame::create(WebSocketFrame::CLOSE, payload);
        send(socket_fd, frame.data(), frame.size(), 0);
        connected = false;
    }
    
    bool is_connected() const { return connected; }
};

// --- WebSocket Server Example ---
void websocket_chat_server() {
    // Список активных подключений
    static std::vector<std::shared_ptr<WebSocketConnection>> connections;
    static std::mutex connections_mutex;
    
    HttpServer app;
    
    // WebSocket endpoint
    app.get("/ws/chat", [](const HttpRequestEx& req, HttpResponse& res) {
        // Проверка WebSocket handshake
        auto ws_key = req.header("Sec-WebSocket-Key");
        auto upgrade = req.header("Upgrade");
        
        if (!ws_key || !upgrade || *upgrade != "websocket") {
            res.status(400).send("Bad Request");
            return;
        }
        
        // Вычисление Sec-WebSocket-Accept
        auto accept_key = WebSocketHandshake::compute_accept_key(*ws_key);
        auto handshake_response = WebSocketHandshake::create_handshake_response(accept_key);
        
        // Отправка handshake (здесь нужно получить socket_fd)
        // После handshake переключиться в режим WebSocket
        
        // Создание WebSocket соединения
        // auto ws_conn = std::make_shared<WebSocketConnection>(socket_fd);
        
        // Добавление в список
        // {
        //     std::lock_guard lock(connections_mutex);
        //     connections.push_back(ws_conn);
        // }
        
        // Обработка сообщений
        // while (ws_conn->is_connected()) {
        //     auto frame = receive_websocket_frame(socket_fd);
        //     
        //     if (frame.opcode == WebSocketFrame::TEXT) {
        //         // Broadcast сообщения всем
        //         broadcast(frame.payload);
        //     } else if (frame.opcode == WebSocketFrame::PING) {
        //         ws_conn->pong();
        //     } else if (frame.opcode == WebSocketFrame::CLOSE) {
        //         ws_conn->close();
        //     }
        // }
    });
    
    app.listen(8080);
}

// Broadcast сообщения всем подключенным клиентам
void broadcast(const std::string& message) {
    static std::vector<std::shared_ptr<WebSocketConnection>> connections;
    static std::mutex connections_mutex;
    
    std::lock_guard lock(connections_mutex);
    for (auto& conn : connections) {
        if (conn->is_connected()) {
            conn->send_text(message);
        }
    }
}

// ============================================
// 📌 Server-Sent Events (SSE)
// ============================================

// --- SSE Connection ---
class SseConnection {
private:
    int socket_fd;
    std::string last_event_id;
    
public:
    SseConnection(int fd) : socket_fd(fd) {
        // Отправка заголовков SSE
        std::string headers = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/event-stream\r\n"
            "Cache-Control: no-cache\r\n"
            "Connection: keep-alive\r\n"
            "\r\n";
        send(socket_fd, headers.c_str(), headers.size(), 0);
    }
    
    // Отправка события
    void send_event(const std::string& data, 
                   const std::string& event = "", 
                   const std::string& id = "") {
        std::ostringstream message;
        
        if (!id.empty()) {
            message << "id: " << id << "\n";
            last_event_id = id;
        }
        
        if (!event.empty()) {
            message << "event: " << event << "\n";
        }
        
        // Данные могут быть многострочными
        std::istringstream data_stream(data);
        std::string line;
        while (std::getline(data_stream, line)) {
            message << "data: " << line << "\n";
        }
        
        message << "\n"; // Двойной перевод строки завершает событие
        
        std::string msg = message.str();
        send(socket_fd, msg.c_str(), msg.size(), 0);
    }
    
    // Комментарий (keep-alive)
    void send_comment(const std::string& text) {
        std::string comment = ": " + text + "\n\n";
        send(socket_fd, comment.c_str(), comment.size(), 0);
    }
    
    // Установка retry (время переподключения)
    void set_retry(int milliseconds) {
        std::string retry = "retry: " + std::to_string(milliseconds) + "\n\n";
        send(socket_fd, retry.c_str(), retry.size(), 0);
    }
};

// --- SSE Server Example ---
void sse_notifications_server() {
    static std::vector<std::shared_ptr<SseConnection>> sse_clients;
    static std::mutex clients_mutex;
    
    HttpServer app;
    
    // SSE endpoint
    app.get("/events", [](const HttpRequestEx& req, HttpResponse& res) {
        // Получение Last-Event-ID для восстановления после разрыва
        auto last_id = req.header("Last-Event-ID");
        
        // Создание SSE соединения (здесь нужен socket_fd)
        // auto sse_conn = std::make_shared<SseConnection>(socket_fd);
        // sse_conn->set_retry(3000); // Переподключение через 3 секунды
        
        // Добавление клиента
        // {
        //     std::lock_guard lock(clients_mutex);
        //     sse_clients.push_back(sse_conn);
        // }
        
        // Keep-alive loop
        // while (true) {
        //     sse_conn->send_comment("ping");
        //     std::this_thread::sleep_for(std::chrono::seconds(30));
        // }
    });
    
    // Пример отправки уведомления всем подключенным
    auto broadcast_notification = []() {
        std::lock_guard lock(clients_mutex);
        for (auto& client : sse_clients) {
            client->send_event(
                "{\"message\": \"New notification\"}",
                "notification",
                std::to_string(std::time(nullptr))
            );
        }
    };
    
    app.listen(8080);
}

// --- Use Cases ---
// 1. Live feed updates (новости, социальные сети)
// 2. Real-time analytics dashboard
// 3. Server monitoring и логи
// 4. Chat notifications (альтернатива WebSocket для односторонней связи)
// 5. Progress tracking (загрузка файлов, batch processing)

// ============================================
// 📌 File Upload Handling
// ============================================

// --- Multipart Form Data Parser ---
class MultipartParser {
public:
    struct Part {
        std::unordered_map<std::string, std::string> headers;
        std::string body;
        
        std::optional<std::string> get_header(const std::string& name) const {
            auto it = headers.find(name);
            return it != headers.end() ? std::optional(it->second) : std::nullopt;
        }
        
        // Извлечение имени поля из Content-Disposition
        std::optional<std::string> get_field_name() const {
            auto disposition = get_header("Content-Disposition");
            if (!disposition) return std::nullopt;
            
            std::regex name_regex(R"(name="([^"]*)")");
            std::smatch match;
            if (std::regex_search(*disposition, match, name_regex)) {
                return match[1].str();
            }
            return std::nullopt;
        }
        
        // Извлечение имени файла
        std::optional<std::string> get_filename() const {
            auto disposition = get_header("Content-Disposition");
            if (!disposition) return std::nullopt;
            
            std::regex filename_regex(R"(filename="([^"]*)")");
            std::smatch match;
            if (std::regex_search(*disposition, match, filename_regex)) {
                return match[1].str();
            }
            return std::nullopt;
        }
    };
    
    static std::vector<Part> parse(const std::string& body, const std::string& boundary) {
        std::vector<Part> parts;
        std::string delimiter = "--" + boundary;
        std::string end_delimiter = "--" + boundary + "--";
        
        size_t pos = 0;
        while ((pos = body.find(delimiter, pos)) != std::string::npos) {
            size_t start = pos + delimiter.size() + 2; // +2 для \r\n
            size_t end = body.find(delimiter, start);
            
            if (end == std::string::npos) break;
            
            std::string part_content = body.substr(start, end - start);
            
            // Разделение headers и body
            size_t header_end = part_content.find("\r\n\r\n");
            if (header_end == std::string::npos) {
                pos = end;
                continue;
            }
            
            Part part;
            std::string headers = part_content.substr(0, header_end);
            part.body = part_content.substr(header_end + 4); // +4 для \r\n\r\n
            
            // Удаление trailing \r\n
            if (part.body.size() >= 2 && part.body.substr(part.body.size() - 2) == "\r\n") {
                part.body.resize(part.body.size() - 2);
            }
            
            // Парсинг заголовков
            std::istringstream header_stream(headers);
            std::string header_line;
            while (std::getline(header_stream, header_line)) {
                auto colon = header_line.find(':');
                if (colon != std::string::npos) {
                    auto key = header_line.substr(0, colon);
                    auto value = header_line.substr(colon + 2); // Skip ": "
                    
                    // Удаление \r
                    if (!value.empty() && value.back() == '\r') {
                        value.pop_back();
                    }
                    
                    part.headers[key] = value;
                }
            }
            
            parts.push_back(part);
            pos = end;
        }
        
        return parts;
    }
};

// --- File Upload Handler ---
void setup_file_upload(HttpServer& app) {
    // POST /upload - загрузка файла
    app.post("/upload", [](const HttpRequestEx& req, HttpResponse& res) {
        auto content_type = req.header("Content-Type");
        if (!content_type || content_type->find("multipart/form-data") == std::string::npos) {
            res.status(400).json("{\"error\": \"Expected multipart/form-data\"}");
            return;
        }
        
        // Извлечение boundary
        std::regex boundary_regex(R"(boundary=([^;]+))");
        std::smatch match;
        std::string boundary;
        if (std::regex_search(*content_type, match, boundary_regex)) {
            boundary = match[1].str();
        } else {
            res.status(400).json("{\"error\": \"Missing boundary\"}");
            return;
        }
        
        // Парсинг multipart data
        auto parts = MultipartParser::parse(req.body(), boundary);
        
        std::vector<std::string> uploaded_files;
        
        for (const auto& part : parts) {
            auto filename = part.get_filename();
            if (!filename) continue; // Это не файл, а обычное поле
            
            // Сохранение файла
            std::string save_path = "./uploads/" + *filename;
            std::ofstream file(save_path, std::ios::binary);
            file.write(part.body.data(), part.body.size());
            file.close();
            
            uploaded_files.push_back(*filename);
        }
        
        // Ответ
        std::ostringstream json_response;
        json_response << "{\"uploaded_files\": [";
        for (size_t i = 0; i < uploaded_files.size(); ++i) {
            json_response << "\"" << uploaded_files[i] << "\"";
            if (i < uploaded_files.size() - 1) json_response << ", ";
        }
        json_response << "]}";
        
        res.status(201).json(json_response.str());
    });
}

// --- Streaming Upload (для больших файлов) ---
class StreamingUploadHandler {
private:
    std::ofstream file_stream;
    size_t bytes_written = 0;
    size_t max_size;
    
public:
    StreamingUploadHandler(const std::string& filepath, size_t max_file_size)
        : max_size(max_file_size) {
        file_stream.open(filepath, std::ios::binary);
    }
    
    bool write_chunk(const char* data, size_t size) {
        if (bytes_written + size > max_size) {
            return false; // Превышен лимит
        }
        
        file_stream.write(data, size);
        bytes_written += size;
        return true;
    }
    
    size_t get_progress() const {
        return bytes_written;
    }
    
    void finalize() {
        file_stream.close();
    }
};

// --- Upload Progress Tracking ---
struct UploadSession {
    std::string session_id;
    std::string filename;
    size_t total_size;
    size_t uploaded_size;
    std::chrono::steady_clock::time_point start_time;
    
    int get_progress_percent() const {
        return total_size > 0 ? (uploaded_size * 100) / total_size : 0;
    }
    
    double get_speed_mbps() const {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time);
        
        if (elapsed.count() == 0) return 0.0;
        
        return (uploaded_size / 1024.0 / 1024.0) / elapsed.count();
    }
};

// ============================================
// 📌 Session Management
// ============================================

// --- Session Store ---
class SessionStore {
public:
    struct Session {
        std::string session_id;
        std::unordered_map<std::string, std::string> data;
        std::chrono::steady_clock::time_point created_at;
        std::chrono::steady_clock::time_point last_accessed;
        int max_age_seconds = 3600; // 1 час по умолчанию
        
        bool is_expired() const {
            auto now = std::chrono::steady_clock::now();
            auto age = std::chrono::duration_cast<std::chrono::seconds>(
                now - last_accessed).count();
            return age > max_age_seconds;
        }
    };
    
private:
    std::unordered_map<std::string, Session> sessions;
    std::mutex mutex;
    
public:
    // Создание новой сессии
    std::string create_session() {
        std::lock_guard lock(mutex);
        
        std::string session_id = generate_uuid();
        Session session;
        session.session_id = session_id;
        session.created_at = std::chrono::steady_clock::now();
        session.last_accessed = session.created_at;
        
        sessions[session_id] = session;
        return session_id;
    }
    
    // Получение сессии
    std::optional<Session> get_session(const std::string& session_id) {
        std::lock_guard lock(mutex);
        
        auto it = sessions.find(session_id);
        if (it == sessions.end()) return std::nullopt;
        
        if (it->second.is_expired()) {
            sessions.erase(it);
            return std::nullopt;
        }
        
        it->second.last_accessed = std::chrono::steady_clock::now();
        return it->second;
    }
    
    // Установка значения в сессию
    void set(const std::string& session_id, const std::string& key, const std::string& value) {
        std::lock_guard lock(mutex);
        
        auto it = sessions.find(session_id);
        if (it != sessions.end()) {
            it->second.data[key] = value;
            it->second.last_accessed = std::chrono::steady_clock::now();
        }
    }
    
    // Получение значения из сессии
    std::optional<std::string> get(const std::string& session_id, const std::string& key) {
        std::lock_guard lock(mutex);
        
        auto it = sessions.find(session_id);
        if (it == sessions.end() || it->second.is_expired()) return std::nullopt;
        
        auto data_it = it->second.data.find(key);
        if (data_it == it->second.data.end()) return std::nullopt;
        
        it->second.last_accessed = std::chrono::steady_clock::now();
        return data_it->second;
    }
    
    // Удаление сессии
    void destroy_session(const std::string& session_id) {
        std::lock_guard lock(mutex);
        sessions.erase(session_id);
    }
    
    // Очистка истекших сессий
    void cleanup_expired() {
        std::lock_guard lock(mutex);
        
        for (auto it = sessions.begin(); it != sessions.end();) {
            if (it->second.is_expired()) {
                it = sessions.erase(it);
            } else {
                ++it;
            }
        }
    }
};

// --- Session Middleware ---
Middleware session_middleware(std::shared_ptr<SessionStore> store) {
    return [store](HttpRequestEx& req, HttpResponse& res) -> bool {
        // Получение session ID из cookie
        auto session_id_cookie = req.cookie("session_id");
        
        std::string session_id;
        if (session_id_cookie) {
            // Проверка существующей сессии
            auto session = store->get_session(*session_id_cookie);
            if (session) {
                session_id = *session_id_cookie;
            } else {
                // Сессия истекла, создаём новую
                session_id = store->create_session();
            }
        } else {
            // Создание новой сессии
            session_id = store->create_session();
        }
        
        // Установка cookie с session ID
        res.set_cookie("session_id", session_id, 3600);
        
        // Сохранение session ID в request для использования в handlers
        // (в реальном коде нужно расширить HttpRequestEx)
        
        return true;
    };
}

// --- CSRF Protection ---
class CsrfProtection {
private:
    std::unordered_map<std::string, std::string> csrf_tokens; // session_id -> token
    std::mutex mutex;
    
public:
    // Генерация CSRF токена для сессии
    std::string generate_token(const std::string& session_id) {
        std::lock_guard lock(mutex);
        
        std::string token = generate_uuid();
        csrf_tokens[session_id] = token;
        return token;
    }
    
    // Проверка CSRF токена
    bool validate_token(const std::string& session_id, const std::string& token) {
        std::lock_guard lock(mutex);
        
        auto it = csrf_tokens.find(session_id);
        if (it == csrf_tokens.end()) return false;
        
        return it->second == token;
    }
};

Middleware csrf_middleware(std::shared_ptr<CsrfProtection> csrf) {
    return [csrf](HttpRequestEx& req, HttpResponse& res) -> bool {
        // CSRF проверка только для модифицирующих методов
        if (req.method() == "GET" || req.method() == "HEAD" || req.method() == "OPTIONS") {
            return true;
        }
        
        auto session_id = req.cookie("session_id");
        if (!session_id) {
            res.status(403).json("{\"error\": \"Missing session\"}");
            return false;
        }
        
        // Получение CSRF токена из заголовка или body
        auto csrf_token = req.header("X-CSRF-Token");
        if (!csrf_token) {
            csrf_token = req.query("csrf_token");
        }
        
        if (!csrf_token || !csrf->validate_token(*session_id, *csrf_token)) {
            res.status(403).json("{\"error\": \"Invalid CSRF token\"}");
            return false;
        }
        
        return true;
    };
}

// ============================================
// 📌 Security
// ============================================

// --- Security Headers Middleware ---
Middleware security_headers_middleware() {
    return [](HttpRequestEx& req, HttpResponse& res) -> bool {
        // Content Security Policy - защита от XSS
        res.set_header("Content-Security-Policy",
            "default-src 'self'; "
            "script-src 'self' 'unsafe-inline'; "
            "style-src 'self' 'unsafe-inline'; "
            "img-src 'self' data:; "
            "font-src 'self'; "
            "connect-src 'self'; "
            "frame-ancestors 'none'");
        
        // Защита от clickjacking
        res.set_header("X-Frame-Options", "DENY");
        
        // Предотвращение MIME type sniffing
        res.set_header("X-Content-Type-Options", "nosniff");
        
        // XSS protection (устаревает, но для совместимости)
        res.set_header("X-XSS-Protection", "1; mode=block");
        
        // HTTPS Strict Transport Security (если используется HTTPS)
        res.set_header("Strict-Transport-Security", 
            "max-age=31536000; includeSubDomains");
        
        // Referrer Policy
        res.set_header("Referrer-Policy", "strict-origin-when-cross-origin");
        
        // Permissions Policy (Feature Policy)
        res.set_header("Permissions-Policy", 
            "geolocation=(), microphone=(), camera=()");
        
        return true;
    };
}

// --- Input Validation ---
class InputValidator {
public:
    // SQL injection prevention - проверка на опасные символы
    static bool is_safe_sql_input(const std::string& input) {
        // Простая проверка (в реальности использовать prepared statements)
        static const std::regex dangerous(R"((--|;|'|\"|\*|\/\*))", 
                                         std::regex::icase);
        return !std::regex_search(input, dangerous);
    }
    
    // XSS prevention - экранирование HTML
    static std::string escape_html(const std::string& input) {
        std::string result;
        result.reserve(input.size());
        
        for (char c : input) {
            switch (c) {
                case '&':  result += "&amp;"; break;
                case '<':  result += "&lt;"; break;
                case '>':  result += "&gt;"; break;
                case '"':  result += "&quot;"; break;
                case '\'': result += "&#x27;"; break;
                case '/':  result += "&#x2F;"; break;
                default:   result += c; break;
            }
        }
        
        return result;
    }
    
    // Path traversal protection
    static bool is_safe_path(const std::string& path) {
        // Проверка на ../
        return path.find("..") == std::string::npos &&
               path.find("~") == std::string::npos;
    }
    
    // Email validation
    static bool is_valid_email(const std::string& email) {
        static const std::regex email_regex(
            R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        return std::regex_match(email, email_regex);
    }
    
    // URL validation
    static bool is_valid_url(const std::string& url) {
        static const std::regex url_regex(
            R"(^https?://[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,}(/.*)?$)");
        return std::regex_match(url, url_regex);
    }
};

// --- Validation Middleware ---
Middleware input_validation_middleware() {
    return [](HttpRequestEx& req, HttpResponse& res) -> bool {
        // Проверка query параметров на path traversal
        // В реальном коде нужно проходить по всем параметрам
        
        // Пример: проверка параметра "file"
        auto file_param = req.query("file");
        if (file_param && !InputValidator::is_safe_path(*file_param)) {
            res.status(400).json("{\"error\": \"Invalid file parameter\"}");
            return false;
        }
        
        return true;
    };
}

// ============================================
// 📌 Performance Optimization
// ============================================

// --- Response Caching ---
class ResponseCache {
private:
    struct CachedResponse {
        std::string body;
        std::unordered_map<std::string, std::string> headers;
        int status_code;
        std::chrono::steady_clock::time_point cached_at;
        int ttl_seconds;
        
        bool is_expired() const {
            auto now = std::chrono::steady_clock::now();
            auto age = std::chrono::duration_cast<std::chrono::seconds>(
                now - cached_at).count();
            return age > ttl_seconds;
        }
    };
    
    std::unordered_map<std::string, CachedResponse> cache;
    std::mutex mutex;
    
public:
    // Сохранение ответа в кэш
    void set(const std::string& key, const HttpResponse& response, int ttl) {
        std::lock_guard lock(mutex);
        
        CachedResponse cached;
        // cached.body = response.get_body(); // Нужен getter
        // cached.headers = response.get_headers();
        // cached.status_code = response.get_status();
        cached.cached_at = std::chrono::steady_clock::now();
        cached.ttl_seconds = ttl;
        
        cache[key] = cached;
    }
    
    // Получение из кэша
    std::optional<CachedResponse> get(const std::string& key) {
        std::lock_guard lock(mutex);
        
        auto it = cache.find(key);
        if (it == cache.end()) return std::nullopt;
        
        if (it->second.is_expired()) {
            cache.erase(it);
            return std::nullopt;
        }
        
        return it->second;
    }
    
    // Очистка
    void clear() {
        std::lock_guard lock(mutex);
        cache.clear();
    }
};

// --- Caching Middleware ---
Middleware caching_middleware(std::shared_ptr<ResponseCache> cache, int ttl = 60) {
    return [cache, ttl](HttpRequestEx& req, HttpResponse& res) -> bool {
        // Кэшируем только GET запросы
        if (req.method() != "GET") return true;
        
        std::string cache_key = req.method() + ":" + req.path();
        
        // Проверка кэша
        auto cached = cache->get(cache_key);
        if (cached) {
            // Возврат кэшированного ответа
            // res = build_from_cached(*cached);
            return false; // Останавливаем обработку
        }
        
        // После выполнения handler нужно сохранить ответ
        // (требуется post-processing hook)
        
        return true;
    };
}

// --- ETag Support ---
class ETagGenerator {
public:
    // Генерация ETag из содержимого (упрощенная версия)
    static std::string generate(const std::string& content) {
        // В реальности использовать MD5 или SHA-1
        std::hash<std::string> hasher;
        size_t hash = hasher(content);
        return "\"" + std::to_string(hash) + "\"";
    }
};

Middleware etag_middleware() {
    return [](HttpRequestEx& req, HttpResponse& res) -> bool {
        // Генерация ETag после создания ответа
        // (требуется post-processing)
        
        // auto etag = ETagGenerator::generate(res.get_body());
        // res.set_header("ETag", etag);
        
        // Проверка If-None-Match
        auto if_none_match = req.header("If-None-Match");
        // if (if_none_match && *if_none_match == etag) {
        //     res.status(304).send("");
        //     return false;
        // }
        
        return true;
    };
}

// --- Compression (gzip) ---
#include <zlib.h>

class GzipCompressor {
public:
    static std::vector<uint8_t> compress(const std::string& data) {
        z_stream stream{};
        stream.zalloc = Z_NULL;
        stream.zfree = Z_NULL;
        stream.opaque = Z_NULL;
        
        if (deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
            return {};
        }
        
        stream.avail_in = data.size();
        stream.next_in = (Bytef*)data.data();
        
        std::vector<uint8_t> compressed;
        compressed.resize(data.size());
        
        stream.avail_out = compressed.size();
        stream.next_out = compressed.data();
        
        deflate(&stream, Z_FINISH);
        compressed.resize(stream.total_out);
        
        deflateEnd(&stream);
        return compressed;
    }
};

// --- Keep-Alive Connection Management ---
class ConnectionPool {
private:
    struct Connection {
        int socket_fd;
        std::chrono::steady_clock::time_point last_used;
        int requests_served = 0;
        int max_requests = 100; // HTTP/1.1 рекомендует ограничение
        
        bool should_close() const {
            auto now = std::chrono::steady_clock::now();
            auto idle = std::chrono::duration_cast<std::chrono::seconds>(
                now - last_used).count();
            
            return idle > 30 || requests_served >= max_requests;
        }
    };
    
    std::unordered_map<int, Connection> connections;
    std::mutex mutex;
    
public:
    void register_connection(int socket_fd) {
        std::lock_guard lock(mutex);
        
        Connection conn;
        conn.socket_fd = socket_fd;
        conn.last_used = std::chrono::steady_clock::now();
        connections[socket_fd] = conn;
    }
    
    bool should_keep_alive(int socket_fd) {
        std::lock_guard lock(mutex);
        
        auto it = connections.find(socket_fd);
        if (it == connections.end()) return false;
        
        it->second.requests_served++;
        it->second.last_used = std::chrono::steady_clock::now();
        
        return !it->second.should_close();
    }
    
    void cleanup_idle() {
        std::lock_guard lock(mutex);
        
        for (auto it = connections.begin(); it != connections.end();) {
            if (it->second.should_close()) {
                close(it->second.socket_fd);
                it = connections.erase(it);
            } else {
                ++it;
            }
        }
    }
};

// --- HTTP/2 Support (концептуально) ---
// HTTP/2 использует:
// • Multiplexing - несколько запросов в одном TCP соединении
// • Server Push - сервер может отправлять ресурсы до запроса
// • Header Compression (HPACK)
// • Binary Framing - бинарный протокол вместо текстового
// • Stream Prioritization

struct Http2Frame {
    uint32_t length;     // 24 bits
    uint8_t type;        // DATA, HEADERS, PRIORITY, etc.
    uint8_t flags;       // END_STREAM, END_HEADERS, etc.
    uint32_t stream_id;  // 31 bits
    std::vector<uint8_t> payload;
};

// ============================================
// 📌 Deployment Patterns
// ============================================

// --- Graceful Shutdown ---
class GracefulShutdown {
private:
    std::atomic<bool> shutdown_requested{false};
    std::condition_variable cv;
    std::mutex mutex;
    
public:
    void request_shutdown() {
        shutdown_requested = true;
        cv.notify_all();
    }
    
    bool is_shutdown_requested() const {
        return shutdown_requested;
    }
    
    // Ожидание завершения с таймаутом
    void wait_for_shutdown(std::chrono::seconds timeout) {
        std::unique_lock lock(mutex);
        cv.wait_for(lock, timeout, [this] { return shutdown_requested.load(); });
    }
};

// --- Signal Handling ---
#include <signal.h>

GracefulShutdown* global_shutdown_handler = nullptr;

void signal_handler(int signal) {
    if (signal == SIGTERM || signal == SIGINT) {
        std::cout << "\nReceived shutdown signal, gracefully stopping..." << std::endl;
        if (global_shutdown_handler) {
            global_shutdown_handler->request_shutdown();
        }
    }
}

void setup_signal_handlers(GracefulShutdown& handler) {
    global_shutdown_handler = &handler;
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
}

// --- Health Check Endpoint ---
void add_health_check(HttpServer& app) {
    // GET /health - проверка здоровья сервиса
    app.get("/health", [](const HttpRequestEx& req, HttpResponse& res) {
        // Проверка различных компонентов
        bool database_ok = true;  // Проверка БД
        bool cache_ok = true;     // Проверка кэша
        bool disk_ok = true;      // Проверка места на диске
        
        if (database_ok && cache_ok && disk_ok) {
            res.status(200).json("{\"status\": \"healthy\"}");
        } else {
            res.status(503).json("{\"status\": \"unhealthy\"}");
        }
    });
    
    // GET /ready - проверка готовности принимать трафик
    app.get("/ready", [](const HttpRequestEx& req, HttpResponse& res) {
        // Проверка, что все инициализировано
        bool is_ready = true; // Проверка состояния
        
        if (is_ready) {
            res.status(200).json("{\"status\": \"ready\"}");
        } else {
            res.status(503).json("{\"status\": \"not ready\"}");
        }
    });
}

// --- Metrics Endpoint (Prometheus format) ---
class MetricsCollector {
private:
    std::atomic<uint64_t> total_requests{0};
    std::atomic<uint64_t> failed_requests{0};
    std::unordered_map<std::string, std::atomic<uint64_t>> endpoint_requests;
    std::mutex mutex;
    
public:
    void record_request(const std::string& endpoint, bool success) {
        total_requests++;
        if (!success) failed_requests++;
        
        std::lock_guard lock(mutex);
        endpoint_requests[endpoint]++;
    }
    
    std::string export_prometheus() {
        std::ostringstream metrics;
        
        // Total requests counter
        metrics << "# HELP http_requests_total Total HTTP requests\n";
        metrics << "# TYPE http_requests_total counter\n";
        metrics << "http_requests_total " << total_requests << "\n\n";
        
        // Failed requests
        metrics << "# HELP http_requests_failed Total failed HTTP requests\n";
        metrics << "# TYPE http_requests_failed counter\n";
        metrics << "http_requests_failed " << failed_requests << "\n\n";
        
        // Per-endpoint requests
        metrics << "# HELP http_requests_by_endpoint Requests by endpoint\n";
        metrics << "# TYPE http_requests_by_endpoint counter\n";
        
        std::lock_guard lock(mutex);
        for (const auto& [endpoint, count] : endpoint_requests) {
            metrics << "http_requests_by_endpoint{endpoint=\"" << endpoint 
                   << "\"} " << count << "\n";
        }
        
        return metrics.str();
    }
};

void add_metrics_endpoint(HttpServer& app, std::shared_ptr<MetricsCollector> metrics) {
    app.get("/metrics", [metrics](const HttpRequestEx& req, HttpResponse& res) {
        res.set_header("Content-Type", "text/plain; version=0.0.4")
           .send(metrics->export_prometheus());
    });
}

// --- Structured Logging ---
enum class LogLevel { DEBUG, INFO, WARN, ERROR };

class Logger {
private:
    LogLevel min_level = LogLevel::INFO;
    std::mutex mutex;
    
public:
    void set_level(LogLevel level) {
        min_level = level;
    }
    
    void log(LogLevel level, const std::string& message,
             const std::unordered_map<std::string, std::string>& fields = {}) {
        if (level < min_level) return;
        
        std::lock_guard lock(mutex);
        
        // Structured JSON logging
        std::ostringstream log_entry;
        log_entry << "{"
                 << "\"timestamp\": \"" << current_time() << "\", "
                 << "\"level\": \"" << level_to_string(level) << "\", "
                 << "\"message\": \"" << message << "\"";
        
        for (const auto& [key, value] : fields) {
            log_entry << ", \"" << key << "\": \"" << value << "\"";
        }
        
        log_entry << "}\n";
        std::cout << log_entry.str();
    }
    
    void debug(const std::string& msg, auto fields = {}) { log(LogLevel::DEBUG, msg, fields); }
    void info(const std::string& msg, auto fields = {}) { log(LogLevel::INFO, msg, fields); }
    void warn(const std::string& msg, auto fields = {}) { log(LogLevel::WARN, msg, fields); }
    void error(const std::string& msg, auto fields = {}) { log(LogLevel::ERROR, msg, fields); }
    
private:
    std::string level_to_string(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARN";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }
};

// --- Configuration Management ---
class Config {
private:
    std::unordered_map<std::string, std::string> config_values;
    
public:
    // Загрузка из environment variables
    void load_from_env() {
        auto get_env = [](const char* name, const std::string& default_val) {
            const char* value = std::getenv(name);
            return value ? std::string(value) : default_val;
        };
        
        config_values["PORT"] = get_env("PORT", "8080");
        config_values["LOG_LEVEL"] = get_env("LOG_LEVEL", "INFO");
        config_values["DB_HOST"] = get_env("DB_HOST", "localhost");
        config_values["REDIS_URL"] = get_env("REDIS_URL", "redis://localhost:6379");
    }
    
    std::string get(const std::string& key, const std::string& default_val = "") const {
        auto it = config_values.find(key);
        return it != config_values.end() ? it->second : default_val;
    }
    
    int get_int(const std::string& key, int default_val = 0) const {
        try {
            return std::stoi(get(key, std::to_string(default_val)));
        } catch (...) {
            return default_val;
        }
    }
};

// --- Complete Production-Ready Server Example ---
int main() {
    // Конфигурация
    Config config;
    config.load_from_env();
    
    // Логирование
    auto logger = std::make_shared<Logger>();
    logger->set_level(LogLevel::INFO);
    
    // Метрики
    auto metrics = std::make_shared<MetricsCollector>();
    
    // Graceful shutdown
    GracefulShutdown shutdown_handler;
    setup_signal_handlers(shutdown_handler);
    
    // Создание сервера
    HttpServer app;
    
    // Middleware
    app.use(logging_middleware());
    app.use(security_headers_middleware());
    app.use(cors_middleware("*"));
    app.use(rate_limit_middleware(100, std::chrono::seconds(60)));
    
    // Health checks и metrics
    add_health_check(app);
    add_metrics_endpoint(app, metrics);
    
    // Бизнес-логика
    app.get("/", [](const HttpRequestEx& req, HttpResponse& res) {
        res.html("<h1>Production C++ Web Server</h1>");
    });
    
    // Статические файлы
    app.static_files("/static", "./public");
    
    // Запуск сервера
    int port = config.get_int("PORT", 8080);
    logger->info("Starting server", {{"port", std::to_string(port)}});
    
    std::thread server_thread([&app, port]() {
        app.listen(port);
    });
    
    // Ожидание сигнала завершения
    shutdown_handler.wait_for_shutdown(std::chrono::seconds(3600));
    
    // Graceful shutdown
    logger->info("Shutting down server");
    app.stop();
    server_thread.join();
    
    logger->info("Server stopped");
    return 0;
}