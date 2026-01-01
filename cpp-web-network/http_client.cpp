// ============================================
// 📌 HTTP Protocol Basics
// ============================================

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <expected>
#include <iostream>
#include <sstream>
#include <regex>

// HTTP/1.1 структура запроса:
// 
// GET /path HTTP/1.1\r\n
// Host: example.com\r\n
// User-Agent: MyClient/1.0\r\n
// Accept: */*\r\n
// \r\n

// HTTP/1.1 структура ответа:
//
// HTTP/1.1 200 OK\r\n
// Content-Type: text/html\r\n
// Content-Length: 1234\r\n
// \r\n
// <body data>

void http_basics() {
    // HTTP Methods:
    // GET    - получение ресурса
    // POST   - создание ресурса
    // PUT    - замена ресурса
    // PATCH  - частичное обновление
    // DELETE - удаление ресурса
    // HEAD   - получение только заголовков
    // OPTIONS - получение поддерживаемых методов
    
    // Status Codes:
    // 1xx - Informational
    // 2xx - Success (200 OK, 201 Created, 204 No Content)
    // 3xx - Redirection (301 Moved Permanently, 302 Found, 304 Not Modified)
    // 4xx - Client Error (400 Bad Request, 401 Unauthorized, 404 Not Found)
    // 5xx - Server Error (500 Internal Server Error, 503 Service Unavailable)
    
    // Important Headers:
    // Host: example.com           - обязательный в HTTP/1.1
    // Content-Type: application/json - тип данных в теле
    // Content-Length: 1234        - размер тела
    // User-Agent: MyClient/1.0    - идентификация клиента
    // Authorization: Bearer token - аутентификация
    // Accept: application/json    - предпочитаемый формат ответа
    // Connection: keep-alive      - повторное использование соединения
}

// Transfer-Encoding: chunked пример:
// 
// 5\r\n
// Hello\r\n
// 7\r\n
// , World\r\n
// 0\r\n
// \r\n

// Keep-Alive позволяет повторно использовать TCP соединение
// для нескольких HTTP запросов (экономия на handshake)

// ============================================
// 📌 Simple HTTP Client
// ============================================

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

// URL Parser - разбор URL на компоненты
struct URL {
    std::string scheme;    // http, https
    std::string host;      // example.com
    std::string port;      // 80, 443
    std::string path;      // /api/users
    std::string query;     // ?id=123
    
    static std::optional<URL> parse(const std::string& url) {
        std::regex url_regex(R"(^(https?):\/\/([^:\/]+)(?::(\d+))?([^?]*)(?:\?(.*))?$)");
        std::smatch match;
        
        if (!std::regex_match(url, match, url_regex)) {
            return std::nullopt;
        }
        
        URL result;
        result.scheme = match[1];
        result.host = match[2];
        result.port = match[3].matched ? match[3].str() : (result.scheme == "https" ? "443" : "80");
        result.path = match[4].matched ? match[4].str() : "/";
        result.query = match[5].matched ? match[5].str() : "";
        
        return result;
    }
};

// Простой синхронный HTTP GET запрос
std::string http_get_simple(const std::string& url_str) {
    // 1. Парсинг URL
    auto url = URL::parse(url_str);
    if (!url) {
        return "Invalid URL";
    }
    
    // 2. DNS resolution
    addrinfo hints{}, *result;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    if (getaddrinfo(url->host.c_str(), url->port.c_str(), &hints, &result) != 0) {
        return "DNS resolution failed";
    }
    
    // 3. TCP подключение
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (connect(sockfd, result->ai_addr, result->ai_addrlen) < 0) {
        freeaddrinfo(result);
        close(sockfd);
        return "Connection failed";
    }
    
    freeaddrinfo(result);
    
    // 4. Формирование и отправка HTTP запроса
    std::ostringstream request;
    request << "GET " << url->path;
    if (!url->query.empty()) {
        request << "?" << url->query;
    }
    request << " HTTP/1.1\r\n";
    request << "Host: " << url->host << "\r\n";
    request << "User-Agent: SimpleHTTPClient/1.0\r\n";
    request << "Accept: */*\r\n";
    request << "Connection: close\r\n";
    request << "\r\n";
    
    std::string req_str = request.str();
    send(sockfd, req_str.c_str(), req_str.size(), 0);
    
    // 5. Получение ответа
    std::string response;
    char buffer[4096];
    
    while (true) {
        ssize_t bytes = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;
        response.append(buffer, bytes);
    }
    
    close(sockfd);
    
    return response;
}

// HTTP POST с JSON телом
std::string http_post_json(const std::string& url_str, const std::string& json_body) {
    auto url = URL::parse(url_str);
    if (!url) return "Invalid URL";
    
    // DNS + Connect (упрощено)
    int sockfd = 0;  // ... создание соединения ...
    
    // Формирование POST запроса
    std::ostringstream request;
    request << "POST " << url->path << " HTTP/1.1\r\n";
    request << "Host: " << url->host << "\r\n";
    request << "Content-Type: application/json\r\n";
    request << "Content-Length: " << json_body.size() << "\r\n";
    request << "Connection: close\r\n";
    request << "\r\n";
    request << json_body;
    
    std::string req_str = request.str();
    send(sockfd, req_str.c_str(), req_str.size(), 0);
    
    // Получение ответа...
    std::string response;
    close(sockfd);
    
    return response;
}

// HTTP POST с form-urlencoded данными
std::string url_encode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;
    
    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << int((unsigned char)c);
        }
    }
    
    return escaped.str();
}

std::string http_post_form(const std::string& url_str,
                           const std::unordered_map<std::string, std::string>& form_data) {
    // Формирование form body
    std::ostringstream body;
    bool first = true;
    
    for (const auto& [key, value] : form_data) {
        if (!first) body << "&";
        body << url_encode(key) << "=" << url_encode(value);
        first = false;
    }
    
    std::string body_str = body.str();
    
    // POST запрос
    auto url = URL::parse(url_str);
    // ...
    
    std::ostringstream request;
    request << "POST " << url->path << " HTTP/1.1\r\n";
    request << "Host: " << url->host << "\r\n";
    request << "Content-Type: application/x-www-form-urlencoded\r\n";
    request << "Content-Length: " << body_str.size() << "\r\n";
    request << "\r\n";
    request << body_str;
    
    // Отправка...
    return "";
}

// Multipart/form-data для загрузки файлов
std::string http_post_multipart(const std::string& url_str,
                                const std::unordered_map<std::string, std::string>& fields,
                                const std::vector<std::pair<std::string, std::vector<char>>>& files) {
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    
    std::ostringstream body;
    
    // Текстовые поля
    for (const auto& [name, value] : fields) {
        body << "--" << boundary << "\r\n";
        body << "Content-Disposition: form-data; name=\"" << name << "\"\r\n";
        body << "\r\n";
        body << value << "\r\n";
    }
    
    // Файлы
    for (const auto& [filename, data] : files) {
        body << "--" << boundary << "\r\n";
        body << "Content-Disposition: form-data; name=\"file\"; filename=\"" << filename << "\"\r\n";
        body << "Content-Type: application/octet-stream\r\n";
        body << "\r\n";
        body.write(data.data(), data.size());
        body << "\r\n";
    }
    
    body << "--" << boundary << "--\r\n";
    
    std::string body_str = body.str();
    
    auto url = URL::parse(url_str);
    
    std::ostringstream request;
    request << "POST " << url->path << " HTTP/1.1\r\n";
    request << "Host: " << url->host << "\r\n";
    request << "Content-Type: multipart/form-data; boundary=" << boundary << "\r\n";
    request << "Content-Length: " << body_str.size() << "\r\n";
    request << "\r\n";
    request << body_str;
    
    return "";
}

// ============================================
// 📌 HTTP Response Handling
// ============================================

struct HTTPResponse {
    int status_code;
    std::string status_message;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

// Парсинг HTTP ответа
HTTPResponse parse_http_response(const std::string& raw_response) {
    HTTPResponse response;
    
    size_t pos = 0;
    size_t end_of_line = raw_response.find("\r\n");
    
    // Парсинг статус линии: HTTP/1.1 200 OK
    std::string status_line = raw_response.substr(0, end_of_line);
    std::istringstream status_stream(status_line);
    
    std::string http_version;
    status_stream >> http_version >> response.status_code;
    std::getline(status_stream, response.status_message);
    
    // Удаляем начальный пробел
    if (!response.status_message.empty() && response.status_message[0] == ' ') {
        response.status_message = response.status_message.substr(1);
    }
    
    pos = end_of_line + 2;
    
    // Парсинг заголовков
    while (true) {
        end_of_line = raw_response.find("\r\n", pos);
        
        if (end_of_line == pos) {
            // Пустая строка - конец заголовков
            pos += 2;
            break;
        }
        
        std::string header_line = raw_response.substr(pos, end_of_line - pos);
        size_t colon = header_line.find(':');
        
        if (colon != std::string::npos) {
            std::string key = header_line.substr(0, colon);
            std::string value = header_line.substr(colon + 1);
            
            // Удаляем начальные пробелы из value
            size_t start = value.find_first_not_of(" \t");
            if (start != std::string::npos) {
                value = value.substr(start);
            }
            
            // Приводим ключ к lowercase для case-insensitive доступа
            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            
            response.headers[key] = value;
        }
        
        pos = end_of_line + 2;
    }
    
    // Тело ответа
    response.body = raw_response.substr(pos);
    
    return response;
}

// Проверка успешности статус кода
bool is_success(int status_code) {
    return status_code >= 200 && status_code < 300;
}

bool is_redirect(int status_code) {
    return status_code >= 300 && status_code < 400;
}

bool is_client_error(int status_code) {
    return status_code >= 400 && status_code < 500;
}

bool is_server_error(int status_code) {
    return status_code >= 500 && status_code < 600;
}

// Chunked transfer encoding декодирование
std::string decode_chunked(const std::string& chunked_body) {
    std::string result;
    size_t pos = 0;
    
    while (pos < chunked_body.size()) {
        // Читаем размер чанка (hex)
        size_t end_of_line = chunked_body.find("\r\n", pos);
        std::string size_str = chunked_body.substr(pos, end_of_line - pos);
        
        size_t chunk_size = std::stoul(size_str, nullptr, 16);
        
        if (chunk_size == 0) {
            // Последний чанк
            break;
        }
        
        pos = end_of_line + 2;
        
        // Читаем данные чанка
        result.append(chunked_body.substr(pos, chunk_size));
        pos += chunk_size + 2;  // +2 для \r\n после чанка
    }
    
    return result;
}

// Обработка редиректов
std::string follow_redirects(const std::string& initial_url, int max_redirects = 10) {
    std::string current_url = initial_url;
    int redirect_count = 0;
    
    while (redirect_count < max_redirects) {
        std::string response = http_get_simple(current_url);
        HTTPResponse parsed = parse_http_response(response);
        
        if (!is_redirect(parsed.status_code)) {
            // Не редирект - возвращаем ответ
            return response;
        }
        
        // Получаем URL для редиректа
        auto loc_it = parsed.headers.find("location");
        if (loc_it == parsed.headers.end()) {
            break;  // Нет Location заголовка
        }
        
        current_url = loc_it->second;
        redirect_count++;
        
        std::cout << "Redirect " << redirect_count << ": " << current_url << '\n';
    }
    
    return "Too many redirects";
}

// ============================================
// 📌 Modern HTTP Client Class
// ============================================

enum class HTTPMethod {
    GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS
};

class HTTPClient {
    struct Connection {
        int sockfd;
        std::string host;
        std::chrono::steady_clock::time_point last_used;
    };
    
    std::vector<Connection> connection_pool_;
    std::unordered_map<std::string, std::string> default_headers_;
    std::chrono::seconds timeout_{30};
    int max_redirects_ = 10;
    
public:
    HTTPClient() {
        default_headers_["User-Agent"] = "ModernHTTPClient/2.0";
        default_headers_["Accept"] = "*/*";
    }
    
    // Установка таймаута
    void set_timeout(std::chrono::seconds timeout) {
        timeout_ = timeout;
    }
    
    // Установка заголовка по умолчанию
    void set_default_header(const std::string& key, const std::string& value) {
        default_headers_[key] = value;
    }
    
    // Basic Authentication
    void set_basic_auth(const std::string& username, const std::string& password) {
        std::string credentials = username + ":" + password;
        // Base64 encode (упрощённо)
        std::string encoded = "base64(" + credentials + ")";
        default_headers_["Authorization"] = "Basic " + encoded;
    }
    
    // Bearer Token Authentication
    void set_bearer_token(const std::string& token) {
        default_headers_["Authorization"] = "Bearer " + token;
    }
    
    // GET запрос
    std::expected<HTTPResponse, std::string> get(const std::string& url) {
        return request(HTTPMethod::GET, url, "");
    }
    
    // POST запрос
    std::expected<HTTPResponse, std::string> post(const std::string& url,
                                                   const std::string& body,
                                                   const std::string& content_type = "application/json") {
        auto headers = default_headers_;
        headers["Content-Type"] = content_type;
        headers["Content-Length"] = std::to_string(body.size());
        
        return request(HTTPMethod::POST, url, body, headers);
    }
    
    // Универсальный метод запроса
    std::expected<HTTPResponse, std::string> request(
        HTTPMethod method,
        const std::string& url_str,
        const std::string& body = "",
        const std::unordered_map<std::string, std::string>& extra_headers = {}
    ) {
        auto url = URL::parse(url_str);
        if (!url) {
            return std::unexpected("Invalid URL");
        }
        
        // Получение соединения из пула или создание нового
        int sockfd = get_or_create_connection(url->host, url->port);
        
        if (sockfd < 0) {
            return std::unexpected("Connection failed");
        }
        
        // Формирование запроса
        std::ostringstream request;
        
        // Метод и путь
        request << method_to_string(method) << " " << url->path;
        if (!url->query.empty()) {
            request << "?" << url->query;
        }
        request << " HTTP/1.1\r\n";
        
        // Заголовки
        request << "Host: " << url->host << "\r\n";
        
        auto headers = default_headers_;
        headers.insert(extra_headers.begin(), extra_headers.end());
        
        for (const auto& [key, value] : headers) {
            request << key << ": " << value << "\r\n";
        }
        
        request << "Connection: keep-alive\r\n";
        request << "\r\n";
        
        // Тело (если есть)
        if (!body.empty()) {
            request << body;
        }
        
        // Отправка
        std::string req_str = request.str();
        send(sockfd, req_str.c_str(), req_str.size(), 0);
        
        // Получение ответа
        std::string response_str = receive_response(sockfd);
        
        // Возврат соединения в пул
        return_connection(sockfd, url->host);
        
        // Парсинг ответа
        return parse_http_response(response_str);
    }
    
private:
    std::string method_to_string(HTTPMethod method) {
        switch (method) {
            case HTTPMethod::GET: return "GET";
            case HTTPMethod::POST: return "POST";
            case HTTPMethod::PUT: return "PUT";
            case HTTPMethod::DELETE: return "DELETE";
            case HTTPMethod::PATCH: return "PATCH";
            case HTTPMethod::HEAD: return "HEAD";
            case HTTPMethod::OPTIONS: return "OPTIONS";
        }
        return "GET";
    }
    
    int get_or_create_connection(const std::string& host, const std::string& port) {
        // Поиск существующего соединения
        for (auto& conn : connection_pool_) {
            if (conn.host == host) {
                // Проверка что соединение ещё живо
                return conn.sockfd;
            }
        }
        
        // Создание нового соединения
        addrinfo hints{}, *result;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        
        if (getaddrinfo(host.c_str(), port.c_str(), &hints, &result) != 0) {
            return -1;
        }
        
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        if (connect(sockfd, result->ai_addr, result->ai_addrlen) < 0) {
            freeaddrinfo(result);
            close(sockfd);
            return -1;
        }
        
        freeaddrinfo(result);
        
        // Добавление в пул
        connection_pool_.push_back({sockfd, host, std::chrono::steady_clock::now()});
        
        return sockfd;
    }
    
    void return_connection(int sockfd, const std::string& host) {
        // Обновляем время последнего использования
        for (auto& conn : connection_pool_) {
            if (conn.sockfd == sockfd) {
                conn.last_used = std::chrono::steady_clock::now();
                break;
            }
        }
    }
    
    std::string receive_response(int sockfd) {
        std::string response;
        char buffer[4096];
        
        while (true) {
            ssize_t bytes = recv(sockfd, buffer, sizeof(buffer), 0);
            if (bytes <= 0) break;
            response.append(buffer, bytes);
            
            // Проверка окончания ответа (упрощённо)
            if (response.find("\r\n\r\n") != std::string::npos) {
                // Для chunked или Content-Length нужна более сложная логика
                break;
            }
        }
        
        return response;
    }
};

// Fluent Interface для удобства
class HTTPRequestBuilder {
    HTTPClient& client_;
    std::string url_;
    HTTPMethod method_ = HTTPMethod::GET;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
    
public:
    HTTPRequestBuilder(HTTPClient& client, const std::string& url)
        : client_(client), url_(url) {}
    
    HTTPRequestBuilder& method(HTTPMethod m) {
        method_ = m;
        return *this;
    }
    
    HTTPRequestBuilder& header(const std::string& key, const std::string& value) {
        headers_[key] = value;
        return *this;
    }
    
    HTTPRequestBuilder& body(const std::string& b) {
        body_ = b;
        return *this;
    }
    
    std::expected<HTTPResponse, std::string> send() {
        return client_.request(method_, url_, body_, headers_);
    }
};

// Использование Fluent Interface
void fluent_interface_example() {
    HTTPClient client;
    
    auto response = HTTPRequestBuilder(client, "https://api.example.com/users")
        .method(HTTPMethod::POST)
        .header("Content-Type", "application/json")
        .header("X-Custom-Header", "value")
        .body(R"({"name":"John","email":"john@example.com"})")
        .send();
    
    if (response) {
        std::cout << "Status: " << response->status_code << '\n';
        std::cout << "Body: " << response->body << '\n';
    } else {
        std::cerr << "Error: " << response.error() << '\n';
    }
}

// ============================================
// 📌 Async HTTP Client (примеры концепций)
// ============================================

// Корутина-based HTTP запрос  
Task<HTTPResponse> async_get(const std::string& url) {
    // В реальности используется async socket I/O
    HTTPClient client;
    auto response = client.get(url);
    
    if (response) {
        co_return *response;
    }
    
    throw std::runtime_error(response.error());
}

// Future-based API
std::future<HTTPResponse> get_async_future(const std::string& url) {
    return std::async(std::launch::async, [url]() {
        HTTPClient client;
        auto response = client.get(url);
        
        if (response) {
            return *response;
        }
        
        throw std::runtime_error(response.error());
    });
}

// Параллельные запросы
std::vector<HTTPResponse> parallel_requests(const std::vector<std::string>& urls) {
    std::vector<std::future<HTTPResponse>> futures;
    
    for (const auto& url : urls) {
        futures.push_back(get_async_future(url));
    }
    
    std::vector<HTTPResponse> results;
    for (auto& future : futures) {
        try {
            results.push_back(future.get());
        } catch (const std::exception& e) {
            std::cerr << "Request failed: " << e.what() << '\n';
        }
    }
    
    return results;
}

// ============================================
// 📌 REST API Client Patterns
// ============================================

// JSON API Client (концепция)
class JSONAPIClient {
    HTTPClient client_;
    std::string base_url_;
    
public:
    JSONAPIClient(const std::string& base_url) : base_url_(base_url) {}
    
    // Type-safe GET request
    template<typename T>
    std::expected<T, std::string> get(const std::string& endpoint) {
        auto response = client_.get(base_url_ + endpoint);
        
        if (!response) {
            return std::unexpected(response.error());
        }
        
        if (!is_success(response->status_code)) {
            return std::unexpected("HTTP " + std::to_string(response->status_code));
        }
        
        // JSON парсинг в тип T (упрощённо)
        // T result = parse_json<T>(response->body);
        T result{};
        
        return result;
    }
    
    // Type-safe POST request
    template<typename T, typename U>
    std::expected<U, std::string> post(const std::string& endpoint, const T& data) {
        // JSON сериализация (упрощённо)
        std::string json_body = "{}";  // serialize_json(data);
        
        auto response = client_.post(base_url_ + endpoint, json_body);
        
        if (!response) {
            return std::unexpected(response.error());
        }
        
        if (!is_success(response->status_code)) {
            return std::unexpected("HTTP " + std::to_string(response->status_code));
        }
        
        U result{};  // parse_json<U>(response->body);
        
        return result;
    }
};

// GitHub API Client пример
class GitHubClient {
    HTTPClient client_;
    std::string base_url_ = "https://api.github.com";
    
public:
    GitHubClient(const std::string& access_token) {
        client_.set_bearer_token(access_token);
        client_.set_default_header("Accept", "application/vnd.github.v3+json");
    }
    
    struct Repository {
        std::string name;
        std::string description;
        int stars;
    };
    
    // Получение репозитория
    std::expected<Repository, std::string> get_repo(const std::string& owner,
                                                     const std::string& repo) {
        std::string endpoint = "/repos/" + owner + "/" + repo;
        auto response = client_.get(base_url_ + endpoint);
        
        if (!response) {
            return std::unexpected(response->error());
        }
        
        // Парсинг JSON ответа (упрощённо)
        Repository result;
        // result = parse_json<Repository>(response->body);
        
        return result;
    }
    
    // Pagination handling
    std::vector<Repository> list_user_repos(const std::string& username) {
        std::vector<Repository> all_repos;
        int page = 1;
        
        while (true) {
            std::string endpoint = "/users/" + username + "/repos?page=" 
                                 + std::to_string(page) + "&per_page=100";
            
            auto response = client_.get(base_url_ + endpoint);
            
            if (!response || response->body.empty()) {
                break;
            }
            
            // Парсинг массива репозиториев
            // auto repos = parse_json<std::vector<Repository>>(response->body);
            // all_repos.insert(all_repos.end(), repos.begin(), repos.end());
            
            // Проверка заголовка Link для следующей страницы
            auto link_it = response->headers.find("link");
            if (link_it == response->headers.end() ||
                link_it->second.find("rel=\"next\"") == std::string::npos) {
                break;  // Последняя страница
            }
            
            page++;
        }
        
        return all_repos;
    }
    
    // ETags для кэширования
    std::optional<Repository> get_repo_cached(const std::string& owner,
                                              const std::string& repo,
                                              std::string& etag) {
        HTTPClient client;
        
        if (!etag.empty()) {
            client.set_default_header("If-None-Match", etag);
        }
        
        std::string endpoint = "/repos/" + owner + "/" + repo;
        auto response = client.get(base_url_ + endpoint);
        
        if (!response) {
            return std::nullopt;
        }
        
        if (response->status_code == 304) {
            // Not Modified - используем кэш
            return std::nullopt;
        }
        
        // Сохраняем новый ETag
        auto etag_it = response->headers.find("etag");
        if (etag_it != response->headers.end()) {
            etag = etag_it->second;
        }
        
        Repository result;
        // result = parse_json<Repository>(response->body);
        
        return result;
    }
};

// ============================================
// 📌 Connection Pooling (расширенная версия)
// ============================================

class ConnectionPool {
    struct PooledConnection {
        int sockfd;
        std::string host;
        std::string port;
        std::chrono::steady_clock::time_point last_used;
        bool in_use;
    };
    
    std::vector<PooledConnection> connections_;
    std::mutex mutex_;
    size_t max_connections_per_host_ = 6;
    std::chrono::seconds idle_timeout_{60};
    
public:
    // Получение соединения
    int acquire(const std::string& host, const std::string& port) {
        std::lock_guard lock(mutex_);
        
        // Удаляем старые idle соединения
        cleanup_idle_connections();
        
        // Ищем свободное соединение к этому хосту
        for (auto& conn : connections_) {
            if (conn.host == host && conn.port == port && !conn.in_use) {
                // Проверка здоровья соединения
                if (is_connection_alive(conn.sockfd)) {
                    conn.in_use = true;
                    conn.last_used = std::chrono::steady_clock::now();
                    return conn.sockfd;
                } else {
                    // Соединение мёртвое - закрываем
                    close(conn.sockfd);
                    conn.sockfd = -1;
                }
            }
        }
        
        // Подсчёт активных соединений к этому хосту
        size_t active_count = 0;
        for (const auto& conn : connections_) {
            if (conn.host == host && conn.port == port && conn.sockfd >= 0) {
                active_count++;
            }
        }
        
        if (active_count >= max_connections_per_host_) {
            return -1;  // Достигнут лимит
        }
        
        // Создаём новое соединение
        int sockfd = create_connection(host, port);
        
        if (sockfd >= 0) {
            connections_.push_back({
                sockfd, host, port,
                std::chrono::steady_clock::now(),
                true
            });
        }
        
        return sockfd;
    }
    
    // Возврат соединения в пул
    void release(int sockfd) {
        std::lock_guard lock(mutex_);
        
        for (auto& conn : connections_) {
            if (conn.sockfd == sockfd) {
                conn.in_use = false;
                conn.last_used = std::chrono::steady_clock::now();
                return;
            }
        }
    }
    
    // Закрытие соединения
    void close_connection(int sockfd) {
        std::lock_guard lock(mutex_);
        
        for (auto it = connections_.begin(); it != connections_.end(); ++it) {
            if (it->sockfd == sockfd) {
                close(sockfd);
                connections_.erase(it);
                return;
            }
        }
    }
    
private:
    int create_connection(const std::string& host, const std::string& port) {
        addrinfo hints{}, *result;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        
        if (getaddrinfo(host.c_str(), port.c_str(), &hints, &result) != 0) {
            return -1;
        }
        
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        if (connect(sockfd, result->ai_addr, result->ai_addrlen) < 0) {
            freeaddrinfo(result);
            close(sockfd);
            return -1;
        }
        
        freeaddrinfo(result);
        return sockfd;
    }
    
    bool is_connection_alive(int sockfd) {
        // Проверка с помощью SO_ERROR
        int error;
        socklen_t len = sizeof(error);
        
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
            return false;
        }
        
        return error == 0;
    }
    
    void cleanup_idle_connections() {
        auto now = std::chrono::steady_clock::now();
        
        for (auto it = connections_.begin(); it != connections_.end(); ) {
            if (!it->in_use) {
                auto idle_time = std::chrono::duration_cast<std::chrono::seconds>(
                    now - it->last_used);
                
                if (idle_time > idle_timeout_) {
                    close(it->sockfd);
                    it = connections_.erase(it);
                    continue;
                }
            }
            ++it;
        }
    }
};

// ============================================
// 📌 Advanced Features
// ============================================

// Streaming Download с progress tracking
void download_file(const std::string& url, const std::string& output_file,
                  std::function<void(size_t downloaded, size_t total)> progress_callback) {
    HTTPClient client;
    auto url_parsed = URL::parse(url);
    
    if (!url_parsed) return;
    
    // Получаем размер файла через HEAD запрос
    auto head_response = client.request(HTTPMethod::HEAD, url);
    
    size_t total_size = 0;
    if (head_response) {
        auto cl_it = head_response->headers.find("content-length");
        if (cl_it != head_response->headers.end()) {
            total_size = std::stoull(cl_it->second);
        }
    }
    
    // Открываем файл для записи
    std::ofstream file(output_file, std::ios::binary);
    
    // GET запрос с chunked чтением
    size_t downloaded = 0;
    
    // В реальности здесь нужен streaming response reader
    auto response = client.get(url);
    
    if (response) {
        file.write(response->body.data(), response->body.size());
        downloaded = response->body.size();
        
        if (progress_callback) {
            progress_callback(downloaded, total_size);
        }
    }
    
    file.close();
}

// Proxy Support
class HTTPClientWithProxy : public HTTPClient {
    std::string proxy_host_;
    std::string proxy_port_;
    
public:
    void set_proxy(const std::string& host, const std::string& port) {
        proxy_host_ = host;
        proxy_port_ = port;
    }
    
    // При использовании proxy, подключаемся к proxy и отправляем
    // полный URL вместо только пути
    // 
    // Без proxy: GET /path HTTP/1.1
    // С proxy:   GET http://example.com/path HTTP/1.1
};

// Compression support (gzip)
std::string decompress_gzip(const std::string& compressed) {
    // Использование zlib для декомпрессии
    // z_stream stream;
    // inflateInit2(&stream, 16 + MAX_WBITS);  // 16 для gzip
    // ...
    
    return "decompressed";  // Упрощённо
}

// ============================================
// 📌 Testing и Mocking
// ============================================

// Mock HTTP Server для тестирования
class MockHTTPServer {
    std::unordered_map<std::string, HTTPResponse> mock_responses_;
    
public:
    void mock_response(const std::string& url, const HTTPResponse& response) {
        mock_responses_[url] = response;
    }
    
    HTTPResponse handle_request(const std::string& url) {
        auto it = mock_responses_.find(url);
        
        if (it != mock_responses_.end()) {
            return it->second;
        }
        
        // 404 по умолчанию
        return HTTPResponse{404, "Not Found", {}, ""};
    }
};

// Request Recording для debugging
class RecordingHTTPClient {
    HTTPClient client_;
    
    struct RecordedRequest {
        std::string url;
        HTTPMethod method;
        std::string body;
        std::chrono::system_clock::time_point timestamp;
        HTTPResponse response;
    };
    
    std::vector<RecordedRequest> recorded_requests_;
    
public:
    std::expected<HTTPResponse, std::string> get(const std::string& url) {
        auto response = client_.get(url);
        
        // Записываем запрос
        RecordedRequest record;
        record.url = url;
        record.method = HTTPMethod::GET;
        record.timestamp = std::chrono::system_clock::now();
        
        if (response) {
            record.response = *response;
        }
        
        recorded_requests_.push_back(std::move(record));
        
        return response;
    }
    
    // Вывод всех записанных запросов
    void print_history() const {
        for (const auto& record : recorded_requests_) {
            std::cout << "URL: " << record.url << '\n';
            std::cout << "Status: " << record.response.status_code << '\n';
            std::cout << "---\n";
        }
    }
    
    // Экспорт в файл для replay
    void export_to_file(const std::string& filename) {
        // Сериализация в JSON или другой формат
    }
};

// Integration Test пример
void test_http_client() {
    HTTPClient client;
    
    // Тест GET запроса
    auto response = client.get("https://httpbin.org/get");
    
    assert(response.has_value());
    assert(is_success(response->status_code));
    
    // Тест POST запроса
    auto post_response = client.post("https://httpbin.org/post",
                                     R"({"key":"value"})",
                                     "application/json");
    
    assert(post_response.has_value());
    assert(post_response->status_code == 200);
    
    std::cout << "All tests passed!\n";
}