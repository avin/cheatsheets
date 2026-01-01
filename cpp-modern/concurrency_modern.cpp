/*
 * ============================================
 * СОВРЕМЕННАЯ МНОГОПОТОЧНОСТЬ C++20/23
 * ============================================
 * 
 * Полное руководство по многопоточному программированию в C++
 * с использованием новейших возможностей C++20/23.
 * 
 * Требования: C++20 или выше
 * Компиляция: g++ -std=c++20 -pthread concurrency_modern.cpp
 */

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <semaphore>       // C++20
#include <latch>           // C++20
#include <barrier>         // C++20
#include <stop_token>      // C++20
#include <atomic>
#include <future>
#include <queue>
#include <vector>
#include <iostream>
#include <chrono>
#include <functional>
#include <memory>
#include <syncstream>      // C++20

using namespace std::chrono_literals;

// ============================================
// 📌 THREAD BASICS
// ============================================

void demo_basic_threads() {
    std::cout << "=== Basic Threads ===\n";
    
    // Простейший способ создания потока
    std::thread t1([] {
        std::cout << "Hello from thread!\n";
    });
    
    // Поток с параметрами
    auto task = [](int id, const std::string& msg) {
        std::cout << "Thread " << id << ": " << msg << '\n';
    };
    
    std::thread t2(task, 1, "First thread");
    std::thread t3(task, 2, "Second thread");
    
    // ВАЖНО: необходимо join или detach
    t1.join();  // Ждем завершения потока
    t2.join();
    t3.join();
    
    // detach - поток работает независимо
    std::thread t4([] {
        std::this_thread::sleep_for(100ms);
        std::cout << "Detached thread\n";
    });
    t4.detach();  // Поток отсоединен, живет своей жизнью
    
    // Информация о системе
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << "Hardware threads: " << n << '\n';
    
    // Thread ID
    auto id = std::this_thread::get_id();
    std::cout << "Current thread ID: " << id << '\n';
}

// C++20: std::jthread - автоматический join
void demo_jthread() {
    std::cout << "\n=== std::jthread (C++20) ===\n";
    
    {
        std::jthread t([] {
            std::cout << "jthread: автоматический join при выходе из scope\n";
        });
        // Не нужен явный join - выполнится автоматически!
    }
    
    // jthread с stop_token для корректной остановки
    std::jthread t([](std::stop_token stoken) {
        int count = 0;
        while (!stoken.stop_requested()) {
            std::cout << "Working... " << count++ << '\n';
            std::this_thread::sleep_for(100ms);
        }
        std::cout << "Stopping gracefully\n";
    });
    
    std::this_thread::sleep_for(500ms);
    t.request_stop();  // Запрос на остановку
    // join автоматически при деструкции
}

// ============================================
// 📌 MUTEXES И LOCKS
// ============================================

class Counter {
private:
    mutable std::mutex mutex_;
    int value_ = 0;
    
public:
    // std::lock_guard - простейший RAII lock
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++value_;
    }  // Автоматический unlock
    
    // std::unique_lock - более гибкий
    void add(int n) {
        std::unique_lock<std::mutex> lock(mutex_);
        value_ += n;
        // Можно unlock раньше:
        lock.unlock();
        // Длинная операция без блокировки
        std::this_thread::sleep_for(1ms);
    }
    
    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }
};

// std::shared_mutex (C++17) - multiple readers, single writer
class SharedData {
private:
    mutable std::shared_mutex mutex_;
    std::vector<int> data_;
    
public:
    // Запись - эксклюзивная блокировка
    void write(int value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        data_.push_back(value);
    }
    
    // Чтение - разделяемая блокировка
    size_t size() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return data_.size();  // Множество читателей одновременно
    }
    
    int at(size_t index) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return data_.at(index);
    }
};

// std::scoped_lock (C++17) - блокировка нескольких мьютексов
void transfer(Counter& from, Counter& to, int amount) {
    // Блокирует оба мьютекса атомарно, избегая deadlock
    std::scoped_lock lock(from.mutex_, to.mutex_);
    from.value_ -= amount;
    to.value_ += amount;
}

// std::recursive_mutex - позволяет повторную блокировку в том же потоке
class RecursiveCounter {
    mutable std::recursive_mutex mutex_;
    int value_ = 0;
    
public:
    void increment() {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        ++value_;
    }
    
    void add_and_log(int n) {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        value_ += n;
        log();  // Повторная блокировка - OK для recursive_mutex
    }
    
    void log() {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        std::cout << "Value: " << value_ << '\n';
    }
};

// std::timed_mutex - блокировка с таймаутом
void demo_timed_mutex() {
    std::timed_mutex mutex;
    
    auto worker = [&](int id) {
        // Попытка захвата с таймаутом
        if (mutex.try_lock_for(100ms)) {
            std::cout << "Thread " << id << " got lock\n";
            std::this_thread::sleep_for(50ms);
            mutex.unlock();
        } else {
            std::cout << "Thread " << id << " timeout\n";
        }
    };
    
    std::jthread t1(worker, 1);
    std::jthread t2(worker, 2);
}

// ============================================
// 📌 CONDITION VARIABLES
// ============================================

class ThreadSafeQueue {
private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<int> queue_;
    bool done_ = false;
    
public:
    void push(int value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(value);
        }
        cv_.notify_one();  // Уведомить один ждущий поток
    }
    
    bool pop(int& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Ждем пока очередь не пуста или не завершена
        cv_.wait(lock, [this] { 
            return !queue_.empty() || done_; 
        });
        
        if (queue_.empty()) {
            return false;  // Очередь завершена
        }
        
        value = queue_.front();
        queue_.pop();
        return true;
    }
    
    // Попытка pop с таймаутом
    bool try_pop(int& value, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        if (!cv_.wait_for(lock, timeout, [this] { 
            return !queue_.empty() || done_; 
        })) {
            return false;  // Таймаут
        }
        
        if (queue_.empty()) {
            return false;
        }
        
        value = queue_.front();
        queue_.pop();
        return true;
    }
    
    void finish() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            done_ = true;
        }
        cv_.notify_all();  // Разбудить все потоки
    }
};

void demo_producer_consumer() {
    ThreadSafeQueue queue;
    
    // Producer
    std::jthread producer([&] {
        for (int i = 0; i < 10; ++i) {
            queue.push(i);
            std::cout << "Produced: " << i << '\n';
            std::this_thread::sleep_for(50ms);
        }
        queue.finish();
    });
    
    // Consumer
    std::jthread consumer([&] {
        int value;
        while (queue.pop(value)) {
            std::cout << "Consumed: " << value << '\n';
        }
    });
}

// ============================================
// 📌 SEMAPHORES (C++20)
// ============================================

void demo_semaphores() {
    std::cout << "\n=== Semaphores (C++20) ===\n";
    
    // counting_semaphore - ограничение числа одновременных доступов
    std::counting_semaphore<3> pool_semaphore(3);  // Максимум 3 потока
    
    auto worker = [&](int id) {
        pool_semaphore.acquire();  // Ждем свободного слота
        std::cout << "Thread " << id << " working\n";
        std::this_thread::sleep_for(100ms);
        std::cout << "Thread " << id << " done\n";
        pool_semaphore.release();  // Освобождаем слот
    };
    
    std::vector<std::jthread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(worker, i);
    }
    
    // binary_semaphore - как мьютекс (0 или 1)
    std::binary_semaphore signal(0);
    
    std::jthread signaler([&] {
        std::this_thread::sleep_for(200ms);
        std::cout << "Sending signal\n";
        signal.release();
    });
    
    std::jthread waiter([&] {
        std::cout << "Waiting for signal...\n";
        signal.acquire();
        std::cout << "Signal received!\n";
    });
}

// ============================================
// 📌 LATCHES И BARRIERS (C++20)
// ============================================

void demo_latch() {
    std::cout << "\n=== Latch (C++20) ===\n";
    
    // latch - одноразовый барьер
    std::latch workers_done(3);  // Ждем 3 потока
    
    auto worker = [&](int id) {
        std::cout << "Worker " << id << " starting\n";
        std::this_thread::sleep_for(100ms * id);
        std::cout << "Worker " << id << " done\n";
        workers_done.count_down();  // Уменьшаем счетчик
    };
    
    std::jthread t1(worker, 1);
    std::jthread t2(worker, 2);
    std::jthread t3(worker, 3);
    
    workers_done.wait();  // Ждем пока все 3 не завершатся
    std::cout << "All workers completed!\n";
}

void demo_barrier() {
    std::cout << "\n=== Barrier (C++20) ===\n";
    
    // barrier - многоразовый барьер с опциональной функцией
    auto on_completion = []() noexcept {
        std::cout << "--- Phase completed ---\n";
    };
    
    std::barrier sync_point(3, on_completion);
    
    auto phased_worker = [&](int id) {
        for (int phase = 0; phase < 3; ++phase) {
            std::cout << "Worker " << id << " phase " << phase << '\n';
            std::this_thread::sleep_for(100ms);
            
            sync_point.arrive_and_wait();  // Ждем всех
            // После этой точки все потоки синхронизированы
        }
    };
    
    std::jthread t1(phased_worker, 1);
    std::jthread t2(phased_worker, 2);
    std::jthread t3(phased_worker, 3);
}

// ============================================
// 📌 ATOMIC OPERATIONS
// ============================================

void demo_atomic_basics() {
    std::cout << "\n=== Atomic Basics ===\n";
    
    // Базовые атомарные операции
    std::atomic<int> counter{0};
    
    // Атомарные операции
    counter++;                           // Атомарный инкремент
    counter.fetch_add(5);               // Добавить и вернуть старое значение
    int old = counter.exchange(100);    // Обменять значение
    
    // Compare-exchange (CAS - Compare And Swap)
    int expected = 100;
    bool success = counter.compare_exchange_strong(expected, 200);
    // Если counter == 100, устанавливает 200 и возвращает true
    // Иначе записывает текущее значение в expected и возвращает false
    
    // Слабая версия (может ложно провалиться)
    while (!counter.compare_exchange_weak(expected, 300)) {
        // В цикле из-за spurious failures
    }
    
    // Чтение и запись
    int value = counter.load();
    counter.store(500);
    
    // is_lock_free - использует ли атомарные инструкции CPU
    std::cout << "int is lock-free: " << std::atomic<int>::is_always_lock_free << '\n';
}

// Memory ordering (модель памяти)
void demo_memory_ordering() {
    std::atomic<int> data{0};
    std::atomic<bool> ready{false};
    
    // Producer с Release семантикой
    auto producer = [&] {
        data.store(42, std::memory_order_relaxed);
        // Release гарантирует, что все операции выше видны другим потокам
        ready.store(true, std::memory_order_release);
    };
    
    // Consumer с Acquire семантикой
    auto consumer = [&] {
        // Acquire гарантирует видимость всех операций до release
        while (!ready.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        // Здесь гарантированно видим data == 42
        std::cout << "Data: " << data.load(std::memory_order_relaxed) << '\n';
    };
    
    std::jthread t1(producer);
    std::jthread t2(consumer);
}

// Atomic wait/notify (C++20) - легковесная альтернатива condition_variable
void demo_atomic_wait() {
    std::atomic<int> value{0};
    
    std::jthread waiter([&] {
        std::cout << "Waiting for value to change...\n";
        value.wait(0);  // Ждем пока value != 0
        std::cout << "Value changed to: " << value.load() << '\n';
    });
    
    std::this_thread::sleep_for(100ms);
    value.store(42);
    value.notify_one();  // Уведомить ждущий поток
}

// Atomic smart pointers (C++20)
void demo_atomic_shared_ptr() {
    // До C++20 нужны были специальные функции std::atomic_store/load
    // С C++20 можно использовать std::atomic напрямую
    std::atomic<std::shared_ptr<int>> ptr;
    
    ptr.store(std::make_shared<int>(42));
    auto current = ptr.load();
    
    // Compare-exchange для безопасной замены
    auto expected = current;
    auto desired = std::make_shared<int>(100);
    ptr.compare_exchange_strong(expected, desired);
}

// ============================================
// 📌 FUTURES И PROMISES
// ============================================

void demo_async_future() {
    std::cout << "\n=== Async & Future ===\n";
    
    // std::async - простейший способ асинхронных вычислений
    auto future = std::async(std::launch::async, [] {
        std::this_thread::sleep_for(100ms);
        return 42;
    });
    
    std::cout << "Waiting for result...\n";
    int result = future.get();  // Блокируется до готовности
    std::cout << "Result: " << result << '\n';
    
    // Launch policies:
    // std::launch::async - гарантированно в новом потоке
    // std::launch::deferred - выполнится при вызове get()
    // std::launch::async | std::launch::deferred - на усмотрение реализации
    
    auto deferred = std::async(std::launch::deferred, [] {
        std::cout << "This runs in calling thread\n";
        return 100;
    });
    // Ничего не выполнилось до вызова get()
    deferred.get();  // Выполняется здесь
}

void demo_promise() {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    
    std::jthread worker([&] {
        try {
            // Какая-то работа
            std::this_thread::sleep_for(100ms);
            int result = 42;
            
            promise.set_value(result);  // Устанавливаем результат
        } catch (...) {
            promise.set_exception(std::current_exception());
        }
    });
    
    // Ждем результата
    try {
        int value = future.get();
        std::cout << "Promise result: " << value << '\n';
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << '\n';
    }
}

void demo_packaged_task() {
    // packaged_task - оборачивает callable и предоставляет future
    std::packaged_task<int(int, int)> task([](int a, int b) {
        return a + b;
    });
    
    std::future<int> future = task.get_future();
    
    // Выполняем в другом потоке
    std::jthread t(std::move(task), 10, 20);
    
    std::cout << "Packaged task result: " << future.get() << '\n';
}

// shared_future - множественные ожидающие
void demo_shared_future() {
    std::promise<int> promise;
    std::shared_future<int> shared_future = promise.get_future();
    
    // Несколько потоков могут ждать один результат
    auto waiter = [](int id, std::shared_future<int> f) {
        std::cout << "Thread " << id << " waiting...\n";
        int value = f.get();  // Можно вызвать get() многократно
        std::cout << "Thread " << id << " got: " << value << '\n';
    };
    
    std::jthread t1(waiter, 1, shared_future);
    std::jthread t2(waiter, 2, shared_future);
    std::jthread t3(waiter, 3, shared_future);
    
    std::this_thread::sleep_for(100ms);
    promise.set_value(42);
}

// ============================================
// 📌 THREAD POOL
// ============================================

class ThreadPool {
private:
    std::vector<std::jthread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable cv_;
    bool stop_ = false;
    
public:
    explicit ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex_);
                        cv_.wait(lock, [this] { 
                            return stop_ || !tasks_.empty(); 
                        });
                        
                        if (stop_ && tasks_.empty()) {
                            return;
                        }
                        
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    
                    task();
                }
            });
        }
    }
    
    template<typename F>
    auto enqueue(F&& f) -> std::future<decltype(f())> {
        using return_type = decltype(f());
        
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::forward<F>(f)
        );
        
        std::future<return_type> result = task->get_future();
        
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if (stop_) {
                throw std::runtime_error("ThreadPool stopped");
            }
            tasks_.emplace([task]() { (*task)(); });
        }
        
        cv_.notify_one();
        return result;
    }
    
    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }
        cv_.notify_all();
    }
};

void demo_thread_pool() {
    std::cout << "\n=== Thread Pool ===\n";
    
    ThreadPool pool(4);
    
    std::vector<std::future<int>> results;
    
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "Task " << i << " running\n";
                std::this_thread::sleep_for(100ms);
                return i * i;
            })
        );
    }
    
    for (auto& result : results) {
        std::cout << "Result: " << result.get() << '\n';
    }
}

// ============================================
// 📌 LOCK-FREE PROGRAMMING
// ============================================

// Простой lock-free stack
template<typename T>
class LockFreeStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T d) : data(std::move(d)), next(nullptr) {}
    };
    
    std::atomic<Node*> head_{nullptr};
    
public:
    void push(T data) {
        Node* new_node = new Node(std::move(data));
        new_node->next = head_.load();
        
        // CAS loop - повторяем пока не успешно
        while (!head_.compare_exchange_weak(new_node->next, new_node)) {
            // new_node->next обновлен текущим head_, повторяем
        }
    }
    
    bool pop(T& result) {
        Node* old_head = head_.load();
        
        while (old_head && 
               !head_.compare_exchange_weak(old_head, old_head->next)) {
            // Повторяем если другой поток изменил head
        }
        
        if (!old_head) {
            return false;  // Стек пуст
        }
        
        result = std::move(old_head->data);
        delete old_head;  // ВНИМАНИЕ: проблема ABA!
        return true;
    }
    
    ~LockFreeStack() {
        T dummy;
        while (pop(dummy)) {}
    }
};

// ============================================
// 📌 STOP TOKENS (C++20)
// ============================================

void demo_stop_tokens() {
    std::cout << "\n=== Stop Tokens (C++20) ===\n";
    
    // jthread автоматически предоставляет stop_token
    std::jthread t([](std::stop_token stoken) {
        int count = 0;
        while (!stoken.stop_requested()) {
            std::cout << "Iteration " << count++ << '\n';
            std::this_thread::sleep_for(100ms);
            
            if (count >= 5) break;
        }
        
        if (stoken.stop_requested()) {
            std::cout << "Stop requested!\n";
        }
    });
    
    std::this_thread::sleep_for(250ms);
    t.request_stop();  // Кооперативная остановка
    
    // stop_source и stop_token можно использовать независимо
    std::stop_source source;
    std::stop_token token = source.get_token();
    
    // Callback при остановке
    std::stop_callback callback(token, [] {
        std::cout << "Stop callback invoked!\n";
    });
    
    source.request_stop();  // Вызовет callback
}

// ============================================
// 📌 COMMON PATTERNS
// ============================================

// Паттерн: Thread-safe Singleton (C++11 гарантирует thread-safety)
class Singleton {
private:
    Singleton() = default;
    
public:
    static Singleton& instance() {
        static Singleton inst;  // Thread-safe с C++11
        return inst;
    }
    
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

// Паттерн: Monitor (синхронизированный объект)
template<typename T>
class Monitor {
private:
    mutable std::mutex mutex_;
    T data_;
    
public:
    template<typename F>
    auto execute(F&& f) {
        std::lock_guard<std::mutex> lock(mutex_);
        return f(data_);
    }
    
    template<typename F>
    auto execute(F&& f) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return f(data_);
    }
};

// Использование Monitor
void demo_monitor() {
    Monitor<std::vector<int>> mon;
    
    mon.execute([](auto& vec) {
        vec.push_back(42);
    });
    
    auto size = mon.execute([](const auto& vec) {
        return vec.size();
    });
}

// Паттерн: Double-Checked Locking (правильная версия с C++11)
class LazyInit {
private:
    static std::atomic<LazyInit*> instance_;
    static std::mutex mutex_;
    
    LazyInit() = default;
    
public:
    static LazyInit* get_instance() {
        LazyInit* tmp = instance_.load(std::memory_order_acquire);
        if (!tmp) {
            std::lock_guard<std::mutex> lock(mutex_);
            tmp = instance_.load(std::memory_order_relaxed);
            if (!tmp) {
                tmp = new LazyInit();
                instance_.store(tmp, std::memory_order_release);
            }
        }
        return tmp;
    }
};

std::atomic<LazyInit*> LazyInit::instance_{nullptr};
std::mutex LazyInit::mutex_;

// ============================================
// 📌 PERFORMANCE & BEST PRACTICES
// ============================================

/*
 * FALSE SHARING - проблема производительности
 * 
 * Когда два потока обращаются к разным переменным,
 * но они находятся в одной cache line (обычно 64 байта),
 * происходит постоянная инвалидация кеша между ядрами.
 */

// ❌ ПЛОХО - False sharing
struct BadCounters {
    std::atomic<int> counter1;  // Могут быть в одной cache line
    std::atomic<int> counter2;
};

// ✅ ХОРОШО - Выравнивание по cache line
struct alignas(64) AlignedCounter {
    std::atomic<int> value;
};

struct GoodCounters {
    AlignedCounter counter1;  // Гарантированно в разных cache lines
    AlignedCounter counter2;
};

/*
 * LOCK CONTENTION - борьба за блокировки
 * 
 * Стратегии уменьшения:
 * 1. Минимизируйте критические секции
 * 2. Используйте lock-free структуры где возможно
 * 3. Sharding - разделение на независимые части
 * 4. Read-write locks для read-heavy сценариев
 */

// Sharding пример
template<size_t N = 16>
class ShardedCounter {
private:
    struct alignas(64) Shard {
        std::atomic<int> value{0};
    };
    
    std::array<Shard, N> shards_;
    
    size_t get_shard() const {
        static thread_local size_t shard = std::hash<std::thread::id>{}(
            std::this_thread::get_id()
        ) % N;
        return shard;
    }
    
public:
    void increment() {
        ++shards_[get_shard()].value;
    }
    
    int total() const {
        int sum = 0;
        for (const auto& shard : shards_) {
            sum += shard.value.load();
        }
        return sum;
    }
};

/*
 * DEBUGGING TIPS:
 * 
 * 1. Используйте ThreadSanitizer (TSan):
 *    g++ -fsanitize=thread -g program.cpp
 * 
 * 2. Valgrind Helgrind:
 *    valgrind --tool=helgrind ./program
 * 
 * 3. std::osyncstream (C++20) для thread-safe вывода:
 */

void demo_osyncstream() {
    auto worker = [](int id) {
        std::osyncstream(std::cout) 
            << "Thread " << id << " output\n";
        // Атомарный вывод, не перемешивается
    };
    
    std::jthread t1(worker, 1);
    std::jthread t2(worker, 2);
}

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== Modern C++ Concurrency Guide ===\n\n";
    
    demo_basic_threads();
    demo_jthread();
    demo_producer_consumer();
    demo_semaphores();
    demo_latch();
    demo_barrier();
    demo_atomic_basics();
    demo_memory_ordering();
    demo_atomic_wait();
    demo_async_future();
    demo_promise();
    demo_packaged_task();
    demo_shared_future();
    demo_thread_pool();
    demo_stop_tokens();
    demo_osyncstream();
    
    std::cout << "\n=== Concurrency Best Practices ===\n";
    std::cout << "1. Предпочитайте jthread вместо thread\n";
    std::cout << "2. Используйте RAII locks (lock_guard, unique_lock)\n";
    std::cout << "3. Избегайте ручного управления мьютексами\n";
    std::cout << "4. Используйте std::scoped_lock для множественных мьютексов\n";
    std::cout << "5. Предпочитайте message passing вместо shared state\n";
    std::cout << "6. Используйте атомики для простых счетчиков/флагов\n";
    std::cout << "7. Профилируйте перед оптимизацией!\n";
    std::cout << "8. Тестируйте с ThreadSanitizer\n";
    
    return 0;
}
