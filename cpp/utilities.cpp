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

// ---------------------------------------------------
// 📌 Обработка исключений
// ---------------------------------------------------
void example_exceptions() {
    try {
        throw std::runtime_error("Ошибка!");
    } catch (const std::runtime_error& e) {
        std::cerr << "Отловлена ошибка std::runtime_error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Отловлена стандартная ошибка: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Неизвестная ошибка" << std::endl;
    }
}


// ---------------------------------------------------
// 📌 Работа с файлами (ifstream, ofstream)
// ---------------------------------------------------
void example_file_io() {
    // Запись в файл
    std::ofstream outFile("output.txt");
    if (outFile.is_open()) {
        outFile << "Пример записи в файл\n";
        outFile.close();
    }

    // Чтение из файла
    std::ifstream inFile("input.txt");
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::cout << line << std::endl;
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
        std::cout << "Значение: " << *locked << std::endl;
    }
}


// ---------------------------------------------------
// 📌 Лямбда-выражения
// ---------------------------------------------------
void example_lambda() {
    std::vector<int> data = {1, 2, 3, 4, 5};
    int multiplier = 10;
    std::for_each(data.begin(), data.end(), [multiplier](int x) {
        std::cout << x * multiplier << " ";
    });
    std::cout << std::endl;
}


// ---------------------------------------------------
// 📌 Многопоточность (std::thread, std::mutex)
// ---------------------------------------------------
std::mutex mtx;

void thread_function(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Поток " << id << " выполняется" << std::endl;
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
        std::cout << "Целое: " << std::get<int>(var) << std::endl;
    }
    var = std::string("Текст");
    if (std::holds_alternative<std::string>(var)) {
        std::cout << "Строка: " << std::get<std::string>(var) << std::endl;
    }
}


// ---------------------------------------------------
// 📌 Работа с файловой системой (std::filesystem)
// ---------------------------------------------------
void example_filesystem() {
    std::filesystem::path p = std::filesystem::current_path();
    std::cout << "Текущая директория: " << p << std::endl;

    std::filesystem::path newDir = p / "test_dir";
    if (!std::filesystem::exists(newDir)) {
        std::filesystem::create_directory(newDir);
    }

    for (const auto& entry : std::filesystem::directory_iterator(p)) {
        std::cout << entry.path() << std::endl;
    }
}


// ---------------------------------------------------
// 📌 Чтение аргументов командной строки
// ---------------------------------------------------
int example_main_with_args(int argc, char* argv[]) {
    std::cout << "Количество аргументов: " << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }
    return 0;
}


// ---------------------------------------------------
// 📌 Измерение даты и времени (std::chrono)
// ---------------------------------------------------
void example_chrono_time() {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::cout << "Текущее время: " << std::ctime(&timeT);
}