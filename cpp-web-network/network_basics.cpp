// ============================================
// 📌 TCP/IP Fundamentals
// ============================================

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <expected>
#include <system_error>
#include <vector>
#include <string_view>
#include <thread>
#include <mutex>
#include <queue>

// Основы сокетов (BSD sockets API)
// Сокет - это конечная точка для отправки или получения данных по сети

void tcp_ip_basics() {
    // IPv4 адрес: 192.168.1.1:8080
    sockaddr_in addr_v4{};
    addr_v4.sin_family = AF_INET;              // IPv4
    addr_v4.sin_port = htons(8080);            // Порт в network byte order
    inet_pton(AF_INET, "192.168.1.1", &addr_v4.sin_addr);
    
    // IPv6 адрес: [::1]:8080 (loopback)
    sockaddr_in6 addr_v6{};
    addr_v6.sin6_family = AF_INET6;            // IPv6
    addr_v6.sin6_port = htons(8080);
    inet_pton(AF_INET6, "::1", &addr_v6.sin6_addr);
    
    // Network byte order (Big Endian)
    // htons - host to network short (для портов)
    // htonl - host to network long (для адресов)
    // ntohs - network to host short
    // ntohl - network to host long
    
    uint16_t port = 8080;
    uint16_t net_port = htons(port);  // Конвертация в network byte order
    
    // Socket types:
    // SOCK_STREAM  - TCP (надёжный, упорядоченный поток байтов)
    // SOCK_DGRAM   - UDP (ненадёжные датаграммы)
    // SOCK_RAW     - RAW (прямой доступ к IP)
}

// Socket Options - настройка поведения сокета
void socket_options_example(int sockfd) {
    // SO_REUSEADDR - позволяет переиспользовать локальный адрес
    // Полезно при перезапуске сервера (избегаем "Address already in use")
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // SO_REUSEPORT - позволяет нескольким сокетам слушать один порт
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    
    // TCP_NODELAY - отключает алгоритм Nagle (уменьшает латентность)
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
    
    // SO_KEEPALIVE - автоматически проверяет живо ли соединение
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
    
    // SO_RCVBUF / SO_SNDBUF - размер буферов приёма/отправки
    int bufsize = 65536;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
    
    // SO_RCVTIMEO / SO_SNDTIMEO - таймауты на операции
    timeval timeout{};
    timeout.tv_sec = 5;   // 5 секунд
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}

// ============================================
// 📌 TCP Sockets - Server
// ============================================

// Простейший TCP сервер (blocking, single-threaded)
void tcp_server_basic() {
    // 1. Создание сокета
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "socket() failed: " << strerror(errno) << '\n';
        return;
    }
    
    // 2. Настройка опций
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // 3. Привязка к адресу
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  // Слушаем на всех интерфейсах
    addr.sin_port = htons(8080);
    
    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind() failed: " << strerror(errno) << '\n';
        close(server_fd);
        return;
    }
    
    // 4. Переход в режим прослушивания
    // backlog = 10 - размер очереди ожидающих соединений
    if (listen(server_fd, 10) < 0) {
        std::cerr << "listen() failed: " << strerror(errno) << '\n';
        close(server_fd);
        return;
    }
    
    std::cout << "Server listening on port 8080...\n";
    
    // 5. Принятие соединений
    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            std::cerr << "accept() failed: " << strerror(errno) << '\n';
            continue;
        }
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        std::cout << "Client connected: " << client_ip << '\n';
        
        // 6. Обмен данными
        char buffer[1024];
        ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            std::cout << "Received: " << buffer << '\n';
            
            // Echo обратно
            send(client_fd, buffer, bytes_read, 0);
        }
        
        // 7. Закрытие соединения
        close(client_fd);
    }
    
    close(server_fd);
}

// ============================================
// 📌 TCP Sockets - Client
// ============================================

// Простой TCP клиент
void tcp_client_basic() {
    // 1. Создание сокета
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "socket() failed: " << strerror(errno) << '\n';
        return;
    }
    
    // 2. Подключение к серверу
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    
    if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "connect() failed: " << strerror(errno) << '\n';
        close(sockfd);
        return;
    }
    
    std::cout << "Connected to server\n";
    
    // 3. Отправка данных
    const char* message = "Hello, Server!";
    send(sockfd, message, strlen(message), 0);
    
    // 4. Получение ответа
    char buffer[1024];
    ssize_t bytes_read = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::cout << "Received: " << buffer << '\n';
    }
    
    // 5. Закрытие
    close(sockfd);
}

// TCP клиент с таймаутом подключения
bool tcp_client_with_timeout(const char* host, uint16_t port, int timeout_sec) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;
    
    // Переключаем в non-blocking режим
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr);
    
    // Неблокирующее подключение
    connect(sockfd, (sockaddr*)&addr, sizeof(addr));
    
    // Ждём завершения с помощью select
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(sockfd, &write_fds);
    
    timeval tv{};
    tv.tv_sec = timeout_sec;
    tv.tv_usec = 0;
    
    int result = select(sockfd + 1, nullptr, &write_fds, nullptr, &tv);
    
    // Возвращаемся в blocking режим
    fcntl(sockfd, F_SETFL, flags);
    
    if (result <= 0) {
        close(sockfd);
        return false;  // Таймаут или ошибка
    }
    
    // Проверяем, успешно ли подключились
    int error;
    socklen_t len = sizeof(error);
    getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
    
    if (error != 0) {
        close(sockfd);
        return false;
    }
    
    std::cout << "Connected successfully!\n";
    close(sockfd);
    return true;
}

// TCP клиент с логикой переподключения
class ReconnectingClient {
    int sockfd_ = -1;
    std::string host_;
    uint16_t port_;
    int max_retries_;
    
public:
    ReconnectingClient(std::string host, uint16_t port, int max_retries = 3)
        : host_(std::move(host)), port_(port), max_retries_(max_retries) {}
    
    ~ReconnectingClient() {
        disconnect();
    }
    
    bool connect_with_retry() {
        for (int attempt = 0; attempt < max_retries_; ++attempt) {
            sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd_ < 0) continue;
            
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port_);
            inet_pton(AF_INET, host_.c_str(), &addr.sin_addr);
            
            if (connect(sockfd_, (sockaddr*)&addr, sizeof(addr)) == 0) {
                std::cout << "Connected on attempt " << (attempt + 1) << '\n';
                return true;
            }
            
            close(sockfd_);
            sockfd_ = -1;
            
            // Экспоненциальная задержка: 1s, 2s, 4s...
            std::this_thread::sleep_for(std::chrono::seconds(1 << attempt));
        }
        
        return false;
    }
    
    void disconnect() {
        if (sockfd_ >= 0) {
            close(sockfd_);
            sockfd_ = -1;
        }
    }
    
    bool send_data(std::string_view data) {
        if (sockfd_ < 0) return false;
        return send(sockfd_, data.data(), data.size(), 0) >= 0;
    }
};

// TCP клиент с Keep-Alive
void tcp_client_keepalive() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Включаем TCP Keep-Alive
    int enable = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
    
    // Настройка параметров keep-alive (Linux)
    int idle = 60;      // Начинаем проверку через 60 секунд простоя
    int interval = 10;  // Интервал между проверками 10 секунд
    int count = 5;      // Количество попыток
    
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count));
    
    // Теперь сокет будет автоматически проверять живо ли соединение
    close(sockfd);
}

// ============================================
// 📌 Full Echo Server Examples
// ============================================

// Single-threaded blocking echo server
void echo_server_single_threaded() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);
    
    std::cout << "Echo server listening on port 8080\n";
    
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) continue;
        
        char buffer[4096];
        while (true) {
            ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
            if (n <= 0) break;  // Клиент отключился или ошибка
            
            // Отправляем обратно всё что получили
            send(client_fd, buffer, n, 0);
        }
        
        close(client_fd);
    }
    
    close(server_fd);
}

// Multi-threaded echo server (thread per connection)
void handle_client(int client_fd) {
    char buffer[4096];
    while (true) {
        ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
        if (n <= 0) break;
        send(client_fd, buffer, n, 0);
    }
    close(client_fd);
}

void echo_server_multi_threaded() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);
    
    std::cout << "Multi-threaded echo server on port 8080\n";
    
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) continue;
        
        // Создаём отдельный поток для каждого клиента
        std::thread(handle_client, client_fd).detach();
    }
    
    close(server_fd);
}

// Thread pool для обработки клиентов
class ThreadPool {
    std::vector<std::thread> workers_;
    std::queue<int> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_ = false;
    
public:
    explicit ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    int client_fd;
                    {
                        std::unique_lock lock(mutex_);
                        cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                        
                        if (stop_ && tasks_.empty()) return;
                        
                        client_fd = tasks_.front();
                        tasks_.pop();
                    }
                    
                    handle_client(client_fd);
                }
            });
        }
    }
    
    ~ThreadPool() {
        {
            std::unique_lock lock(mutex_);
            stop_ = true;
        }
        cv_.notify_all();
        
        for (auto& worker : workers_) {
            if (worker.joinable()) worker.join();
        }
    }
    
    void submit(int client_fd) {
        {
            std::unique_lock lock(mutex_);
            tasks_.push(client_fd);
        }
        cv_.notify_one();
    }
};

void echo_server_thread_pool() {
    ThreadPool pool(4);  // 4 рабочих потока
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);
    
    std::cout << "Thread pool echo server on port 8080\n";
    
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd >= 0) {
            pool.submit(client_fd);
        }
    }
    
    close(server_fd);
}

// ============================================
// 📌 UDP Sockets
// ============================================

// UDP сервер
void udp_server_example() {
    // 1. Создание UDP сокета
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "socket() failed\n";
        return;
    }
    
    // 2. Привязка к адресу
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    
    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind() failed\n";
        close(sockfd);
        return;
    }
    
    std::cout << "UDP server listening on port 8080\n";
    
    // 3. Приём данных
    char buffer[65536];  // Максимальный размер UDP датаграммы
    
    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        
        // recvfrom - получаем данные и адрес отправителя
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                             (sockaddr*)&client_addr, &client_len);
        
        if (n > 0) {
            buffer[n] = '\0';
            
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
            
            std::cout << "Received from " << client_ip << ": " << buffer << '\n';
            
            // Отправляем ответ
            sendto(sockfd, buffer, n, 0, (sockaddr*)&client_addr, client_len);
        }
    }
    
    close(sockfd);
}

// UDP клиент
void udp_client_example() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    
    const char* message = "Hello UDP!";
    sendto(sockfd, message, strlen(message), 0,
           (sockaddr*)&server_addr, sizeof(server_addr));
    
    // Получаем ответ
    char buffer[1024];
    sockaddr_in from_addr{};
    socklen_t from_len = sizeof(from_addr);
    
    ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                         (sockaddr*)&from_addr, &from_len);
    
    if (n > 0) {
        buffer[n] = '\0';
        std::cout << "Received: " << buffer << '\n';
    }
    
    close(sockfd);
}

// UDP Broadcast - отправка всем в локальной сети
void udp_broadcast_example() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Включаем broadcast
    int broadcast_enable = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, 
               &broadcast_enable, sizeof(broadcast_enable));
    
    sockaddr_in broadcast_addr{};
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(8080);
    broadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;  // 255.255.255.255
    
    const char* message = "Broadcast message!";
    sendto(sockfd, message, strlen(message), 0,
           (sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
    
    close(sockfd);
}

// UDP Multicast - отправка группе подписчиков
void udp_multicast_sender() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    sockaddr_in multicast_addr{};
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "239.0.0.1", &multicast_addr.sin_addr);  // Multicast группа
    
    const char* message = "Multicast message!";
    sendto(sockfd, message, strlen(message), 0,
           (sockaddr*)&multicast_addr, sizeof(multicast_addr));
    
    close(sockfd);
}

void udp_multicast_receiver() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    
    bind(sockfd, (sockaddr*)&addr, sizeof(addr));
    
    // Присоединяемся к multicast группе
    ip_mreq mreq{};
    inet_pton(AF_INET, "239.0.0.1", &mreq.imr_multiaddr);
    mreq.imr_interface.s_addr = INADDR_ANY;
    
    setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    
    std::cout << "Listening for multicast messages...\n";
    
    char buffer[1024];
    while (true) {
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0, nullptr, nullptr);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "Multicast message: " << buffer << '\n';
        }
    }
    
    close(sockfd);
}

// Обработка ненадёжной доставки в UDP
class ReliableUdpSender {
    int sockfd_;
    sockaddr_in dest_addr_;
    
public:
    // Отправка с подтверждением (simple ARQ)
    bool send_with_ack(const char* data, size_t len, int max_retries = 3) {
        for (int attempt = 0; attempt < max_retries; ++attempt) {
            // Отправляем данные
            sendto(sockfd_, data, len, 0, (sockaddr*)&dest_addr_, sizeof(dest_addr_));
            
            // Ждём ACK с таймаутом
            timeval tv{};
            tv.tv_sec = 1;  // 1 секунда таймаут
            tv.tv_usec = 0;
            setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            
            char ack[4];
            ssize_t n = recvfrom(sockfd_, ack, sizeof(ack), 0, nullptr, nullptr);
            
            if (n > 0 && std::string_view(ack, n) == "ACK") {
                return true;  // Успешно получили подтверждение
            }
            
            std::cout << "No ACK, retrying... (attempt " << (attempt + 1) << ")\n";
        }
        
        return false;  // Не удалось доставить
    }
};

// ============================================
// 📌 Socket Operations
// ============================================

// Блокирующий vs неблокирующий режим
void blocking_vs_nonblocking() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Получаем текущие флаги
    int flags = fcntl(sockfd, F_GETFL, 0);
    
    // Переключаем в неблокирующий режим
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    
    // Теперь recv() будет немедленно возвращаться с EAGAIN если данных нет
    char buffer[1024];
    ssize_t n = recv(sockfd, buffer, sizeof(buffer), 0);
    
    if (n < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            std::cout << "No data available (non-blocking)\n";
        } else {
            std::cerr << "recv() error\n";
        }
    }
    
    // Возвращаемся в блокирующий режим
    fcntl(sockfd, F_SETFL, flags);
    
    close(sockfd);
}

// select() - кросс-платформенная мультиплексирование I/O
void select_example() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // ... bind, listen ...
    
    fd_set read_fds;
    int max_fd = server_fd;
    
    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        
        // Таймаут 5 секунд
        timeval timeout{};
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, &timeout);
        
        if (activity < 0) {
            std::cerr << "select() error\n";
            break;
        }
        
        if (activity == 0) {
            std::cout << "Timeout\n";
            continue;
        }
        
        // Проверяем, есть ли активность на server_fd
        if (FD_ISSET(server_fd, &read_fds)) {
            int client_fd = accept(server_fd, nullptr, nullptr);
            std::cout << "New connection\n";
            // Обработка клиента...
            close(client_fd);
        }
    }
    
    close(server_fd);
}

// poll() - улучшенная версия select()
#include <poll.h>

void poll_example() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // ... bind, listen ...
    
    std::vector<pollfd> poll_fds;
    poll_fds.push_back({server_fd, POLLIN, 0});
    
    while (true) {
        int activity = poll(poll_fds.data(), poll_fds.size(), 5000);  // 5 сек таймаут
        
        if (activity < 0) {
            std::cerr << "poll() error\n";
            break;
        }
        
        if (activity == 0) {
            std::cout << "Timeout\n";
            continue;
        }
        
        for (auto& pfd : poll_fds) {
            if (pfd.revents & POLLIN) {
                if (pfd.fd == server_fd) {
                    // Новое подключение
                    int client_fd = accept(server_fd, nullptr, nullptr);
                    poll_fds.push_back({client_fd, POLLIN, 0});
                } else {
                    // Данные от клиента
                    char buffer[1024];
                    ssize_t n = recv(pfd.fd, buffer, sizeof(buffer), 0);
                    if (n <= 0) {
                        close(pfd.fd);
                        pfd.fd = -1;  // Помечаем для удаления
                    }
                }
            }
        }
        
        // Удаляем закрытые соединения
        poll_fds.erase(
            std::remove_if(poll_fds.begin(), poll_fds.end(),
                          [](const pollfd& pfd) { return pfd.fd == -1; }),
            poll_fds.end()
        );
    }
    
    close(server_fd);
}

// epoll() - эффективный механизм для Linux
#include <sys/epoll.h>

void epoll_example() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // ... bind, listen ...
    
    // Создаём epoll instance
    int epoll_fd = epoll_create1(0);
    
    // Добавляем server_fd в epoll
    epoll_event ev{};
    ev.events = EPOLLIN;  // Интересуемся событиями чтения
    ev.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
    
    const int MAX_EVENTS = 10;
    epoll_event events[MAX_EVENTS];
    
    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, 5000);  // 5 сек таймаут
        
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_fd) {
                // Новое подключение
                int client_fd = accept(server_fd, nullptr, nullptr);
                
                // Добавляем клиента в epoll
                ev.events = EPOLLIN | EPOLLET;  // Edge-triggered mode
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
            } else {
                // Данные от клиента
                int client_fd = events[i].data.fd;
                char buffer[1024];
                ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
                
                if (n <= 0) {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
                    close(client_fd);
                }
            }
        }
    }
    
    close(epoll_fd);
    close(server_fd);
}

// Edge-triggered vs Level-triggered в epoll
// 
// Level-triggered (по умолчанию):
// - epoll_wait() возвращает событие, пока условие истинно
// - Если данные доступны, epoll_wait() будет возвращать событие
//   даже если вы не прочитали все данные
// 
// Edge-triggered (EPOLLET):
// - epoll_wait() возвращает событие только при изменении состояния
// - Вы должны читать до EAGAIN, иначе можете пропустить данные
// - Более эффективный, но требует осторожности

void epoll_edge_triggered_example(int client_fd) {
    // Переключаем в неблокирующий режим (обязательно для edge-triggered)
    int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
    
    char buffer[1024];
    
    // Читаем ВСЕ доступные данные
    while (true) {
        ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
        
        if (n > 0) {
            // Обработка данных...
        } else if (n == 0) {
            // Соединение закрыто
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Все данные прочитаны
                break;
            } else {
                // Реальная ошибка
                break;
            }
        }
    }
}

// ============================================
// 📌 Modern C++ Socket Wrapper
// ============================================

// Категория ошибок для сети
enum class NetworkError {
    Success = 0,
    ConnectionRefused,
    Timeout,
    Disconnected,
    InvalidAddress,
    Unknown
};

// Конвертация errno в NetworkError
NetworkError errno_to_network_error() {
    switch (errno) {
        case ECONNREFUSED: return NetworkError::ConnectionRefused;
        case ETIMEDOUT: return NetworkError::Timeout;
        case ENOTCONN: return NetworkError::Disconnected;
        case EINVAL: return NetworkError::InvalidAddress;
        default: return NetworkError::Unknown;
    }
}

// RAII Socket wrapper с современным C++23
class Socket {
    int fd_ = -1;
    
public:
    Socket() = default;
    
    // Создание сокета
    static std::expected<Socket, NetworkError> create(int domain, int type) {
        Socket sock;
        sock.fd_ = socket(domain, type, 0);
        
        if (sock.fd_ < 0) {
            return std::unexpected(errno_to_network_error());
        }
        
        return sock;
    }
    
    // Запрещаем копирование
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    
    // Разрешаем перемещение
    Socket(Socket&& other) noexcept : fd_(other.fd_) {
        other.fd_ = -1;
    }
    
    Socket& operator=(Socket&& other) noexcept {
        if (this != &other) {
            close();
            fd_ = other.fd_;
            other.fd_ = -1;
        }
        return *this;
    }
    
    ~Socket() {
        close();
    }
    
    void close() {
        if (fd_ >= 0) {
            ::close(fd_);
            fd_ = -1;
        }
    }
    
    // Получение файлового дескриптора
    int fd() const { return fd_; }
    bool is_valid() const { return fd_ >= 0; }
    
    // Bind
    std::expected<void, NetworkError> bind(const sockaddr* addr, socklen_t len) {
        if (::bind(fd_, addr, len) < 0) {
            return std::unexpected(errno_to_network_error());
        }
        return {};
    }
    
    // Listen
    std::expected<void, NetworkError> listen(int backlog = 10) {
        if (::listen(fd_, backlog) < 0) {
            return std::unexpected(errno_to_network_error());
        }
        return {};
    }
    
    // Connect
    std::expected<void, NetworkError> connect(const sockaddr* addr, socklen_t len) {
        if (::connect(fd_, addr, len) < 0) {
            return std::unexpected(errno_to_network_error());
        }
        return {};
    }
    
    // Accept
    std::expected<Socket, NetworkError> accept() {
        Socket client;
        client.fd_ = ::accept(fd_, nullptr, nullptr);
        
        if (client.fd_ < 0) {
            return std::unexpected(errno_to_network_error());
        }
        
        return client;
    }
    
    // Send
    std::expected<size_t, NetworkError> send(std::string_view data) {
        ssize_t sent = ::send(fd_, data.data(), data.size(), 0);
        
        if (sent < 0) {
            return std::unexpected(errno_to_network_error());
        }
        
        return static_cast<size_t>(sent);
    }
    
    // Receive
    std::expected<std::string, NetworkError> recv(size_t max_size = 4096) {
        std::string buffer;
        buffer.resize(max_size);
        
        ssize_t received = ::recv(fd_, buffer.data(), max_size, 0);
        
        if (received < 0) {
            return std::unexpected(errno_to_network_error());
        }
        
        if (received == 0) {
            return std::unexpected(NetworkError::Disconnected);
        }
        
        buffer.resize(received);
        return buffer;
    }
    
    // Set socket option
    template<typename T>
    void set_option(int level, int optname, const T& value) {
        setsockopt(fd_, level, optname, &value, sizeof(T));
    }
};

// Использование Socket wrapper
void socket_wrapper_usage() {
    // Создание TCP сокета
    auto sock_result = Socket::create(AF_INET, SOCK_STREAM);
    
    if (!sock_result) {
        std::cerr << "Failed to create socket\n";
        return;
    }
    
    Socket sock = std::move(*sock_result);
    
    // Настройка опций
    int opt = 1;
    sock.set_option(SOL_SOCKET, SO_REUSEADDR, opt);
    
    // Привязка
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    
    if (auto result = sock.bind((sockaddr*)&addr, sizeof(addr)); !result) {
        std::cerr << "bind() failed\n";
        return;
    }
    
    // Listen
    if (auto result = sock.listen(); !result) {
        std::cerr << "listen() failed\n";
        return;
    }
    
    // Accept клиента
    auto client_result = sock.accept();
    
    if (!client_result) {
        std::cerr << "accept() failed\n";
        return;
    }
    
    Socket client = std::move(*client_result);
    
    // Получение данных
    auto data_result = client.recv();
    
    if (data_result) {
        std::cout << "Received: " << *data_result << '\n';
        
        // Отправка ответа
        client.send("Hello, Client!");
    }
    
    // Сокеты автоматически закроются при выходе из области видимости
}

// ============================================
// 📌 Address Resolution
// ============================================

// DNS resolution с getaddrinfo
void dns_resolution_example(const char* hostname, const char* service) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;      // IPv4 или IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // Для серверов
    
    addrinfo* result = nullptr;
    
    int status = getaddrinfo(hostname, service, &hints, &result);
    
    if (status != 0) {
        std::cerr << "getaddrinfo() error: " << gai_strerror(status) << '\n';
        return;
    }
    
    // Перебираем все найденные адреса
    for (addrinfo* addr = result; addr != nullptr; addr = addr->ai_next) {
        char ip_str[INET6_ADDRSTRLEN];
        
        if (addr->ai_family == AF_INET) {
            // IPv4
            sockaddr_in* ipv4 = (sockaddr_in*)addr->ai_addr;
            inet_ntop(AF_INET, &ipv4->sin_addr, ip_str, sizeof(ip_str));
            std::cout << "IPv4: " << ip_str << '\n';
        } else if (addr->ai_family == AF_INET6) {
            // IPv6
            sockaddr_in6* ipv6 = (sockaddr_in6*)addr->ai_addr;
            inet_ntop(AF_INET6, &ipv6->sin6_addr, ip_str, sizeof(ip_str));
            std::cout << "IPv6: " << ip_str << '\n';
        }
    }
    
    freeaddrinfo(result);  // Обязательно освобождаем память
}

// Reverse DNS - получение имени хоста по IP
void reverse_dns_example() {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr);
    
    char hostname[1024];
    char service[20];
    
    int status = getnameinfo((sockaddr*)&addr, sizeof(addr),
                            hostname, sizeof(hostname),
                            service, sizeof(service),
                            0);
    
    if (status == 0) {
        std::cout << "Hostname: " << hostname << '\n';
    }
}

// IPv4/IPv6 Dual-Stack сервер
Socket create_dual_stack_server(uint16_t port) {
    auto sock_result = Socket::create(AF_INET6, SOCK_STREAM);
    if (!sock_result) {
        throw std::runtime_error("Failed to create socket");
    }
    
    Socket sock = std::move(*sock_result);
    
    // Отключаем IPV6_V6ONLY - позволяет принимать IPv4 и IPv6
    int no = 0;
    sock.set_option(IPPROTO_IPV6, IPV6_V6ONLY, no);
    
    int opt = 1;
    sock.set_option(SOL_SOCKET, SO_REUSEADDR, opt);
    
    sockaddr_in6 addr{};
    addr.sin6_family = AF_INET6;
    addr.sin6_addr = in6addr_any;  // :: (все интерфейсы)
    addr.sin6_port = htons(port);
    
    sock.bind((sockaddr*)&addr, sizeof(addr));
    sock.listen();
    
    return sock;
}

// ============================================
// 📌 Network Byte Serialization
// ============================================

// Сериализация примитивов с учётом endianness
class BinarySerializer {
    std::vector<uint8_t> buffer_;
    
public:
    // Сериализация целых чисел
    void write_uint16(uint16_t value) {
        uint16_t net_value = htons(value);
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&net_value);
        buffer_.insert(buffer_.end(), bytes, bytes + sizeof(net_value));
    }
    
    void write_uint32(uint32_t value) {
        uint32_t net_value = htonl(value);
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&net_value);
        buffer_.insert(buffer_.end(), bytes, bytes + sizeof(net_value));
    }
    
    void write_uint64(uint64_t value) {
        // Нет стандартной функции для 64-бит, делаем вручную
        write_uint32(static_cast<uint32_t>(value >> 32));
        write_uint32(static_cast<uint32_t>(value & 0xFFFFFFFF));
    }
    
    // Сериализация строк (length-prefixed)
    void write_string(std::string_view str) {
        write_uint32(static_cast<uint32_t>(str.size()));
        buffer_.insert(buffer_.end(), str.begin(), str.end());
    }
    
    // Сериализация float/double
    void write_float(float value) {
        uint32_t int_value;
        std::memcpy(&int_value, &value, sizeof(float));
        write_uint32(int_value);
    }
    
    void write_double(double value) {
        uint64_t int_value;
        std::memcpy(&int_value, &value, sizeof(double));
        write_uint64(int_value);
    }
    
    const std::vector<uint8_t>& data() const { return buffer_; }
};

class BinaryDeserializer {
    const uint8_t* data_;
    size_t size_;
    size_t pos_ = 0;
    
public:
    BinaryDeserializer(const uint8_t* data, size_t size) 
        : data_(data), size_(size) {}
    
    uint16_t read_uint16() {
        if (pos_ + sizeof(uint16_t) > size_) throw std::runtime_error("Out of bounds");
        
        uint16_t net_value;
        std::memcpy(&net_value, data_ + pos_, sizeof(uint16_t));
        pos_ += sizeof(uint16_t);
        
        return ntohs(net_value);
    }
    
    uint32_t read_uint32() {
        if (pos_ + sizeof(uint32_t) > size_) throw std::runtime_error("Out of bounds");
        
        uint32_t net_value;
        std::memcpy(&net_value, data_ + pos_, sizeof(uint32_t));
        pos_ += sizeof(uint32_t);
        
        return ntohl(net_value);
    }
    
    uint64_t read_uint64() {
        uint64_t high = read_uint32();
        uint64_t low = read_uint32();
        return (high << 32) | low;
    }
    
    std::string read_string() {
        uint32_t len = read_uint32();
        if (pos_ + len > size_) throw std::runtime_error("Out of bounds");
        
        std::string result(reinterpret_cast<const char*>(data_ + pos_), len);
        pos_ += len;
        return result;
    }
    
    float read_float() {
        uint32_t int_value = read_uint32();
        float result;
        std::memcpy(&result, &int_value, sizeof(float));
        return result;
    }
    
    double read_double() {
        uint64_t int_value = read_uint64();
        double result;
        std::memcpy(&result, &int_value, sizeof(double));
        return result;
    }
};

// Пример использования сериализации
void serialization_example() {
    BinarySerializer ser;
    ser.write_uint32(42);
    ser.write_string("Hello, Network!");
    ser.write_float(3.14f);
    
    const auto& data = ser.data();
    
    // Отправка по сети
    // send(sockfd, data.data(), data.size(), 0);
    
    // Десериализация
    BinaryDeserializer deser(data.data(), data.size());
    
    uint32_t num = deser.read_uint32();
    std::string text = deser.read_string();
    float pi = deser.read_float();
    
    std::cout << "Num: " << num << ", Text: " << text << ", Pi: " << pi << '\n';
}

// Простой текстовый протокол (альтернатива бинарному)
void text_protocol_example(Socket& sock) {
    // Отправка команды
    std::string command = "GET /users/123\r\n";
    sock.send(command);
    
    // Получение построчно
    std::string response;
    std::string line;
    
    while (true) {
        auto data = sock.recv(1);  // Читаем по символу
        if (!data) break;
        
        char c = (*data)[0];
        
        if (c == '\n') {
            if (line == "\r") break;  // Пустая строка - конец
            response += line + '\n';
            line.clear();
        } else if (c != '\r') {
            line += c;
        }
    }
    
    std::cout << "Response:\n" << response << '\n';
}

// ============================================
// 📌 Connection Management
// ============================================

// Connection Pool для переиспользования соединений
class ConnectionPool {
    struct Connection {
        Socket socket;
        std::chrono::steady_clock::time_point last_used;
        bool in_use = false;
    };
    
    std::vector<Connection> connections_;
    std::mutex mutex_;
    std::string host_;
    uint16_t port_;
    size_t max_connections_;
    
public:
    ConnectionPool(std::string host, uint16_t port, size_t max_conn = 10)
        : host_(std::move(host)), port_(port), max_connections_(max_conn) {}
    
    // Получение соединения из пула
    std::unique_ptr<Socket> acquire() {
        std::lock_guard lock(mutex_);
        
        // Ищем свободное соединение
        for (auto& conn : connections_) {
            if (!conn.in_use && conn.socket.is_valid()) {
                conn.in_use = true;
                conn.last_used = std::chrono::steady_clock::now();
                
                // Возвращаем временное владение
                return std::make_unique<Socket>(std::move(conn.socket));
            }
        }
        
        // Создаём новое соединение если есть место
        if (connections_.size() < max_connections_) {
            auto sock_result = Socket::create(AF_INET, SOCK_STREAM);
            if (!sock_result) return nullptr;
            
            Socket sock = std::move(*sock_result);
            
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port_);
            inet_pton(AF_INET, host_.c_str(), &addr.sin_addr);
            
            if (auto result = sock.connect((sockaddr*)&addr, sizeof(addr)); !result) {
                return nullptr;
            }
            
            connections_.push_back({std::move(sock), 
                                   std::chrono::steady_clock::now(), 
                                   true});
            
            return std::make_unique<Socket>(std::move(connections_.back().socket));
        }
        
        return nullptr;  // Пул исчерпан
    }
    
    // Возврат соединения в пул
    void release(std::unique_ptr<Socket> sock) {
        std::lock_guard lock(mutex_);
        
        // Находим соответствующее соединение и помечаем как свободное
        for (auto& conn : connections_) {
            if (!conn.socket.is_valid()) {
                conn.socket = std::move(*sock);
                conn.in_use = false;
                return;
            }
        }
    }
    
    // Очистка старых соединений
    void cleanup(std::chrono::seconds max_idle = std::chrono::seconds(60)) {
        std::lock_guard lock(mutex_);
        auto now = std::chrono::steady_clock::now();
        
        for (auto& conn : connections_) {
            if (!conn.in_use) {
                auto idle_time = std::chrono::duration_cast<std::chrono::seconds>(
                    now - conn.last_used);
                
                if (idle_time > max_idle) {
                    conn.socket.close();
                }
            }
        }
    }
};

// Graceful Shutdown - корректное завершение соединения
void graceful_shutdown(int sockfd) {
    // Шаг 1: Останавливаем отправку данных (half-close)
    shutdown(sockfd, SHUT_WR);  // SHUT_WR = больше не отправляем
    
    // Шаг 2: Продолжаем читать данные от клиента до EOF
    char buffer[1024];
    while (recv(sockfd, buffer, sizeof(buffer), 0) > 0) {
        // Читаем и игнорируем
    }
    
    // Шаг 3: Полное закрытие
    close(sockfd);
}

// SO_LINGER опция - контроль поведения при закрытии
void linger_option_example(int sockfd) {
    linger ling{};
    
    // Вариант 1: Жёсткое закрытие (RST вместо FIN)
    ling.l_onoff = 1;   // Включить linger
    ling.l_linger = 0;  // Таймаут 0 = немедленное закрытие
    setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
    
    // Вариант 2: Ждать отправки данных до 10 секунд
    ling.l_onoff = 1;
    ling.l_linger = 10;  // 10 секунд
    setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
    
    // Вариант 3: Поведение по умолчанию
    ling.l_onoff = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
}