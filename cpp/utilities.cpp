#include <iostream>
#include <fstream>
#include <memory>
#include <exception>
#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <filesystem>
#include <thread>
#include <mutex>
#include <chrono>
#include <expected>     // C++23
#include <stacktrace>   // C++23
#include <utility>      // std::unreachable C++23
#include <print>        // C++23

// ---------------------------------------------------
// 📌 Обработка ошибок с std::expected (C++23)
// ---------------------------------------------------
enum class ParseError {
    InvalidFormat,
    OutOfRange,
    EmptyString
};

std::expected<int, ParseError> parseInteger(const std::string& str) {
    if (str.empty()) {
        return std::unexpected(ParseError::EmptyString);
    }
    
    try {
        int value = std::stoi(str);
        if (value < 0 || value > 100) {
            return std::unexpected(ParseError::OutOfRange);
        }
        return value;
    } catch (...) {
        return std::unexpected(ParseError::InvalidFormat);
    }
}

void example_expected() {
    auto result = parseInteger("42");
    
    if (result) {
        // Успешный результат
        std::println("Parsed value: {}", *result);
    } else {
        // Обработка ошибки
        switch (result.error()) {
            case ParseError::InvalidFormat:
                std::println("Error: Invalid format");
                break;
            case ParseError::OutOfRange:
                std::println("Error: Value out of range");
                break;
            case ParseError::EmptyString:
                std::println("Error: Empty string");
                break;
        }
    }
    
    // Использование value_or для значения по умолчанию
    int value = parseInteger("invalid").value_or(0);
    
    // Трансформация результата
    auto doubled = parseInteger("10").transform([](int x) { return x * 2; });
}

// ---------------------------------------------------
// 📌 Обработка исключений (классический подход)
// ---------------------------------------------------
void example_exceptions() {
    try {
        throw std::runtime_error("Ошибка!");
    } catch (const std::runtime_error& e) {
        std::println("Отловлена ошибка std::runtime_error: {}", e.what());
    } catch (const std::exception& e) {
        std::println("Отловлена стандартная ошибка: {}", e.what());
    } catch (...) {
        std::println("Неизвестная ошибка");
    }
}

// ---------------------------------------------------
// 📌 Stack trace (C++23) - получение стека вызовов
// ---------------------------------------------------
void function_c() {
    // Получение текущего stack trace
    auto trace = std::stacktrace::current();
    
    std::println("Stack trace:");
    for (const auto& entry : trace) {
        std::println("  {}", entry);
    }
    
    // Пропуск первых N фреймов
    auto trace_skip = std::stacktrace::current(2);  // пропустить 2 фрейма
    
    // Ограничение количества фреймов
    auto trace_limit = std::stacktrace::current(0, 5);  // максимум 5 фреймов
}

void function_b() {
    function_c();
}

void function_a() {
    function_b();
}

void example_stacktrace() {
    function_a();
}

// ---------------------------------------------------
// 📌 std::unreachable() (C++23) - hint для компилятора
// ---------------------------------------------------
enum class Color { Red, Green, Blue };

int getColorValue(Color c) {
    switch (c) {
        case Color::Red:   return 0xFF0000;
        case Color::Green: return 0x00FF00;
        case Color::Blue:  return 0x0000FF;
    }
    
    // Сообщаем компилятору, что этот код недостижим
    // Улучшает оптимизацию и предотвращает предупреждения
    std::unreachable();
}

// Пример с assert-подобным использованием
int divide(int a, int b) {
    if (b == 0) {
        std::println("Error: Division by zero!");
        std::unreachable();  // никогда не должно быть достигнуто
    }
    return a / b;
}

// ---------------------------------------------------
// 📌 Работа с файлами (ifstream, ofstream)
// ---------------------------------------------------
void example_file_io() {
    // Запись в файл
    std::ofstream outFile("output.txt");
    if (outFile.is_open()) {
        std::println(outFile, "Пример записи в файл");  // C++23 print в файл
        outFile.close();
    }

    // Чтение из файла
    std::ifstream inFile("input.txt");
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::println("{}", line);
        }
        inFile.close();
    }
}


// ---------------------------------------------------
// 📌 Умные указатели (std::unique_ptr, std::shared_ptr, std::weak_ptr)
// ---------------------------------------------------
struct Node {
    int value;
    std::unique_ptr<Node> next;
    Node(int v) : value(v), next(nullptr) {}
};

void example_smart_pointers() {
    // std::unique_ptr
    std::unique_ptr<Node> head = std::make_unique<Node>(1);
    head->next = std::make_unique<Node>(2);

    // std::shared_ptr и std::weak_ptr
    std::shared_ptr<int> sp = std::make_shared<int>(10);
    std::weak_ptr<int> wp = sp;
    if (auto locked = wp.lock()) {
        std::println("Значение: {}", *locked);
    }
}


// ---------------------------------------------------
// 📌 Лямбда-выражения
// ---------------------------------------------------
void example_lambda() {
    std::vector<int> data = {1, 2, 3, 4, 5};
    int multiplier = 10;
    std::for_each(data.begin(), data.end(), [multiplier](int x) {
        std::print("{} ", x * multiplier);
    });
    std::println("");
}


// ---------------------------------------------------
// 📌 Многопоточность (std::thread, std::mutex)
// ---------------------------------------------------
std::mutex mtx;

void thread_function(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    std::println("Поток {} выполняется", id);
}

void example_multithreading() {
    std::thread t1(thread_function, 1);
    std::thread t2(thread_function, 2);
    t1.join();
    t2.join();
}


// ---------------------------------------------------
// 📌 std::optional и std::variant
// ---------------------------------------------------
std::optional<int> example_optional(bool flag) {
    if (flag) {
        return 42;
    }
    return std::nullopt;
}

void example_variant() {
    std::variant<int, std::string> var;
    var = 100;
    if (std::holds_alternative<int>(var)) {
        std::println("Целое: {}", std::get<int>(var));
    }
    var = std::string("Текст");
    if (std::holds_alternative<std::string>(var)) {
        std::println("Строка: {}", std::get<std::string>(var));
    }
}


// ---------------------------------------------------
// 📌 Работа с файловой системой (std::filesystem)
// ---------------------------------------------------
void example_filesystem() {
    std::filesystem::path p = std::filesystem::current_path();
    std::println("Текущая директория: {}", p.string());

    std::filesystem::path newDir = p / "test_dir";
    if (!std::filesystem::exists(newDir)) {
        std::filesystem::create_directory(newDir);
    }

    for (const auto& entry : std::filesystem::directory_iterator(p)) {
        std::println("{}", entry.path().string());
    }
}


// ---------------------------------------------------
// 📌 Чтение аргументов командной строки
// ---------------------------------------------------
int example_main_with_args(int argc, char* argv[]) {
    std::println("Количество аргументов: {}", argc);
    for (int i = 0; i < argc; ++i) {
        std::println("argv[{}] = {}", i, argv[i]);
    }
    return 0;
}


// ---------------------------------------------------
// 📌 Измерение даты и времени (std::chrono)
// ---------------------------------------------------
void example_chrono_time() {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::println("Текущее время: {}", std::ctime(&timeT));
}
