#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <atomic>
#include <future>
#include <chrono>
#include <print>  // C++23

// ---------------------------------------------------
// 📌 Создание и запуск потоков (std::thread)
// ---------------------------------------------------
void example_thread_basic() {
    auto func = []() {
        std::println("Привет из потока!");
    };
    std::thread t(func);
    t.join();
}

// ---------------------------------------------------
// 📌 Детач и джойн потоков
// ---------------------------------------------------
void example_thread_join_detach() {
    // Пример join
    std::thread t1([]() {
        std::cout << "Поток t1 выполняется" << std::endl;
    });
    if (t1.joinable()) {
        t1.join();
    }

    // Пример detach
    std::thread t2([]() {
        std::cout << "Поток t2 (detached) выполняется" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Поток t2 завершился" << std::endl;
    });
    t2.detach();
    // Главное приложение может завершиться раньше detached-потока
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

// ---------------------------------------------------
// 📌 std::mutex и std::lock_guard
// ---------------------------------------------------
void example_mutex_lock_guard() {
    std::mutex mtx;
    int counter = 0;
    auto increment = [&](int id) {
        for (int i = 0; i < 1000; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            ++counter;
        }
        std::println("Поток {} закончил", id);
    };

    std::thread t1(increment, 1);
    std::thread t2(increment, 2);
    t1.join();
    t2.join();
    std::println("Итоговый счетчик: {}", counter);
}

// ---------------------------------------------------
// 📌 std::unique_lock и std::condition_variable
// ---------------------------------------------------
void example_condition_variable() {
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;
    int data = 0;

    auto producer = [&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            std::lock_guard<std::mutex> lock(mtx);
            data = 42;
            ready = true;
            std::cout << "Производитель: данные готовы" << std::endl;
        }
        cv.notify_one();
    };

    auto consumer = [&]() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return ready; });
        std::cout << "Потребитель: получил данные = " << data << std::endl;
    };

    std::thread prod(producer);
    std::thread cons(consumer);
    prod.join();
    cons.join();
}

// ---------------------------------------------------
// 📌 std::atomic для безопасного доступа к переменным
// ---------------------------------------------------
void example_atomic() {
    std::atomic<int> atomic_counter{0};
    auto increment = [&]() {
        for (int i = 0; i < 1000; ++i) {
            atomic_counter.fetch_add(1, std::memory_order_relaxed);
        }
    };

    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();
    std::cout << "Atomic счетчик: " << atomic_counter.load() << std::endl;
}

// ---------------------------------------------------
// 📌 std::async и std::future
// ---------------------------------------------------
int compute_heavy(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return x * x;
}

void example_async_future() {
    std::future<int> fut = std::async(std::launch::async, compute_heavy, 5);
    std::cout << "Ожидание результата..." << std::endl;
    int result = fut.get();
    std::cout << "Результат: " << result << std::endl;
}
