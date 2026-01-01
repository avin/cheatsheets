// ============================================
// 📌 WebSocket Protocol
// ============================================

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

// RFC 6455 - WebSocket протокол поверх HTTP
// Преимущества:
// • Full-duplex двусторонняя связь
// • Низкая задержка (low latency)
// • Меньше overhead чем HTTP polling
// • Real-time обновления

// ============================================
// 📌 WebSocket Handshake
// ============================================

class WebSocketHandshake {
public:
    // Генерация Sec-WebSocket-Accept ключа
    static std::string compute_accept_key(const std::string& client_key) {
        // RFC 6455: Sec-WebSocket-Accept = base64(SHA1(Sec-WebSocket-Key + GUID))
        const std::string GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        std::string combined = client_key + GUID;
        
        // SHA-1 хэш
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char*>(combined.c_str()), 
             combined.size(), hash);
        
        // Base64 кодирование
        return base64_encode(hash, SHA_DIGEST_LENGTH);
    }
    
    // Генерация случайного Sec-WebSocket-Key для клиента
    static std::string generate_client_key() {
        unsigned char random_bytes[16];
        // RAND_bytes(random_bytes, 16); // OpenSSL random
        
        // Упрощенная генерация для примера
        for (int i = 0; i < 16; ++i) {
            random_bytes[i] = rand() % 256;
        }
        
        return base64_encode(random_bytes, 16);
    }
    
    // Создание HTTP Upgrade запроса (client)
    static std::string create_client_handshake(const std::string& host, 
                                               const std::string& path,
                                               const std::string& key) {
        std::ostringstream request;
        request << "GET " << path << " HTTP/1.1\r\n"
                << "Host: " << host << "\r\n"
                << "Upgrade: websocket\r\n"
                << "Connection: Upgrade\r\n"
                << "Sec-WebSocket-Key: " << key << "\r\n"
                << "Sec-WebSocket-Version: 13\r\n"
                << "\r\n";
        return request.str();
    }
    
    // Создание HTTP Upgrade ответа (server)
    static std::string create_server_handshake(const std::string& accept_key) {
        std::ostringstream response;
        response << "HTTP/1.1 101 Switching Protocols\r\n"
                 << "Upgrade: websocket\r\n"
                 << "Connection: Upgrade\r\n"
                 << "Sec-WebSocket-Accept: " << accept_key << "\r\n"
                 << "\r\n";
        return response.str();
    }
    
private:
    static std::string base64_encode(const unsigned char* data, size_t length) {
        BIO *bio, *b64;
        BUF_MEM *buffer;
        
        b64 = BIO_new(BIO_f_base64());
        bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);
        
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        BIO_write(bio, data, length);
        BIO_flush(bio);
        
        BIO_get_mem_ptr(bio, &buffer);
        std::string result(buffer->data, buffer->length);
        
        BIO_free_all(bio);
        return result;
    }
};

// ============================================
// 📌 Frame Parsing
// ============================================

// WebSocket Frame структура:
// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-------+-+-------------+-------------------------------+
// |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
// |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
// |N|V|V|V|       |S|             |   (if payload len==126/127)   |
// | |1|2|3|       |K|             |                               |
// +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +

struct WebSocketFrame {
    enum Opcode : uint8_t {
        CONTINUATION = 0x0,  // Продолжение фрагментированного сообщения
        TEXT = 0x1,          // Текстовые данные (UTF-8)
        BINARY = 0x2,        // Бинарные данные
        CLOSE = 0x8,         // Закрытие соединения
        PING = 0x9,          // Ping
        PONG = 0xA           // Pong (ответ на Ping)
    };
    
    bool fin;                      // FIN bit - последний фрагмент?
    uint8_t rsv1, rsv2, rsv3;      // Резервные биты (для расширений)
    Opcode opcode;                 // Тип фрейма
    bool masked;                   // Маскировка данных (клиент всегда маскирует)
    uint64_t payload_length;       // Длина payload
    uint8_t masking_key[4];        // Ключ маскировки (если masked=true)
    std::vector<uint8_t> payload;  // Данные
    
    // Парсинг фрейма из буфера
    static std::optional<WebSocketFrame> parse(const uint8_t* data, size_t len, size_t& consumed) {
        if (len < 2) return std::nullopt; // Минимум 2 байта
        
        WebSocketFrame frame;
        size_t offset = 0;
        
        // Первый байт: FIN + RSV + Opcode
        frame.fin = (data[0] & 0x80) != 0;
        frame.rsv1 = (data[0] & 0x40) != 0;
        frame.rsv2 = (data[0] & 0x20) != 0;
        frame.rsv3 = (data[0] & 0x10) != 0;
        frame.opcode = static_cast<Opcode>(data[0] & 0x0F);
        
        // Второй байт: MASK + Payload Length
        frame.masked = (data[1] & 0x80) != 0;
        uint8_t payload_len = data[1] & 0x7F;
        offset = 2;
        
        // Расширенная длина payload
        if (payload_len == 126) {
            if (len < offset + 2) return std::nullopt;
            frame.payload_length = (static_cast<uint16_t>(data[offset]) << 8) | 
                                   data[offset + 1];
            offset += 2;
        } else if (payload_len == 127) {
            if (len < offset + 8) return std::nullopt;
            frame.payload_length = 0;
            for (int i = 0; i < 8; ++i) {
                frame.payload_length = (frame.payload_length << 8) | data[offset + i];
            }
            offset += 8;
        } else {
            frame.payload_length = payload_len;
        }
        
        // Masking key (если есть)
        if (frame.masked) {
            if (len < offset + 4) return std::nullopt;
            memcpy(frame.masking_key, data + offset, 4);
            offset += 4;
        }
        
        // Payload data
        if (len < offset + frame.payload_length) return std::nullopt;
        
        frame.payload.resize(frame.payload_length);
        memcpy(frame.payload.data(), data + offset, frame.payload_length);
        
        // Демаскировка (если нужно)
        if (frame.masked) {
            for (size_t i = 0; i < frame.payload_length; ++i) {
                frame.payload[i] ^= frame.masking_key[i % 4];
            }
        }
        
        consumed = offset + frame.payload_length;
        return frame;
    }
    
    // Создание фрейма для отправки
    static std::vector<uint8_t> create(Opcode opcode, const std::vector<uint8_t>& data,
                                       bool fin = true, bool mask = false) {
        std::vector<uint8_t> frame;
        
        // Первый байт: FIN + RSV + Opcode
        uint8_t byte1 = opcode;
        if (fin) byte1 |= 0x80;
        frame.push_back(byte1);
        
        // Второй байт: MASK + Payload Length
        uint64_t len = data.size();
        uint8_t byte2 = 0;
        if (mask) byte2 |= 0x80;
        
        if (len <= 125) {
            byte2 |= static_cast<uint8_t>(len);
            frame.push_back(byte2);
        } else if (len <= 65535) {
            byte2 |= 126;
            frame.push_back(byte2);
            frame.push_back((len >> 8) & 0xFF);
            frame.push_back(len & 0xFF);
        } else {
            byte2 |= 127;
            frame.push_back(byte2);
            for (int i = 7; i >= 0; --i) {
                frame.push_back((len >> (i * 8)) & 0xFF);
            }
        }
        
        // Masking key и данные
        if (mask) {
            uint8_t masking_key[4];
            for (int i = 0; i < 4; ++i) {
                masking_key[i] = rand() % 256;
                frame.push_back(masking_key[i]);
            }
            
            // Маскированные данные
            for (size_t i = 0; i < data.size(); ++i) {
                frame.push_back(data[i] ^ masking_key[i % 4]);
            }
        } else {
            // Немаскированные данные (сервер не маскирует)
            frame.insert(frame.end(), data.begin(), data.end());
        }
        
        return frame;
    }
    
    // Создание текстового фрейма
    static std::vector<uint8_t> create_text(const std::string& text, bool mask = false) {
        std::vector<uint8_t> data(text.begin(), text.end());
        return create(TEXT, data, true, mask);
    }
    
    // Создание close фрейма
    static std::vector<uint8_t> create_close(uint16_t code = 1000, 
                                              const std::string& reason = "",
                                              bool mask = false) {
        std::vector<uint8_t> data;
        data.push_back((code >> 8) & 0xFF);
        data.push_back(code & 0xFF);
        data.insert(data.end(), reason.begin(), reason.end());
        return create(CLOSE, data, true, mask);
    }
};

// ============================================
// 📌 WebSocket Connection Class
// ============================================

enum class ConnectionState {
    CONNECTING,
    OPEN,
    CLOSING,
    CLOSED
};

class WebSocketConnection {
private:
    int socket_fd;
    ConnectionState state = ConnectionState::OPEN;
    std::string id;
    std::vector<uint8_t> fragment_buffer; // Для фрагментированных сообщений
    
    // Callbacks
    std::function<void(const std::string&)> on_text_message;
    std::function<void(const std::vector<uint8_t>&)> on_binary_message;
    std::function<void(uint16_t, const std::string&)> on_close_callback;
    std::function<void(const std::string&)> on_error_callback;
    
public:
    WebSocketConnection(int fd, const std::string& conn_id) 
        : socket_fd(fd), id(conn_id) {}
    
    const std::string& get_id() const { return id; }
    
    // Отправка текстового сообщения
    void send_text(const std::string& message) {
        if (state != ConnectionState::OPEN) return;
        
        auto frame = WebSocketFrame::create_text(message, false);
        send(socket_fd, frame.data(), frame.size(), 0);
    }
    
    // Отправка бинарных данных
    void send_binary(const std::vector<uint8_t>& data) {
        if (state != ConnectionState::OPEN) return;
        
        auto frame = WebSocketFrame::create(WebSocketFrame::BINARY, data, true, false);
        send(socket_fd, frame.data(), frame.size(), 0);
    }
    
    // Отправка ping
    void ping() {
        std::vector<uint8_t> empty;
        auto frame = WebSocketFrame::create(WebSocketFrame::PING, empty, true, false);
        send(socket_fd, frame.data(), frame.size(), 0);
    }
    
    // Отправка pong
    void pong() {
        std::vector<uint8_t> empty;
        auto frame = WebSocketFrame::create(WebSocketFrame::PONG, empty, true, false);
        send(socket_fd, frame.data(), frame.size(), 0);
    }
    
    // Закрытие соединения
    void close(uint16_t code = 1000, const std::string& reason = "") {
        if (state == ConnectionState::CLOSING || state == ConnectionState::CLOSED) return;
        
        state = ConnectionState::CLOSING;
        auto frame = WebSocketFrame::create_close(code, reason, false);
        send(socket_fd, frame.data(), frame.size(), 0);
        
        // После отправки close фрейма ждём close от клиента
    }
    
    // Обработка входящего фрейма
    void handle_frame(const WebSocketFrame& frame) {
        switch (frame.opcode) {
            case WebSocketFrame::TEXT:
                if (frame.fin) {
                    std::string message(frame.payload.begin(), frame.payload.end());
                    if (on_text_message) on_text_message(message);
                } else {
                    // Начало фрагментированного сообщения
                    fragment_buffer = frame.payload;
                }
                break;
                
            case WebSocketFrame::BINARY:
                if (frame.fin) {
                    if (on_binary_message) on_binary_message(frame.payload);
                } else {
                    fragment_buffer = frame.payload;
                }
                break;
                
            case WebSocketFrame::CONTINUATION:
                fragment_buffer.insert(fragment_buffer.end(), 
                                      frame.payload.begin(), frame.payload.end());
                if (frame.fin) {
                    // Завершение фрагментированного сообщения
                    if (on_binary_message) on_binary_message(fragment_buffer);
                    fragment_buffer.clear();
                }
                break;
                
            case WebSocketFrame::PING:
                pong();
                break;
                
            case WebSocketFrame::PONG:
                // Получен pong, соединение живо
                break;
                
            case WebSocketFrame::CLOSE:
                handle_close_frame(frame);
                break;
        }
    }
    
    // Установка callbacks
    void on_message(std::function<void(const std::string&)> cb) {
        on_text_message = cb;
    }
    
    void on_close(std::function<void(uint16_t, const std::string&)> cb) {
        on_close_callback = cb;
    }
    
private:
    void handle_close_frame(const WebSocketFrame& frame) {
        uint16_t code = 1000;
        std::string reason;
        
        if (frame.payload.size() >= 2) {
            code = (frame.payload[0] << 8) | frame.payload[1];
            if (frame.payload.size() > 2) {
                reason = std::string(frame.payload.begin() + 2, frame.payload.end());
            }
        }
        
        if (state == ConnectionState::OPEN) {
            // Клиент инициировал закрытие, отправляем close в ответ
            auto close_frame = WebSocketFrame::create_close(code, reason, false);
            send(socket_fd, close_frame.data(), close_frame.size(), 0);
        }
        
        state = ConnectionState::CLOSED;
        if (on_close_callback) on_close_callback(code, reason);
        
        ::close(socket_fd);
    }
};

// ============================================
// 📌 WebSocket Server
// ============================================

class WebSocketServer {
private:
    std::unordered_map<std::string, std::shared_ptr<WebSocketConnection>> connections;
    std::unordered_map<std::string, std::vector<std::string>> rooms; // room -> connection_ids
    std::mutex connections_mutex;
    
public:
    // Добавление нового подключения
    void add_connection(std::shared_ptr<WebSocketConnection> conn) {
        std::lock_guard lock(connections_mutex);
        connections[conn->get_id()] = conn;
        
        // Callback на сообщения
        conn->on_message([this, conn](const std::string& msg) {
            handle_message(conn->get_id(), msg);
        });
        
        conn->on_close([this, conn](uint16_t code, const std::string& reason) {
            remove_connection(conn->get_id());
        });
    }
    
    // Broadcast всем подключенным
    void broadcast(const std::string& message) {
        std::lock_guard lock(connections_mutex);
        for (const auto& [id, conn] : connections) {
            conn->send_text(message);
        }
    }
    
    // Отправка конкретному подключению
    void send_to(const std::string& conn_id, const std::string& message) {
        std::lock_guard lock(connections_mutex);
        auto it = connections.find(conn_id);
        if (it != connections.end()) {
            it->second->send_text(message);
        }
    }
    
    // Room management
    void join_room(const std::string& conn_id, const std::string& room) {
        std::lock_guard lock(connections_mutex);
        rooms[room].push_back(conn_id);
    }
    
    void leave_room(const std::string& conn_id, const std::string& room) {
        std::lock_guard lock(connections_mutex);
        auto& members = rooms[room];
        members.erase(std::remove(members.begin(), members.end(), conn_id), members.end());
    }
    
    // Broadcast в room
    void broadcast_to_room(const std::string& room, const std::string& message) {
        std::lock_guard lock(connections_mutex);
        auto it = rooms.find(room);
        if (it != rooms.end()) {
            for (const auto& conn_id : it->second) {
                auto conn_it = connections.find(conn_id);
                if (conn_it != connections.end()) {
                    conn_it->second->send_text(message);
                }
            }
        }
    }
    
private:
    void remove_connection(const std::string& conn_id) {
        std::lock_guard lock(connections_mutex);
        connections.erase(conn_id);
        
        // Удаление из всех rooms
        for (auto& [room, members] : rooms) {
            members.erase(std::remove(members.begin(), members.end(), conn_id), members.end());
        }
    }
    
    void handle_message(const std::string& conn_id, const std::string& message) {
        // Обработка входящих сообщений
        // Например, парсинг JSON команд:
        // {"type": "join_room", "room": "chat1"}
        // {"type": "message", "room": "chat1", "text": "Hello"}
    }
};

// ============================================
// 📌 WebSocket Client
// ============================================

class WebSocketClient {
private:
    int socket_fd = -1;
    ConnectionState state = ConnectionState::CLOSED;
    std::string url;
    bool auto_reconnect = true;
    std::chrono::seconds ping_interval{30};
    
    std::function<void(const std::string&)> on_message_callback;
    std::function<void()> on_open_callback;
    std::function<void(uint16_t, const std::string&)> on_close_callback;
    
public:
    WebSocketClient(const std::string& ws_url) : url(ws_url) {}
    
    // Асинхронное подключение
    void connect() {
        // Парсинг URL: ws://example.com:8080/path
        auto [host, port, path] = parse_websocket_url(url);
        
        // Создание TCP сокета
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        
        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);
        
        if (::connect(socket_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            handle_error("Connection failed");
            return;
        }
        
        // WebSocket handshake
        auto key = WebSocketHandshake::generate_client_key();
        auto handshake = WebSocketHandshake::create_client_handshake(host, path, key);
        send(socket_fd, handshake.c_str(), handshake.size(), 0);
        
        // Чтение ответа
        char buffer[1024];
        recv(socket_fd, buffer, sizeof(buffer), 0);
        
        // Проверка Sec-WebSocket-Accept
        // ...
        
        state = ConnectionState::OPEN;
        if (on_open_callback) on_open_callback();
        
        // Запуск event loop
        std::thread([this]() { event_loop(); }).detach();
        
        // Запуск ping timer
        std::thread([this]() { ping_loop(); }).detach();
    }
    
    // Отправка сообщения
    void send(const std::string& message) {
        if (state != ConnectionState::OPEN) return;
        
        auto frame = WebSocketFrame::create_text(message, true); // Клиент маскирует
        ::send(socket_fd, frame.data(), frame.size(), 0);
    }
    
    // Установка callbacks
    void on_message(std::function<void(const std::string&)> cb) {
        on_message_callback = cb;
    }
    
    void on_open(std::function<void()> cb) {
        on_open_callback = cb;
    }
    
    void on_close(std::function<void(uint16_t, const std::string&)> cb) {
        on_close_callback = cb;
    }
    
private:
    void event_loop() {
        uint8_t buffer[4096];
        
        while (state == ConnectionState::OPEN) {
            ssize_t bytes = recv(socket_fd, buffer, sizeof(buffer), 0);
            if (bytes <= 0) {
                // Соединение закрыто
                if (auto_reconnect) {
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    connect();
                }
                break;
            }
            
            // Парсинг фреймов
            size_t offset = 0;
            while (offset < bytes) {
                size_t consumed = 0;
                auto frame = WebSocketFrame::parse(buffer + offset, bytes - offset, consumed);
                
                if (!frame) break;
                
                handle_frame(*frame);
                offset += consumed;
            }
        }
    }
    
    void ping_loop() {
        while (state == ConnectionState::OPEN) {
            std::this_thread::sleep_for(ping_interval);
            
            std::vector<uint8_t> empty;
            auto frame = WebSocketFrame::create(WebSocketFrame::PING, empty, true, true);
            ::send(socket_fd, frame.data(), frame.size(), 0);
        }
    }
    
    void handle_frame(const WebSocketFrame& frame) {
        if (frame.opcode == WebSocketFrame::TEXT) {
            std::string msg(frame.payload.begin(), frame.payload.end());
            if (on_message_callback) on_message_callback(msg);
        }
        // ... остальные opcodes
    }
    
    void handle_error(const std::string& error) {
        std::cerr << "WebSocket error: " << error << std::endl;
    }
    
    std::tuple<std::string, int, std::string> parse_websocket_url(const std::string& url) {
        // ws://example.com:8080/path -> ("example.com", 8080, "/path")
        // Упрощенный парсинг
        return {"localhost", 8080, "/"};
    }
};

// ============================================
// 📌 Real-time Patterns
// ============================================

// --- Pub/Sub System ---
class PubSubWebSocket {
private:
    WebSocketServer server;
    std::unordered_map<std::string, std::vector<std::string>> subscriptions; // topic -> conn_ids
    std::mutex mutex;
    
public:
    // Подписка на топик
    void subscribe(const std::string& conn_id, const std::string& topic) {
        std::lock_guard lock(mutex);
        subscriptions[topic].push_back(conn_id);
    }
    
    // Публикация в топик
    void publish(const std::string& topic, const std::string& message) {
        std::lock_guard lock(mutex);
        auto it = subscriptions.find(topic);
        if (it != subscriptions.end()) {
            for (const auto& conn_id : it->second) {
                server.send_to(conn_id, message);
            }
        }
    }
    
    // Wildcard подписки: "chat.*" -> "chat.room1", "chat.room2"
    void subscribe_wildcard(const std::string& conn_id, const std::string& pattern) {
        // Реализация с regex matching
    }
};

// --- Chat Application Example ---
struct ChatMessage {
    std::string from_user;
    std::string to_room;
    std::string text;
    std::chrono::system_clock::time_point timestamp;
    
    std::string to_json() const {
        return "{\"from\":\"" + from_user + 
               "\",\"room\":\"" + to_room + 
               "\",\"text\":\"" + text + "\"}";
    }
};

class ChatServer {
private:
    WebSocketServer ws_server;
    std::unordered_map<std::string, std::string> users; // conn_id -> username
    
public:
    void handle_join(const std::string& conn_id, const std::string& username, 
                    const std::string& room) {
        users[conn_id] = username;
        ws_server.join_room(conn_id, room);
        
        // Уведомление о присоединении
        std::string join_msg = "{\"type\":\"user_joined\",\"user\":\"" + username + "\"}";
        ws_server.broadcast_to_room(room, join_msg);
    }
    
    void handle_message(const std::string& conn_id, const ChatMessage& msg) {
        ws_server.broadcast_to_room(msg.to_room, msg.to_json());
    }
    
    void handle_typing(const std::string& conn_id, const std::string& room) {
        auto username = users[conn_id];
        std::string typing_msg = "{\"type\":\"typing\",\"user\":\"" + username + "\"}";
        ws_server.broadcast_to_room(room, typing_msg);
    }
};

// ============================================
// 📌 Scaling WebSockets
// ============================================
// • Load balancing (sticky sessions)
// • Horizontal scaling (Redis pub/sub)
// • Connection migration
// • State synchronization
// • Distributed broadcasting

// ============================================
// 📌 Security
// ============================================
// • Origin validation
// • Authentication (query params, headers)
// • Rate limiting
// • Message size limits
// • Timeout protection
// • DoS prevention

// ============================================
// 📌 Compression (permessage-deflate)
// ============================================
// • Extension negotiation
// • Compression parameters
// • Context takeover
// • Performance considerations

// ============================================
// 📌 Testing
// ============================================
// • Mock WebSocket server
// • Client testing
// • Load testing
// • Stress testing