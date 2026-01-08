/*
 * ============================================
 * КОРУТИНЫ C++20 - ПОЛНОЕ РУКОВОДСТВО
 * ============================================
 * 
 * Подробное руководство по корутинам C++20 с практическими
 * примерами generator<T>, task<T> и async операций.
 * 
 * Требования: C++20 или выше
 * Компиляция: g++ -std=c++20 -fcoroutines coroutines.cpp
 */

#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <optional>
#include <vector>
#include <string>
#include <chrono>

// ============================================
// 📌 COROUTINE BASICS - КЛЮЧЕВЫЕ СЛОВА
// ============================================

/*
 * Три ключевых слова делают функцию корутиной:
 * 
 * co_await  - приостановить выполнение и ждать результата
 * co_yield  - приостановить и вернуть значение (для generators)
 * co_return - завершить корутину и вернуть результат
 * 
 * Любая функция с хотя бы одним из этих ключевых слов
 * становится корутиной.
 */

// ============================================
// 📌 GENERATOR<T> - ПОЛНАЯ РЕАЛИЗАЦИЯ
// ============================================

template<typename T>
class generator {
public:
    // Promise type - управляет поведением корутины
    struct promise_type {
        T current_value_;
        std::exception_ptr exception_;
        
        generator get_return_object() {
            return generator{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        
        std::suspend_always yield_value(T value) {
            current_value_ = std::move(value);
            return {};
        }
        
        void return_void() {}
        
        void unhandled_exception() {
            exception_ = std::current_exception();
        }
    };
    
    // Iterator для range-based for
    struct iterator {
        std::coroutine_handle<promise_type> coro_;
        bool done_;
        
        iterator(std::coroutine_handle<promise_type> coro, bool done)
            : coro_(coro), done_(done) {
            if (!done_) {
                coro_.resume();
                done_ = coro_.done();
            }
        }
        
        iterator& operator++() {
            coro_.resume();
            done_ = coro_.done();
            return *this;
        }
        
        bool operator==(const iterator& other) const {
            return done_ == other.done_;
        }
        
        T& operator*() const {
            return coro_.promise().current_value_;
        }
    };
    
    iterator begin() {
        if (coro_) {
            return iterator{coro_, false};
        }
        return iterator{nullptr, true};
    }
    
    iterator end() {
        return iterator{nullptr, true};
    }
    
    explicit generator(std::coroutine_handle<promise_type> h)
        : coro_(h) {}
    
    ~generator() {
        if (coro_) {
            coro_.destroy();
        }
    }
    
    generator(const generator&) = delete;
    generator& operator=(const generator&) = delete;
    
    generator(generator&& other) noexcept
        : coro_(std::exchange(other.coro_, {})) {}
    
    generator& operator=(generator&& other) noexcept {
        if (this != &other) {
            if (coro_) {
                coro_.destroy();
            }
            coro_ = std::exchange(other.coro_, {});
        }
        return *this;
    }
    
private:
    std::coroutine_handle<promise_type> coro_;
};

// Примеры использования generator
generator<int> range(int start, int end) {
    for (int i = start; i < end; ++i) {
        co_yield i;  // Приостановить и вернуть значение
    }
}

generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

// ────────────────────────────────────────────────────────────────────────────────────
// Примеры использования generator
// ────────────────────────────────────────────────────────────────────────────────────

// Range-based for с generator
std::cout << "Range 0-5: ";
for (int i : range(0, 5)) {
    std::cout << i << ' ';
}
std::cout << '\n';

// Fibonacci последовательность
std::cout << "First 10 Fibonacci: ";
int count = 0;
for (int fib : fibonacci()) {
    std::cout << fib << ' ';
    if (++count == 10) break;
}
std::cout << '\n';

// Рекурсивный generator
generator<int> tree_traverse(int depth) {
    if (depth == 0) co_return;
    
    co_yield depth;
    
    // Вложенные generators
    for (int val : tree_traverse(depth - 1)) {
        co_yield val;
    }
}

// Generator для чтения строк
generator<std::string> lines_from_string(const std::string& text) {
    size_t start = 0;
    while (start < text.size()) {
        size_t end = text.find('\n', start);
        if (end == std::string::npos) {
            end = text.size();
        }
        co_yield text.substr(start, end - start);
        start = end + 1;
    }
}

// ============================================
// 📌 TASK<T> - ASYNC ОПЕРАЦИИ
// ============================================

template<typename T>
class task {
public:
    struct promise_type {
        std::optional<T> result_;
        std::exception_ptr exception_;
        std::coroutine_handle<> continuation_;
        
        task get_return_object() {
            return task{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        
        std::suspend_always initial_suspend() { return {}; }
        
        struct final_awaiter {
            bool await_ready() noexcept { return false; }
            
            std::coroutine_handle<> await_suspend(
                std::coroutine_handle<promise_type> h) noexcept {
                auto& promise = h.promise();
                if (promise.continuation_) {
                    return promise.continuation_;  // Symmetric transfer
                }
                return std::noop_coroutine();
            }
            
            void await_resume() noexcept {}
        };
        
        final_awaiter final_suspend() noexcept { return {}; }
        
        void return_value(T value) {
            result_ = std::move(value);
        }
        
        void unhandled_exception() {
            exception_ = std::current_exception();
        }
    };
    
    // Awaiter для task
    struct awaiter {
        std::coroutine_handle<promise_type> coro_;
        
        bool await_ready() {
            return false;
        }
        
        std::coroutine_handle<> await_suspend(
            std::coroutine_handle<> awaiting_coro) {
            coro_.promise().continuation_ = awaiting_coro;
            return coro_;  // Symmetric transfer
        }
        
        T await_resume() {
            auto& promise = coro_.promise();
            if (promise.exception_) {
                std::rethrow_exception(promise.exception_);
            }
            return std::move(*promise.result_);
        }
    };
    
    awaiter operator co_await() {
        return awaiter{coro_};
    }
    
    explicit task(std::coroutine_handle<promise_type> h)
        : coro_(h) {}
    
    ~task() {
        if (coro_) {
            coro_.destroy();
        }
    }
    
    task(const task&) = delete;
    task& operator=(const task&) = delete;
    
    task(task&& other) noexcept
        : coro_(std::exchange(other.coro_, {})) {}
    
    task& operator=(task&& other) noexcept {
        if (this != &other) {
            if (coro_) {
                coro_.destroy();
            }
            coro_ = std::exchange(other.coro_, {});
        }
        return *this;
    }
    
    // Синхронное получение результата
    T get() {
        if (!coro_) {
            throw std::runtime_error("Empty task");
        }
        
        if (!coro_.done()) {
            coro_.resume();
        }
        
        auto& promise = coro_.promise();
        if (promise.exception_) {
            std::rethrow_exception(promise.exception_);
        }
        
        return std::move(*promise.result_);
    }
    
private:
    std::coroutine_handle<promise_type> coro_;
};

// Специализация для void
template<>
class task<void> {
public:
    struct promise_type {
        std::exception_ptr exception_;
        std::coroutine_handle<> continuation_;
        
        task get_return_object() {
            return task{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        
        std::suspend_always initial_suspend() { return {}; }
        
        struct final_awaiter {
            bool await_ready() noexcept { return false; }
            
            std::coroutine_handle<> await_suspend(
                std::coroutine_handle<promise_type> h) noexcept {
                auto& promise = h.promise();
                if (promise.continuation_) {
                    return promise.continuation_;
                }
                return std::noop_coroutine();
            }
            
            void await_resume() noexcept {}
        };
        
        final_awaiter final_suspend() noexcept { return {}; }
        
        void return_void() {}
        
        void unhandled_exception() {
            exception_ = std::current_exception();
        }
    };
    
    struct awaiter {
        std::coroutine_handle<promise_type> coro_;
        
        bool await_ready() { return false; }
        
        std::coroutine_handle<> await_suspend(
            std::coroutine_handle<> awaiting_coro) {
            coro_.promise().continuation_ = awaiting_coro;
            return coro_;
        }
        
        void await_resume() {
            auto& promise = coro_.promise();
            if (promise.exception_) {
                std::rethrow_exception(promise.exception_);
            }
        }
    };
    
    awaiter operator co_await() {
        return awaiter{coro_};
    }
    
    explicit task(std::coroutine_handle<promise_type> h)
        : coro_(h) {}
    
    ~task() {
        if (coro_) {
            coro_.destroy();
        }
    }
    
    task(const task&) = delete;
    task& operator=(const task&) = delete;
    
    task(task&& other) noexcept
        : coro_(std::exchange(other.coro_, {})) {}
    
    task& operator=(task&& other) noexcept {
        if (this != &other) {
            if (coro_) {
                coro_.destroy();
            }
            coro_ = std::exchange(other.coro_, {});
        }
        return *this;
    }
    
    void get() {
        if (!coro_) {
            throw std::runtime_error("Empty task");
        }
        
        if (!coro_.done()) {
            coro_.resume();
        }
        
        auto& promise = coro_.promise();
        if (promise.exception_) {
            std::rethrow_exception(promise.exception_);
        }
    }
    
private:
    std::coroutine_handle<promise_type> coro_;
};

// Примеры использования task
task<int> async_add(int a, int b) {
    std::cout << "Computing " << a << " + " << b << '\n';
    co_return a + b;
}

task<int> async_multiply(int a, int b) {
    std::cout << "Computing " << a << " * " << b << '\n';
    co_return a * b;
}

task<int> complex_computation() {
    int sum = co_await async_add(5, 10);
    std::cout << "Sum: " << sum << '\n';
    
    int product = co_await async_multiply(sum, 2);
    std::cout << "Product: " << product << '\n';
    
    co_return product + 100;
}

// Использование task
auto task_result = complex_computation();
int final_value = task_result.get();
std::cout << "Final result: " << final_value << '\n';

// ============================================
// 📌 CUSTOM AWAITABLES
// ============================================

// Awaitable для задержки (симуляция async sleep)
struct sleep_awaiter {
    std::chrono::milliseconds duration_;
    
    explicit sleep_awaiter(std::chrono::milliseconds duration)
        : duration_(duration) {}
    
    bool await_ready() const noexcept {
        return duration_.count() <= 0;
    }
    
    void await_suspend(std::coroutine_handle<> handle) const {
        // В реальной реализации здесь был бы timer в event loop
        // Для примера просто делаем sleep
        std::this_thread::sleep_for(duration_);
        handle.resume();
    }
    
    void await_resume() const noexcept {}
};

// Функция для удобного использования
sleep_awaiter sleep(std::chrono::milliseconds duration) {
    return sleep_awaiter{duration};
}

task<void> delayed_print(const std::string& msg, int delay_ms) {
    std::cout << "Waiting " << delay_ms << "ms...\n";
    co_await sleep(std::chrono::milliseconds{delay_ms});
    std::cout << msg << '\n';
}

// Awaitable для value (сразу готово)
template<typename T>
struct value_awaiter {
    T value_;
    
    explicit value_awaiter(T value) : value_(std::move(value)) {}
    
    bool await_ready() const noexcept { return true; }
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    T await_resume() { return std::move(value_); }
};

// ============================================
// 📌 LAZY EVALUATION
// ============================================

template<typename T>
class lazy {
public:
    struct promise_type {
        T value_;
        std::exception_ptr exception_;
        
        lazy get_return_object() {
            return lazy{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        
        void return_value(T value) {
            value_ = std::move(value);
        }
        
        void unhandled_exception() {
            exception_ = std::current_exception();
        }
    };
    
    explicit lazy(std::coroutine_handle<promise_type> h)
        : coro_(h) {}
    
    ~lazy() {
        if (coro_) {
            coro_.destroy();
        }
    }
    
    lazy(const lazy&) = delete;
    lazy& operator=(const lazy&) = delete;
    
    lazy(lazy&& other) noexcept
        : coro_(std::exchange(other.coro_, {})) {}
    
    T get() {
        if (!coro_) {
            throw std::runtime_error("Empty lazy");
        }
        
        if (!coro_.done()) {
            coro_.resume();
        }
        
        auto& promise = coro_.promise();
        if (promise.exception_) {
            std::rethrow_exception(promise.exception_);
        }
        
        return promise.value_;
    }
    
private:
    std::coroutine_handle<promise_type> coro_;
};

lazy<int> expensive_computation() {
    std::cout << "Doing expensive computation...\n";
    co_return 42;
}

// Использование lazy
auto lazy_value = expensive_computation();
std::cout << "Lazy created (вычисление НЕ началось)\n";

std::cout << "Getting value...\n";
int lazy_result = lazy_value.get();  // ЗДЕСЬ начинается выполнение
std::cout << "Result: " << lazy_result << '\n';

// ============================================
// 📌 ASYNC PRODUCER-CONSUMER
// ============================================

template<typename T>
class async_queue {
private:
    std::queue<T> queue_;
    std::queue<std::coroutine_handle<>> waiters_;
    
public:
    struct push_awaiter {
        async_queue& queue_;
        T value_;
        
        bool await_ready() const noexcept { return false; }
        
        void await_suspend(std::coroutine_handle<> handle) {
            // Если есть ожидающие, разбудим одного
            if (!queue_.waiters_.empty()) {
                auto waiter = queue_.waiters_.front();
                queue_.waiters_.pop();
                queue_.queue_.push(std::move(value_));
                waiter.resume();
            } else {
                queue_.queue_.push(std::move(value_));
            }
        }
        
        void await_resume() noexcept {}
    };
    
    struct pop_awaiter {
        async_queue& queue_;
        
        bool await_ready() const noexcept {
            return !queue_.queue_.empty();
        }
        
        void await_suspend(std::coroutine_handle<> handle) {
            queue_.waiters_.push(handle);
        }
        
        T await_resume() {
            T value = std::move(queue_.queue_.front());
            queue_.queue_.pop();
            return value;
        }
    };
    
    push_awaiter push(T value) {
        return push_awaiter{*this, std::move(value)};
    }
    
    pop_awaiter pop() {
        return pop_awaiter{*this};
    }
};

// ============================================
// 📌 SYMMETRIC TRANSFER
// ============================================

/*
 * Symmetric transfer - оптимизация для избежания переполнения стека
 * при цепочке корутин.
 * 
 * await_suspend может возвращать:
 * - void: всегда приостанавливает
 * - bool: true = приостановить, false = продолжить
 * - std::coroutine_handle<>: возобновить указанную корутину (symmetric transfer)
 */

task<int> level3() {
    std::cout << "Level 3\n";
    co_return 3;
}

task<int> level2() {
    std::cout << "Level 2\n";
    int val = co_await level3();
    co_return val + 2;
}

task<int> level1() {
    std::cout << "Level 1\n";
    int val = co_await level2();
    co_return val + 1;
}

// Использование symmetric transfer
auto sym_task = level1();
std::cout << "Result: " << sym_task.get() << '\n';

// ============================================
// 📌 ERROR HANDLING В КОРУТИНАХ
// ============================================

task<int> might_throw(bool should_throw) {
    if (should_throw) {
        throw std::runtime_error("Error in coroutine!");
    }
    co_return 42;
}

task<int> handle_errors() {
    try {
        int result = co_await might_throw(false);
        std::cout << "Success: " << result << '\n';
        
        // Это бросит exception
        result = co_await might_throw(true);
        std::cout << "Won't reach here\n";
        
        co_return result;
    } catch (const std::exception& e) {
        std::cout << "Caught in coroutine: " << e.what() << '\n';
        co_return -1;
    }
}

// Использование
auto error_task = handle_errors();
try {
    int error_result = error_task.get();
    std::cout << "Final result: " << error_result << '\n';
} catch (const std::exception& e) {
    std::cout << "Caught outside: " << e.what() << '\n';
}

// ============================================
// 📌 PERFORMANCE CONSIDERATIONS
// ============================================

/*
 * ПРОИЗВОДИТЕЛЬНОСТЬ КОРУТИН:
 * 
 * ✓ Преимущества:
 * - Легковесные (обычно меньше накладных расходов чем потоки)
 * - Кооперативная многозадачность
 * - Сохранение контекста только при необходимости
 * - Компилятор может оптимизировать
 * 
 * ✗ Недостатки:
 * - Аллокация состояния (heap allocation по умолчанию)
 * - Сложность отладки
 * - Не для CPU-bound задач (не параллелизм!)
 * 
 * ОПТИМИЗАЦИИ:
 * 
 * 1. HALO (Heap Allocation eLision Optimization)
 *    Компилятор может оптимизировать аллокацию на стек
 * 
 * 2. Кастомный аллокатор через promise_type::operator new
 * 
 * 3. Symmetric transfer вместо рекурсивных вызовов
 */

// Пример кастомного аллокатора
template<typename T>
class task_with_custom_allocator {
public:
    struct promise_type {
        // Кастомный оператор new
        void* operator new(std::size_t size) {
            std::cout << "Custom allocator: " << size << " bytes\n";
            return ::operator new(size);
        }
        
        void operator delete(void* ptr) {
            std::cout << "Custom deallocator\n";
            ::operator delete(ptr);
        }
        
        // ... остальные методы promise_type
    };
};

// ============================================
// 📌 ПРАКТИЧЕСКИЕ СОВЕТЫ
// ============================================

/*
 * КОГДА ИСПОЛЬЗОВАТЬ КОРУТИНЫ:
 * 
 * ✓ Async I/O операции
 * ✓ Generators для ленивых последовательностей
 * ✓ State machines
 * ✓ Event-driven программирование
 * ✓ Парсеры и итераторы
 * 
 * ✗ CPU-bound вычисления (используйте потоки)
 * ✗ Простые функции (overhead не оправдан)
 * ✗ Real-time системы (недетерминированность аллокаций)
 * 
 * 
 * BEST PRACTICES:
 * 
 * 1. Всегда проверяйте lifetime объектов в корутинах
 * 2. Используйте RAII для управления ресурсами
 * 3. Избегайте захвата по ссылке в лямбдах внутри корутин
 * 4. Тестируйте с sanitizers (особенно ASan)
 * 5. Документируйте suspend points
 * 6. Используйте symmetric transfer для глубоких цепочек
 */

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

/*
 * ════════════════════════════════════════════════════════════════════════════════════
 * 🎯 РЕЗЮМЕ - КОРУТИНЫ C++20
 * ════════════════════════════════════════════════════════════════════════════════════
 * 
 * КЛЮЧЕВЫЕ СЛОВА:
 * ✓ co_await  - приостановить и дождаться
 * ✓ co_yield  - вернуть значение в generator
 * ✓ co_return - завершить корутину
 * 
 * ТИПЫ КОРУТИН:
 * ✓ generator<T>  - для ленивых последовательностей (итераторы)
 * ✓ task<T>       - для async операций
 * ✓ lazy<T>       - ленивые вычисления (старт по запросу)
 * ✓ Custom awaitables - интеграция с event loops
 * 
 * ВАЖНЫЕ КОНЦЕПЦИИ:
 * ✓ Promise type        - управляет поведением корутины
 * ✓ Symmetric transfer  - избежать stack overflow
 * ✓ Awaitable interface - await_ready/suspend/resume
 * ✓ Exception handling  - unhandled_exception()
 * 
 * ПРЕИМУЩЕСТВА:
 * ✓ Ленивые вычисления (generator)
 * ✓ Асинхронный код без callback hell
 * ✓ Экономия памяти (не создаем всю последовательность)
 * ✓ Читаемый асинхронный код
 * 
 * КОМПИЛЯЦИЯ:
 * g++ -std=c++20 -fcoroutines coroutines.cpp
 * clang++ -std=c++20 -stdlib=libc++ coroutines.cpp
 */
