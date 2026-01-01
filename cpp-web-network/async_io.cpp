// ============================================
// 📌 Asynchronous I/O Patterns
// ============================================

#include <functional>
#include <future>
#include <coroutine>
#include <chrono>
#include <thread>
#include <queue>
#include <vector>
#include <memory>
#include <optional>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

// ============================================
// 1. CALLBACKS - классический подход
// ============================================

using CompletionCallback = std::function<void(int error, size_t bytes_transferred)>;

class AsyncSocket {
    int fd_;
    
public:
    // Асинхронное чтение с callback
    void async_read(char* buffer, size_t size, CompletionCallback callback) {
        // Переключаем в неблокирующий режим
        int flags = fcntl(fd_, F_GETFL, 0);
        fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
        
        // В реальности здесь бы использовался event loop
        std::thread([this, buffer, size, callback = std::move(callback)]() {
            ssize_t n = recv(fd_, buffer, size, 0);
            
            if (n >= 0) {
                callback(0, n);  // Успех
            } else {
                callback(errno, 0);  // Ошибка
            }
        }).detach();
    }
    
    // Асинхронная запись
    void async_write(const char* data, size_t size, CompletionCallback callback) {
        std::thread([this, data, size, callback = std::move(callback)]() {
            ssize_t n = send(fd_, data, size, 0);
            
            if (n >= 0) {
                callback(0, n);
            } else {
                callback(errno, 0);
            }
        }).detach();
    }
};

// Пример использования callback-based API
void callback_example() {
    AsyncSocket sock;
    
    char buffer[1024];
    sock.async_read(buffer, sizeof(buffer), 
        [](int error, size_t bytes) {
            if (error == 0) {
                std::cout << "Read " << bytes << " bytes\n";
            } else {
                std::cerr << "Read error: " << error << '\n';
            }
        }
    );
    
    // Проблема callbacks: callback hell при цепочке операций
    sock.async_read(buffer, sizeof(buffer),
        [&sock, buffer](int error1, size_t bytes1) {
            if (error1 == 0) {
                sock.async_write(buffer, bytes1,
                    [&sock, buffer](int error2, size_t bytes2) {
                        if (error2 == 0) {
                            sock.async_read(buffer, sizeof(buffer),
                                [](int error3, size_t bytes3) {
                                    // Вложенность растёт...
                                }
                            );
                        }
                    }
                );
            }
        }
    );
}

// ============================================
// 2. FUTURES/PROMISES - упрощение callbacks
// ============================================

class FutureSocket {
    int fd_;
    
public:
    // Асинхронное чтение возвращает future
    std::future<size_t> async_read(char* buffer, size_t size) {
        auto promise = std::make_shared<std::promise<size_t>>();
        std::future<size_t> future = promise->get_future();
        
        std::thread([this, buffer, size, promise]() {
            ssize_t n = recv(fd_, buffer, size, 0);
            
            if (n >= 0) {
                promise->set_value(n);
            } else {
                promise->set_exception(
                    std::make_exception_ptr(std::runtime_error("recv failed"))
                );
            }
        }).detach();
        
        return future;
    }
    
    std::future<size_t> async_write(const char* data, size_t size) {
        auto promise = std::make_shared<std::promise<size_t>>();
        std::future<size_t> future = promise->get_future();
        
        std::thread([this, data, size, promise]() {
            ssize_t n = send(fd_, data, size, 0);
            
            if (n >= 0) {
                promise->set_value(n);
            } else {
                promise->set_exception(
                    std::make_exception_ptr(std::runtime_error("send failed"))
                );
            }
        }).detach();
        
        return future;
    }
};

// Использование futures - более читаемый код
void future_example() {
    FutureSocket sock;
    
    char buffer[1024];
    
    // Цепочка операций выглядит лучше
    auto read_future = sock.async_read(buffer, sizeof(buffer));
    
    try {
        size_t bytes_read = read_future.get();  // Ждём завершения
        std::cout << "Read " << bytes_read << " bytes\n";
        
        auto write_future = sock.async_write(buffer, bytes_read);
        size_t bytes_written = write_future.get();
        
        std::cout << "Written " << bytes_written << " bytes\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

// ============================================
// 3. COROUTINES (C++20) - лучший подход
// ============================================

// Task<T> - простейший awaitable type
template<typename T>
struct Task {
    struct promise_type {
        T value;
        std::exception_ptr exception;
        
        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        
        void return_value(T val) {
            value = std::move(val);
        }
        
        void unhandled_exception() {
            exception = std::current_exception();
        }
    };
    
    std::coroutine_handle<promise_type> handle;
    
    ~Task() {
        if (handle) handle.destroy();
    }
    
    T get() {
        if (handle.promise().exception) {
            std::rethrow_exception(handle.promise().exception);
        }
        return handle.promise().value;
    }
};

// Awaiter для асинхронного чтения
struct AsyncReadAwaiter {
    int fd;
    char* buffer;
    size_t size;
    ssize_t result = -1;
    
    bool await_ready() {
        // Проверяем, можем ли читать сразу
        return false;
    }
    
    void await_suspend(std::coroutine_handle<> handle) {
        // Запускаем асинхронную операцию
        std::thread([this, handle]() mutable {
            result = recv(fd, buffer, size, 0);
            handle.resume();  // Возобновляем корутину
        }).detach();
    }
    
    ssize_t await_resume() {
        return result;
    }
};

// Awaiter для асинхронной записи
struct AsyncWriteAwaiter {
    int fd;
    const char* data;
    size_t size;
    ssize_t result = -1;
    
    bool await_ready() { return false; }
    
    void await_suspend(std::coroutine_handle<> handle) {
        std::thread([this, handle]() mutable {
            result = send(fd, data, size, 0);
            handle.resume();
        }).detach();
    }
    
    ssize_t await_resume() {
        return result;
    }
};

// Coroutine-based socket wrapper
class CoroSocket {
    int fd_;
    
public:
    explicit CoroSocket(int fd) : fd_(fd) {}
    
    AsyncReadAwaiter async_read(char* buffer, size_t size) {
        return AsyncReadAwaiter{fd_, buffer, size};
    }
    
    AsyncWriteAwaiter async_write(const char* data, size_t size) {
        return AsyncWriteAwaiter{fd_, data, size};
    }
};

// Использование корутин - самый читаемый код!
Task<void> coroutine_example(CoroSocket& sock) {
    char buffer[1024];
    
    // Выглядит как синхронный код, работает асинхронно
    ssize_t bytes_read = co_await sock.async_read(buffer, sizeof(buffer));
    std::cout << "Read " << bytes_read << " bytes\n";
    
    ssize_t bytes_written = co_await sock.async_write(buffer, bytes_read);
    std::cout << "Written " << bytes_written << " bytes\n";
    
    co_return;
}

// ============================================
// 4. REACTOR PATTERN - event-driven architecture
// ============================================

// Reactor обрабатывает события I/O
class Reactor {
public:
    enum EventType {
        READ = 1,
        WRITE = 2
    };
    
    using EventHandler = std::function<void()>;
    
private:
    int epoll_fd_;
    bool running_ = false;
    
    struct HandlerInfo {
        EventHandler read_handler;
        EventHandler write_handler;
    };
    
    std::unordered_map<int, HandlerInfo> handlers_;
    
public:
    Reactor() {
        epoll_fd_ = epoll_create1(0);
    }
    
    ~Reactor() {
        close(epoll_fd_);
    }
    
    // Регистрация обработчика событий
    void register_handler(int fd, EventType events, EventHandler handler) {
        epoll_event ev{};
        ev.data.fd = fd;
        
        if (events & READ) {
            ev.events |= EPOLLIN;
            handlers_[fd].read_handler = handler;
        }
        
        if (events & WRITE) {
            ev.events |= EPOLLOUT;
            handlers_[fd].write_handler = handler;
        }
        
        epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
    }
    
    // Удаление обработчика
    void unregister_handler(int fd) {
        epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
        handlers_.erase(fd);
    }
    
    // Главный event loop
    void run() {
        running_ = true;
        const int MAX_EVENTS = 10;
        epoll_event events[MAX_EVENTS];
        
        while (running_) {
            int nfds = epoll_wait(epoll_fd_, events, MAX_EVENTS, 1000);
            
            for (int i = 0; i < nfds; ++i) {
                int fd = events[i].data.fd;
                auto it = handlers_.find(fd);
                
                if (it != handlers_.end()) {
                    if (events[i].events & EPOLLIN && it->second.read_handler) {
                        it->second.read_handler();
                    }
                    
                    if (events[i].events & EPOLLOUT && it->second.write_handler) {
                        it->second.write_handler();
                    }
                }
            }
        }
    }
    
    void stop() {
        running_ = false;
    }
};

// Использование Reactor
void reactor_example() {
    Reactor reactor;
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // ... bind, listen ...
    
    // Регистрируем обработчик для новых подключений
    reactor.register_handler(server_fd, Reactor::READ, [&reactor, server_fd]() {
        int client_fd = accept(server_fd, nullptr, nullptr);
        std::cout << "New client connected\n";
        
        // Регистрируем обработчик для чтения от клиента
        reactor.register_handler(client_fd, Reactor::READ, [client_fd, &reactor]() {
            char buffer[1024];
            ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
            
            if (n > 0) {
                std::cout << "Received data\n";
                send(client_fd, buffer, n, 0);  // Echo
            } else {
                reactor.unregister_handler(client_fd);
                close(client_fd);
            }
        });
    });
    
    reactor.run();
}

// ============================================
// 5. PROACTOR PATTERN - асинхронные операции
// ============================================

// Proactor запускает операции и вызывает handlers при завершении
class Proactor {
    struct AsyncOperation {
        std::function<void()> operation;
        std::function<void()> completion_handler;
    };
    
    std::queue<AsyncOperation> operations_;
    std::mutex mutex_;
    std::thread worker_;
    bool running_ = false;
    
public:
    Proactor() {
        running_ = true;
        worker_ = std::thread([this]() {
            while (running_) {
                std::unique_lock lock(mutex_);
                
                if (!operations_.empty()) {
                    auto op = std::move(operations_.front());
                    operations_.pop();
                    lock.unlock();
                    
                    // Выполняем операцию
                    op.operation();
                    
                    // Вызываем completion handler
                    op.completion_handler();
                } else {
                    lock.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
        });
    }
    
    ~Proactor() {
        running_ = false;
        if (worker_.joinable()) worker_.join();
    }
    
    void async_operation(std::function<void()> operation, 
                        std::function<void()> completion) {
        std::lock_guard lock(mutex_);
        operations_.push({std::move(operation), std::move(completion)});
    }
};

// ============================================
// 📌 Event Loop Implementation
// ============================================

// Полноценный Event Loop с таймерами и сигналами
class EventLoop {
    int epoll_fd_;
    bool running_ = false;
    
    struct Timer {
        std::chrono::steady_clock::time_point deadline;
        std::function<void()> callback;
        bool periodic;
        std::chrono::milliseconds interval;
    };
    
    std::vector<Timer> timers_;
    std::mutex timers_mutex_;
    
public:
    EventLoop() {
        epoll_fd_ = epoll_create1(0);
    }
    
    ~EventLoop() {
        close(epoll_fd_);
    }
    
    // Регистрация события
    void add_event(int fd, uint32_t events, std::function<void()> callback) {
        epoll_event ev{};
        ev.events = events;
        ev.data.fd = fd;
        
        epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
        // Сохраняем callback...
    }
    
    // Добавление таймера
    void add_timer(std::chrono::milliseconds delay, std::function<void()> callback,
                   bool periodic = false) {
        std::lock_guard lock(timers_mutex_);
        
        Timer timer;
        timer.deadline = std::chrono::steady_clock::now() + delay;
        timer.callback = std::move(callback);
        timer.periodic = periodic;
        timer.interval = delay;
        
        timers_.push_back(std::move(timer));
    }
    
    // Главный цикл
    void run() {
        running_ = true;
        const int MAX_EVENTS = 32;
        epoll_event events[MAX_EVENTS];
        
        while (running_) {
            // Вычисляем таймаут до ближайшего таймера
            int timeout = calculate_timeout();
            
            int nfds = epoll_wait(epoll_fd_, events, MAX_EVENTS, timeout);
            
            // Обработка событий I/O
            for (int i = 0; i < nfds; ++i) {
                // Вызов соответствующего callback...
            }
            
            // Обработка таймеров
            process_timers();
        }
    }
    
    void stop() {
        running_ = false;
    }
    
private:
    int calculate_timeout() {
        std::lock_guard lock(timers_mutex_);
        
        if (timers_.empty()) return 1000;  // 1 секунда по умолчанию
        
        auto now = std::chrono::steady_clock::now();
        auto earliest = std::min_element(timers_.begin(), timers_.end(),
            [](const Timer& a, const Timer& b) {
                return a.deadline < b.deadline;
            }
        );
        
        auto time_to_wait = earliest->deadline - now;
        
        if (time_to_wait <= std::chrono::milliseconds(0)) {
            return 0;  // Есть просроченные таймеры
        }
        
        return std::chrono::duration_cast<std::chrono::milliseconds>(time_to_wait).count();
    }
    
    void process_timers() {
        std::lock_guard lock(timers_mutex_);
        auto now = std::chrono::steady_clock::now();
        
        for (auto it = timers_.begin(); it != timers_.end(); ) {
            if (it->deadline <= now) {
                // Вызываем callback
                it->callback();
                
                if (it->periodic) {
                    // Перепланируем периодический таймер
                    it->deadline = now + it->interval;
                    ++it;
                } else {
                    // Удаляем одноразовый таймер
                    it = timers_.erase(it);
                }
            } else {
                ++it;
            }
        }
    }
};

// Использование EventLoop
void event_loop_example() {
    EventLoop loop;
    
    // Добавляем таймер
    loop.add_timer(std::chrono::seconds(5), []() {
        std::cout << "Timer fired after 5 seconds\n";
    });
    
    // Периодический таймер
    loop.add_timer(std::chrono::seconds(1), []() {
        std::cout << "Periodic timer (every 1 second)\n";
    }, true);
    
    // Запуск event loop
    loop.run();
}

// ============================================
// 📌 epoll-based Event Loop (подробно)
// ============================================

class EpollEventLoop {
    int epoll_fd_;
    bool running_ = false;
    
    struct EventHandler {
        int fd;
        std::function<void()> on_read;
        std::function<void()> on_write;
        std::function<void()> on_error;
    };
    
    std::unordered_map<int, EventHandler> handlers_;
    
public:
    EpollEventLoop() {
        epoll_fd_ = epoll_create1(0);
        if (epoll_fd_ < 0) {
            throw std::runtime_error("epoll_create1 failed");
        }
    }
    
    ~EpollEventLoop() {
        close(epoll_fd_);
    }
    
    // Регистрация с level-triggered mode
    void add_level_triggered(int fd, std::function<void()> on_read) {
        epoll_event ev{};
        ev.events = EPOLLIN;  // Level-triggered по умолчанию
        ev.data.fd = fd;
        
        epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
        
        handlers_[fd] = EventHandler{fd, std::move(on_read), nullptr, nullptr};
    }
    
    // Регистрация с edge-triggered mode
    void add_edge_triggered(int fd, std::function<void()> on_read) {
        // Переключаем сокет в non-blocking (обязательно для edge-triggered)
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        
        epoll_event ev{};
        ev.events = EPOLLIN | EPOLLET;  // Edge-triggered
        ev.data.fd = fd;
        
        epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
        
        handlers_[fd] = EventHandler{fd, std::move(on_read), nullptr, nullptr};
    }
    
    // One-shot event - срабатывает один раз, затем автоматически отключается
    void add_oneshot(int fd, std::function<void()> on_read) {
        epoll_event ev{};
        ev.events = EPOLLIN | EPOLLONESHOT;
        ev.data.fd = fd;
        
        epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
        
        handlers_[fd] = EventHandler{fd, std::move(on_read), nullptr, nullptr};
    }
    
    // Перевооружение one-shot события
    void rearm_oneshot(int fd) {
        epoll_event ev{};
        ev.events = EPOLLIN | EPOLLONESHOT;
        ev.data.fd = fd;
        
        epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev);
    }
    
    void run() {
        running_ = true;
        const int MAX_EVENTS = 64;
        epoll_event events[MAX_EVENTS];
        
        while (running_) {
            int nfds = epoll_wait(epoll_fd_, events, MAX_EVENTS, -1);
            
            for (int i = 0; i < nfds; ++i) {
                int fd = events[i].data.fd;
                auto it = handlers_.find(fd);
                
                if (it == handlers_.end()) continue;
                
                if (events[i].events & EPOLLIN) {
                    if (it->second.on_read) {
                        it->second.on_read();
                    }
                }
                
                if (events[i].events & EPOLLOUT) {
                    if (it->second.on_write) {
                        it->second.on_write();
                    }
                }
                
                if (events[i].events & (EPOLLERR | EPOLLHUP)) {
                    if (it->second.on_error) {
                        it->second.on_error();
                    }
                }
            }
        }
    }
    
    void stop() {
        running_ = false;
    }
};

// ============================================
// 📌 io_uring (Modern Linux)
// ============================================

#include <liburing.h>

class IoUringEventLoop {
    io_uring ring_;
    
public:
    IoUringEventLoop(unsigned entries = 256) {
        // Инициализация io_uring
        if (io_uring_queue_init(entries, &ring_, 0) < 0) {
            throw std::runtime_error("io_uring_queue_init failed");
        }
    }
    
    ~IoUringEventLoop() {
        io_uring_queue_exit(&ring_);
    }
    
    // Асинхронное чтение с io_uring
    void async_read(int fd, void* buffer, size_t size, uint64_t offset,
                    std::function<void(int result)> callback) {
        // Получаем submission queue entry
        io_uring_sqe* sqe = io_uring_get_sqe(&ring_);
        
        // Подготавливаем операцию чтения
        io_uring_prep_read(sqe, fd, buffer, size, offset);
        
        // Сохраняем callback в user_data
        io_uring_sqe_set_data(sqe, new std::function(callback));
        
        // Отправляем запрос
        io_uring_submit(&ring_);
    }
    
    // Асинхронная запись
    void async_write(int fd, const void* buffer, size_t size, uint64_t offset,
                     std::function<void(int result)> callback) {
        io_uring_sqe* sqe = io_uring_get_sqe(&ring_);
        io_uring_prep_write(sqe, fd, buffer, size, offset);
        io_uring_sqe_set_data(sqe, new std::function(callback));
        io_uring_submit(&ring_);
    }
    
    // Обработка завершённых операций
    void process_completions() {
        io_uring_cqe* cqe;
        unsigned head;
        unsigned count = 0;
        
        // Перебираем completion queue
        io_uring_for_each_cqe(&ring_, head, cqe) {
            // Получаем callback из user_data
            auto* callback = static_cast<std::function<void(int)>*>(
                io_uring_cqe_get_data(cqe)
            );
            
            // Вызываем callback с результатом
            if (callback) {
                (*callback)(cqe->res);  // res - количество байт или код ошибки
                delete callback;
            }
            
            count++;
        }
        
        // Помечаем обработанные завершения
        io_uring_cq_advance(&ring_, count);
    }
    
    // Zero-copy передача между файловыми дескрипторами
    void splice(int fd_in, int fd_out, size_t len,
                std::function<void(int)> callback) {
        io_uring_sqe* sqe = io_uring_get_sqe(&ring_);
        io_uring_prep_splice(sqe, fd_in, -1, fd_out, -1, len, 0);
        io_uring_sqe_set_data(sqe, new std::function(callback));
        io_uring_submit(&ring_);
    }
    
    // Цепочка операций - выполняются последовательно
    void read_then_write(int fd_in, int fd_out, void* buffer, size_t size) {
        // Операция 1: Чтение
        io_uring_sqe* sqe1 = io_uring_get_sqe(&ring_);
        io_uring_prep_read(sqe1, fd_in, buffer, size, 0);
        sqe1->flags |= IOSQE_IO_LINK;  // Следующая операция зависит от этой
        
        // Операция 2: Запись (выполнится только если чтение успешно)
        io_uring_sqe* sqe2 = io_uring_get_sqe(&ring_);
        io_uring_prep_write(sqe2, fd_out, buffer, size, 0);
        
        io_uring_submit(&ring_);
    }
};

// Пример использования io_uring
void io_uring_example() {
    IoUringEventLoop uring;
    
    int fd = open("test.txt", O_RDONLY);
    char buffer[4096];
    
    uring.async_read(fd, buffer, sizeof(buffer), 0, 
        [](int result) {
            if (result > 0) {
                std::cout << "Read " << result << " bytes\n";
            } else {
                std::cerr << "Read failed\n";
            }
        }
    );
    
    // Обработка завершённых операций
    uring.process_completions();
    
    close(fd);
}

// ============================================
// 📌 Async Socket Operations (полный пример)
// ============================================

class FullAsyncSocket {
    int fd_;
    EpollEventLoop& loop_;
    
public:
    FullAsyncSocket(int fd, EpollEventLoop& loop) : fd_(fd), loop_(loop) {}
    
    // Асинхронное подключение
    void async_connect(const sockaddr* addr, socklen_t len,
                      std::function<void(bool success)> callback) {
        // Переключаем в non-blocking
        int flags = fcntl(fd_, F_GETFL, 0);
        fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
        
        // Начинаем подключение
        int result = connect(fd_, addr, len);
        
        if (result == 0) {
            // Подключились сразу (маловероятно)
            callback(true);
            return;
        }
        
        if (errno != EINPROGRESS) {
            callback(false);
            return;
        }
        
        // Ждём события записи (означает завершение connect)
        // Здесь нужно использовать event loop для мониторинга EPOLLOUT
        callback(true);  // Упрощённо
    }
    
    // Асинхронное чтение
    void async_read(char* buffer, size_t size,
                   std::function<void(ssize_t bytes)> callback) {
        loop_.add_edge_triggered(fd_, [this, buffer, size, callback]() {
            // Edge-triggered: читаем всё что доступно
            ssize_t total = 0;
            
            while (true) {
                ssize_t n = recv(fd_, buffer + total, size - total, 0);
                
                if (n > 0) {
                    total += n;
                } else if (n == 0) {
                    callback(total);  // EOF
                    return;
                } else {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        callback(total);  // Прочитали всё доступное
                        return;
                    } else {
                        callback(-1);  // Ошибка
                        return;
                    }
                }
            }
        });
    }
    
    // Асинхронная запись
    void async_write(const char* data, size_t size,
                    std::function<void(ssize_t bytes)> callback) {
        // Попытка записать сразу
        ssize_t n = send(fd_, data, size, 0);
        
        if (n == size) {
            callback(n);  // Записали всё
            return;
        }
        
        if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            callback(-1);  // Ошибка
            return;
        }
        
        // Не всё записалось, ждём EPOLLOUT
        size_t written = (n > 0) ? n : 0;
        // Регистрируем обработчик для дозаписи...
        callback(written);
    }
    
    // Асинхронное принятие соединений
    void async_accept(std::function<void(int client_fd)> callback) {
        loop_.add_level_triggered(fd_, [this, callback]() {
            int client_fd = accept(fd_, nullptr, nullptr);
            
            if (client_fd >= 0) {
                callback(client_fd);
            }
        });
    }
};

// ============================================
// 📌 Thread Pool для Network I/O
// ============================================

class NetworkThreadPool {
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_ = false;
    
public:
    explicit NetworkThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    
                    {
                        std::unique_lock lock(mutex_);
                        cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                        
                        if (stop_ && tasks_.empty()) return;
                        
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    
                    task();
                }
            });
        }
    }
    
    ~NetworkThreadPool() {
        {
            std::unique_lock lock(mutex_);
            stop_ = true;
        }
        
        cv_.notify_all();
        
        for (auto& worker : workers_) {
            if (worker.joinable()) worker.join();
        }
    }
    
    void submit(std::function<void()> task) {
        {
            std::unique_lock lock(mutex_);
            tasks_.push(std::move(task));
        }
        cv_.notify_one();
    }
};

// Thread-per-core architecture
class ThreadPerCoreServer {
    std::vector<std::thread> workers_;
    std::vector<int> listen_fds_;  // По одному listen socket на поток
    
public:
    void start(uint16_t port, size_t num_cores) {
        for (size_t i = 0; i < num_cores; ++i) {
            workers_.emplace_back([this, port]() {
                // Каждый поток создаёт свой listen socket с SO_REUSEPORT
                int fd = socket(AF_INET, SOCK_STREAM, 0);
                
                int opt = 1;
                setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
                setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
                
                sockaddr_in addr{};
                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = INADDR_ANY;
                addr.sin_port = htons(port);
                
                bind(fd, (sockaddr*)&addr, sizeof(addr));
                listen(fd, 128);
                
                // Каждый поток обрабатывает свои соединения
                EpollEventLoop loop;
                loop.add_level_triggered(fd, [fd]() {
                    int client = accept(fd, nullptr, nullptr);
                    // Обработка клиента...
                });
                
                loop.run();
            });
        }
    }
};

// ============================================
// 📌 Buffering Strategies
// ============================================

// Ring Buffer для эффективной буферизации
class RingBuffer {
    std::vector<char> buffer_;
    size_t read_pos_ = 0;
    size_t write_pos_ = 0;
    size_t size_;
    
public:
    explicit RingBuffer(size_t size) : buffer_(size), size_(size) {}
    
    size_t write(const char* data, size_t len) {
        size_t available = (size_ - write_pos_ + read_pos_ - 1) % size_;
        size_t to_write = std::min(len, available);
        
        if (to_write == 0) return 0;
        
        // Записываем данные циклически
        if (write_pos_ + to_write <= size_) {
            std::memcpy(buffer_.data() + write_pos_, data, to_write);
            write_pos_ = (write_pos_ + to_write) % size_;
        } else {
            size_t first_part = size_ - write_pos_;
            std::memcpy(buffer_.data() + write_pos_, data, first_part);
            std::memcpy(buffer_.data(), data + first_part, to_write - first_part);
            write_pos_ = to_write - first_part;
        }
        
        return to_write;
    }
    
    size_t read(char* data, size_t len) {
        size_t available = (size_ - read_pos_ + write_pos_) % size_;
        size_t to_read = std::min(len, available);
        
        if (to_read == 0) return 0;
        
        if (read_pos_ + to_read <= size_) {
            std::memcpy(data, buffer_.data() + read_pos_, to_read);
            read_pos_ = (read_pos_ + to_read) % size_;
        } else {
            size_t first_part = size_ - read_pos_;
            std::memcpy(data, buffer_.data() + read_pos_, first_part);
            std::memcpy(data + first_part, buffer_.data(), to_read - first_part);
            read_pos_ = to_read - first_part;
        }
        
        return to_read;
    }
};

// Scatter/Gather I/O - векторизованные операции
void scatter_gather_example(int sockfd) {
    // Подготовка нескольких буферов
    char header[128];
    char body[1024];
    char footer[64];
    
    // Scatter read - читаем в несколько буферов одной операцией
    iovec iov[3];
    iov[0].iov_base = header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = body;
    iov[1].iov_len = sizeof(body);
    iov[2].iov_base = footer;
    iov[2].iov_len = sizeof(footer);
    
    ssize_t n = readv(sockfd, iov, 3);
    
    // Gather write - отправляем несколько буферов одной операцией
    ssize_t sent = writev(sockfd, iov, 3);
    
    std::cout << "Read: " << n << ", Sent: " << sent << '\n';
}

// Buffer Pool - переиспользование буферов
class BufferPool {
    std::vector<std::unique_ptr<char[]>> buffers_;
    std::queue<char*> available_;
    std::mutex mutex_;
    size_t buffer_size_;
    
public:
    BufferPool(size_t buffer_size, size_t count) 
        : buffer_size_(buffer_size) {
        for (size_t i = 0; i < count; ++i) {
            auto buffer = std::make_unique<char[]>(buffer_size);
            available_.push(buffer.get());
            buffers_.push_back(std::move(buffer));
        }
    }
    
    char* acquire() {
        std::lock_guard lock(mutex_);
        
        if (available_.empty()) {
            // Создаём новый буфер
            auto buffer = std::make_unique<char[]>(buffer_size_);
            char* ptr = buffer.get();
            buffers_.push_back(std::move(buffer));
            return ptr;
        }
        
        char* buffer = available_.front();
        available_.pop();
        return buffer;
    }
    
    void release(char* buffer) {
        std::lock_guard lock(mutex_);
        available_.push(buffer);
    }
};

// ============================================
// 📌 Performance Optimization
// ============================================

void optimize_socket(int sockfd) {
    // 1. Отключаем алгоритм Nagle для низкой латентности
    int nodelay = 1;
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));
    
    // 2. Увеличиваем буферы приёма/отправки
    int bufsize = 256 * 1024;  // 256 KB
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
    
    // 3. Включаем TCP_QUICKACK (Linux) - отправляем ACK немедленно
    int quickack = 1;
    setsockopt(sockfd, IPPROTO_TCP, TCP_QUICKACK, &quickack, sizeof(quickack));
    
    // 4. Настройка TCP_CORK - накапливаем данные перед отправкой
    // Полезно для отправки больших объёмов данных
    int cork = 1;
    setsockopt(sockfd, IPPROTO_TCP, TCP_CORK, &cork, sizeof(cork));
    
    // Отправка данных...
    
    // Отключаем cork для отправки
    cork = 0;
    setsockopt(sockfd, IPPROTO_TCP, TCP_CORK, &cork, sizeof(cork));
}

// ============================================
// 📌 Timeout Management
// ============================================

class TimeoutManager {
    struct TimeoutEntry {
        std::chrono::steady_clock::time_point deadline;
        std::function<void()> callback;
        int fd;
    };
    
    std::vector<TimeoutEntry> timeouts_;
    std::mutex mutex_;
    
public:
    void add_timeout(int fd, std::chrono::milliseconds duration,
                    std::function<void()> callback) {
        std::lock_guard lock(mutex_);
        
        TimeoutEntry entry;
        entry.deadline = std::chrono::steady_clock::now() + duration;
        entry.callback = std::move(callback);
        entry.fd = fd;
        
        timeouts_.push_back(std::move(entry));
    }
    
    void cancel_timeout(int fd) {
        std::lock_guard lock(mutex_);
        
        timeouts_.erase(
            std::remove_if(timeouts_.begin(), timeouts_.end(),
                [fd](const TimeoutEntry& e) { return e.fd == fd; }),
            timeouts_.end()
        );
    }
    
    void process() {
        std::lock_guard lock(mutex_);
        auto now = std::chrono::steady_clock::now();
        
        for (auto it = timeouts_.begin(); it != timeouts_.end(); ) {
            if (it->deadline <= now) {
                it->callback();
                it = timeouts_.erase(it);
            } else {
                ++it;
            }
        }
    }
};

// Cancellation Token для отмены операций
class CancellationToken {
    std::atomic<bool> cancelled_{false};
    
public:
    void cancel() {
        cancelled_ = true;
    }
    
    bool is_cancelled() const {
        return cancelled_.load();
    }
};

// Использование с async операциями
void async_read_with_cancellation(int fd, char* buffer, size_t size,
                                  CancellationToken& token,
                                  std::function<void(ssize_t)> callback) {
    std::thread([fd, buffer, size, &token, callback]() {
        while (!token.is_cancelled()) {
            ssize_t n = recv(fd, buffer, size, MSG_DONTWAIT);
            
            if (n >= 0 || errno != EAGAIN) {
                if (!token.is_cancelled()) {
                    callback(n);
                }
                return;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        // Операция отменена
        callback(-1);
    }).detach();
}