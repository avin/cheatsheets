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

// ---------------------------------------------------
// 📌 Вывод данных (print / log)
// ---------------------------------------------------
void example_log() {
    std::cout << "Hello" << std::endl;
    std::vector<int> arr = {1, 2, 3};
    for (int x : arr) std::cout << x << " ";
    std::cout << std::endl;
    struct Obj { int a; std::string b; };
    Obj obj{1, "two"};
    std::cout << "{ a: " << obj.a << ", b: " << obj.b << " }" << std::endl;
}

// ---------------------------------------------------
// 📌 Форматирование вывода (std::format, std::ostringstream)
// ---------------------------------------------------
void example_format() {
    std::string name = "Alice";
    int age = 30;
    std::cout << std::format("Name: {}, Age: {}\n", name, age);

    std::ostringstream oss;
    oss << "Строка через ostringstream: " << name << ", " << age;
    std::cout << oss.str() << std::endl;
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
    std::cout << "Time: " << elapsed.count() << " ms" << std::endl;
}

// ---------------------------------------------------
// 📌 Отладочные принты
// ---------------------------------------------------
void example_debug() {
    int x = 42;
    std::cout << "DEBUG: x = " << x << std::endl;
    std::vector<std::map<std::string,int>> data = { {{"key", 1}}, {{"key", 2}} };
    std::cout << "Table:" << std::endl;
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << i << ": ";
        for (const auto& kv : data[i]) {
            std::cout << kv.first << "=" << kv.second << " ";
        }
        std::cout << std::endl;
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
    std::cout << "Random string: " << str << std::endl;
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