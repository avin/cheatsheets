// ============================================
// 📌 TCP/IP Fundamentals
// ============================================
// • Сокеты (BSD sockets API)
// • IPv4 vs IPv6
// • Endpoint (address + port)
// • Network byte order (htons, htonl, ntohs, ntohl)
// • Socket types (SOCK_STREAM, SOCK_DGRAM)
// • Socket options (SO_REUSEADDR, TCP_NODELAY, etc.)

// ============================================
// 📌 TCP Sockets
// ============================================

// --- TCP Server ---
// • socket() - создание
// • bind() - привязка к адресу
// • listen() - режим прослушивания
// • accept() - принятие соединения
// • send() / recv() - отправка/получение
// • close() - закрытие
// • RAII wrapper для сокетов
// • Error handling

// --- TCP Client ---
// • socket() - создание
// • connect() - подключение к серверу
// • send() / recv() - обмен данными
// • Connection timeout
// • Reconnection logic
// • Keep-alive механизм

// --- Full Echo Server Example ---
// • Single-threaded blocking
// • Multi-threaded (thread per connection)
// • Thread pool approach

// ============================================
// 📌 UDP Sockets
// ============================================
// • sendto() / recvfrom()
// • UDP server example
// • UDP client example
// • Broadcast и multicast
// • Unreliable delivery handling
// • Message boundaries

// ============================================
// 📌 Socket Operations
// ============================================

// --- Blocking vs Non-blocking ---
// • fcntl() / ioctlsocket()
// • O_NONBLOCK flag
// • Handling EAGAIN/EWOULDBLOCK

// --- Select/Poll/Epoll ---
// • select() - cross-platform
// • poll() - Linux/Unix
// • epoll() - Linux only (edge-triggered vs level-triggered)
// • Event loops

// ============================================
// 📌 Modern C++ Socket Wrapper
// ============================================
// • RAII Socket class
// • Exception-safe design
// • Move semantics
// • unique_ptr для ownership
// • std::expected для error handling (C++23)

// ============================================
// 📌 Address Resolution
// ============================================
// • getaddrinfo() / freeaddrinfo()
// • getnameinfo()
// • DNS resolution
// • IPv4/IPv6 dual-stack
// • Error handling

// ============================================
// 📌 Network Byte Serialization
// ============================================
// • Endianness handling
// • Serialization primitives (int, float, etc.)
// • String serialization (length-prefixed)
// • Binary protocols
// • Text protocols

// ============================================
// 📌 Connection Management
// ============================================
// • Connection pooling
// • Connection timeout
// • Keep-alive
// • Graceful shutdown
// • Half-close (shutdown())
// • SO_LINGER option

// ============================================
// 📌 Error Handling Patterns
// ============================================
// • errno vs std::error_code
// • std::expected<T, NetworkError> (C++23)
// • Custom error categories
// • Exception vs error codes
// • Logging network errors