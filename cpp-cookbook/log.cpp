#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <format>
#include <print>  // C++23

// ---------------------------------------------------
// 📌 Вывод данных (std::print / std::println C++23)
// ---------------------------------------------------
void example_log() {
    // C++23 способ - проще и современнее
    std::println("Hello");
    
    std::vector<int> arr = {1, 2, 3};
    std::print("Array: ");
    for (int x : arr) std::print("{} ", x);
    std::println("");
    
    struct Obj { int a; std::string b; };
    Obj obj{1, "two"};
    std::println("{{ a: {}, b: {} }}", obj.a, obj.b);
    
    // Старый способ (всё ещё работает)
    std::cout << "Hello (old style)" << std::endl;
}

// ---------------------------------------------------
// 📌 Форматирование вывода (std::format C++20, std::print C++23)
// ---------------------------------------------------
void example_format() {
    std::string name = "Alice";
    int age = 30;
    
    // C++23 std::print с форматированием
    std::println("Name: {}, Age: {}", name, age);
    
    // C++20 std::format (для создания строки)
    std::string formatted = std::format("Name: {}, Age: {}", name, age);
    std::println("{}", formatted);
    
    // Старый способ с ostringstream
    std::ostringstream oss;
    oss << "Строка через ostringstream: " << name << ", " << age;
    std::println("{}", oss.str());
}

// ---------------------------------------------------
// 📌 Измерение времени выполнения
// ---------------------------------------------------
void example_timing() {
    auto start = std::chrono::high_resolution_clock::now();
    // код
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::println("Time: {} ms", elapsed.count());
}

// ---------------------------------------------------
// 📌 Отладочные принты (с std::print C++23)
// ---------------------------------------------------
void example_debug() {
    int x = 42;
    std::println("DEBUG: x = {}", x);
    
    std::vector<std::map<std::string,int>> data = { {{"key", 1}}, {{"key", 2}} };
    std::println("Table:");
    for (size_t i = 0; i < data.size(); ++i) {
        std::print("{}: ", i);
        for (const auto& kv : data[i]) {
            std::print("{}={} ", kv.first, kv.second);
        }
        std::println("");
    }
}

// ---------------------------------------------------
// 📌 Тестовые данные и генерация input'ов
// ---------------------------------------------------
int getRandomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

void example_test_data() {
    std::vector<int> arr(100);
    std::generate(arr.begin(), arr.end(), []() { return getRandomInt(1, 1000); });
    
    std::string str;
    for (int i = 0; i < 10; ++i)
        str += static_cast<char>(getRandomInt(97, 122));
    std::println("Random string: {}", str);
}

// ---------------------------------------------------
// 📌 Запись логов в файл
// ---------------------------------------------------
void example_file_log() {
    std::ofstream logFile("app.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << std::format("[{}] {}\n", std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), "Начало работы приложения");
        logFile.close();
    }
}
