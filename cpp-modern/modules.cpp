/*
 * ============================================
 * C++20 МОДУЛИ - ПОЛНОЕ РУКОВОДСТВО
 * ============================================
 * 
 * ВАЖНО: Этот файл демонстрирует концепции модулей.
 * Он НЕ компилируется - это чисто учебный материал,
 * показывающий КАК писать модули в разных файлах.
 * 
 * Темы:
 * - Синтаксис модулей (export module, import)
 * - Partitions (разделы модулей)
 * - Header units
 * - Миграция с заголовочных файлов
 * - CMake интеграция
 * - Производительность
 * 
 * Компиляция модулей:
 *   g++ -std=c++20 -fmodules-ts -xc++-module module_file.cpp
 *   clang++ -std=c++20 -fmodules module_file.cpp
 *   cl /std:c++20 /experimental:module module_file.cpp
 */


// ============================================
// 📌 БАЗОВЫЙ СИНТАКСИС МОДУЛЕЙ
// ============================================

// ========== math.cppm (module interface) ==========
// Объявление модуля
export module math;

// Экспортируемые функции
export int add(int a, int b) {
    return a + b;
}

export int multiply(int a, int b) {
    return a * b;
}

// Приватная функция (не экспортируется)
int helper_function() {
    return 42;
}

// Экспортируемый класс
export class Calculator {
public:
    int calculate(int x, int y) {
        return x + y;
    }
};


// ========== main.cpp (использование модуля) ==========
import math;  // Импортируем модуль

#include <iostream>

int main() {
    std::cout << "2 + 3 = " << add(2, 3) << '\n';
    std::cout << "4 * 5 = " << multiply(4, 5) << '\n';
    
    Calculator calc;
    std::cout << "calc(10, 20) = " << calc.calculate(10, 20) << '\n';
    
    // helper_function();  // ❌ Ошибка - не экспортирована
}


// ========== Компиляция ==========
// 1. Компилируем модуль:
//    g++ -std=c++20 -fmodules-ts -xc++-module math.cppm -o math.o
//
// 2. Компилируем main:
//    g++ -std=c++20 -fmodules-ts main.cpp math.o
//
// MSVC:
//    cl /std:c++20 /experimental:module /c math.cppm
//    cl /std:c++20 /experimental:module main.cpp math.obj


// ============================================
// 📌 GLOBAL MODULE FRAGMENT
// ============================================

// ========== utils.cppm ==========
// Global module fragment - для legacy #include
module;

#include <iostream>
#include <string>
#include <vector>

export module utils;

// Теперь можем использовать std::string и другие типы
export void print_vector(const std::vector<int>& vec) {
    for (int x : vec) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

export std::string get_greeting(const std::string& name) {
    return "Hello, " + name + "!";
}


// ========== Использование ==========
import utils;
import <vector>;  // C++23: импорт стандартной библиотеки

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    print_vector(numbers);
    
    auto msg = get_greeting("World");
    std::cout << msg << '\n';
}


// ВАЖНО:
// • module; должен быть ПЕРВОЙ строкой
// • После него идут #include
// • Затем export module name;
// • Это для совместимости с legacy кодом


// ============================================
// 📌 PRIVATE MODULE FRAGMENT
// ============================================

// ========== calculator.cppm ==========
export module calculator;

// Публичный интерфейс
export class Calculator {
public:
    int add(int a, int b);
    int subtract(int a, int b);
    
private:
    int internal_state = 0;
};

// Private module fragment - реализация
module :private;

int Calculator::add(int a, int b) {
    internal_state++;  // Приватная логика
    return a + b;
}

int Calculator::subtract(int a, int b) {
    internal_state--;
    return a - b;
}

// Приватные helper функции
static int validate(int x) {
    return x > 0 ? x : 0;
}


// ПРЕИМУЩЕСТВА:
// • Всё в одном файле (interface + implementation)
// • Приватная часть не видна пользователям
// • Компилируется как единый модуль
// • Нет необходимости в .cpp файле


// ============================================
// 📌 MODULE PARTITIONS (Разделы)
// ============================================

// ========== geometry.shapes.cppm (partition) ==========
export module geometry:shapes;

export class Circle {
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const { return 3.14159 * radius * radius; }
};

export class Rectangle {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() const { return width * height; }
};


// ========== geometry.math.cppm (partition) ==========
export module geometry:math;

export constexpr double PI = 3.14159265359;

export double square(double x) {
    return x * x;
}


// ========== geometry.cppm (primary module interface) ==========
export module geometry;

// Экспортируем разделы
export import :shapes;
export import :math;

// Дополнительные функции модуля
export void print_info() {
    std::cout << "Geometry module v1.0\n";
}


// ========== main.cpp ==========
import geometry;  // Импортируем весь модуль

int main() {
    Circle c(5.0);
    Rectangle r(3.0, 4.0);
    
    std::cout << "Circle area: " << c.area() << '\n';
    std::cout << "Rectangle area: " << r.area() << '\n';
    std::cout << "PI = " << PI << '\n';
    
    print_info();
}


// СТРУКТУРА ПРОЕКТА:
// geometry/
//   ├── geometry.cppm          (основной интерфейс)
//   ├── geometry.shapes.cppm   (partition: shapes)
//   └── geometry.math.cppm     (partition: math)


// ВАЖНО:
// • Partition имя: module:partition_name
// • Импорт partition: import :partition_name;
// • Export partition: export import :partition_name;
// • Partitions видны только внутри модуля


// ============================================
// 📌 HEADER UNITS
// ============================================

// ========== Импорт стандартных заголовков ==========
import <iostream>;   // Вместо #include <iostream>
import <vector>;
import <string>;
import <algorithm>;

export module my_module;

export void process_data() {
    std::vector<int> vec = {3, 1, 4, 1, 5};
    std::sort(vec.begin(), vec.end());
    
    for (int x : vec) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}


// ========== Импорт legacy заголовков ==========
// legacy_math.h:
// #ifndef LEGACY_MATH_H
// #define LEGACY_MATH_H
// 
// inline int legacy_add(int a, int b) {
//     return a + b;
// }
// 
// #endif

import "legacy_math.h";  // Импорт как header unit

export module wrapper;

export int use_legacy(int x, int y) {
    return legacy_add(x, y);
}


// ПРЕИМУЩЕСТВА HEADER UNITS:
// ✓ Быстрее компиляция (header precompiled)
// ✓ Изоляция макросов
// ✓ Совместимость с legacy кодом
// ✓ Постепенная миграция на модули

// КОМПИЛЯЦИЯ:
// g++ -std=c++20 -fmodules-ts -xc++-system-header iostream
// clang++ -std=c++20 --precompile -xc++-module <iostream>


// ============================================
// 📌 МИГРАЦИЯ С ЗАГОЛОВОЧНЫХ ФАЙЛОВ
// ============================================

// ========== ШАГ 1: Старый код (headers) ==========

// string_utils.h
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& str, char delim);
std::string join(const std::vector<std::string>& parts, const std::string& sep);

#endif

// string_utils.cpp
#include "string_utils.h"

std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delim);
    
    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delim, start);
    }
    result.push_back(str.substr(start));
    return result;
}

std::string join(const std::vector<std::string>& parts, const std::string& sep) {
    if (parts.empty()) return "";
    
    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += sep + parts[i];
    }
    return result;
}


// ========== ШАГ 2: Миграция на модули ==========

// string_utils.cppm
module;

#include <string>
#include <vector>

export module string_utils;

export std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delim);
    
    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delim, start);
    }
    result.push_back(str.substr(start));
    return result;
}

export std::string join(const std::vector<std::string>& parts, 
                        const std::string& sep) {
    if (parts.empty()) return "";
    
    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += sep + parts[i];
    }
    return result;
}


// ========== ШАГ 3: Использование ==========

// Старый способ
#include "string_utils.h"

// Новый способ
import string_utils;

int main() {
    auto parts = split("a,b,c", ',');
    auto joined = join(parts, " | ");
}


// СТРАТЕГИЯ МИГРАЦИИ:
// 1. Начните с листовых модулей (без зависимостей)
// 2. Постепенно конвертируйте заголовки в модули
// 3. Используйте header units для legacy кода
// 4. Обновите build систему (CMake 3.28+)
// 5. Тестируйте каждый шаг

// СОВМЕСТИМОСТЬ:
// • Модули и headers могут сосуществовать
// • Модуль может использовать headers через module;
// • Headers могут импортировать модули (осторожно!)


// ============================================
// 📌 ПРИМЕРЫ РЕАЛЬНЫХ МОДУЛЕЙ
// ============================================

// ========== Модуль логирования ==========

// logger.cppm
export module logger;

import <iostream>;
import <string>;
import <chrono>;
import <format>;

export enum class LogLevel {
    DEBUG, INFO, WARNING, ERROR
};

export class Logger {
public:
    void log(LogLevel level, const std::string& message) {
        auto now = std::chrono::system_clock::now();
        std::cout << std::format("[{}] {}: {}\n", 
                                  now, level_to_string(level), message);
    }
    
    void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
    void info(const std::string& msg) { log(LogLevel::INFO, msg); }
    void warning(const std::string& msg) { log(LogLevel::WARNING, msg); }
    void error(const std::string& msg) { log(LogLevel::ERROR, msg); }
    
private:
    const char* level_to_string(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
        }
        return "UNKNOWN";
    }
};


// ========== HTTP клиент модуль ==========

// http_client.cppm
export module http_client;

import <string>;
import <map>;
import <memory>;

export class HttpClient {
public:
    struct Response {
        int status_code;
        std::string body;
        std::map<std::string, std::string> headers;
    };
    
    HttpClient(const std::string& base_url) : base_url_(base_url) {}
    
    Response get(const std::string& path);
    Response post(const std::string& path, const std::string& data);
    Response put(const std::string& path, const std::string& data);
    Response del(const std::string& path);
    
    void set_header(const std::string& key, const std::string& value);
    void set_timeout(int milliseconds);
    
private:
    std::string base_url_;
    std::map<std::string, std::string> default_headers_;
    int timeout_ms_ = 30000;
};


// ========== JSON парсер модуль ==========

// json.cppm
export module json;

import <string>;
import <variant>;
import <map>;
import <vector>;
import <stdexcept>;

export class JsonValue {
public:
    using Null = std::monostate;
    using Boolean = bool;
    using Number = double;
    using String = std::string;
    using Array = std::vector<JsonValue>;
    using Object = std::map<std::string, JsonValue>;
    
    JsonValue() = default;
    JsonValue(bool value) : data_(value) {}
    JsonValue(int value) : data_(static_cast<double>(value)) {}
    JsonValue(double value) : data_(value) {}
    JsonValue(const std::string& value) : data_(value) {}
    JsonValue(const char* value) : data_(std::string(value)) {}
    
    static JsonValue parse(const std::string& json_str);
    std::string stringify() const;
    
    // Accessors
    bool is_null() const { return std::holds_alternative<Null>(data_); }
    bool is_bool() const { return std::holds_alternative<Boolean>(data_); }
    bool is_number() const { return std::holds_alternative<Number>(data_); }
    bool is_string() const { return std::holds_alternative<String>(data_); }
    bool is_array() const { return std::holds_alternative<Array>(data_); }
    bool is_object() const { return std::holds_alternative<Object>(data_); }
    
    bool as_bool() const { return std::get<Boolean>(data_); }
    double as_number() const { return std::get<Number>(data_); }
    const std::string& as_string() const { return std::get<String>(data_); }
    const Array& as_array() const { return std::get<Array>(data_); }
    const Object& as_object() const { return std::get<Object>(data_); }
    
private:
    std::variant<Null, Boolean, Number, String, Array, Object> data_;
};


// ========== Использование всех модулей ==========

import logger;
import http_client;
import json;

int main() {
    Logger log;
    log.info("Application started");
    
    HttpClient client("https://api.example.com");
    client.set_header("Authorization", "Bearer token123");
    
    auto response = client.get("/users");
    log.info("Received response with status: " + std::to_string(response.status_code));
    
    auto json_data = JsonValue::parse(response.body);
    if (json_data.is_array()) {
        log.info("Received " + std::to_string(json_data.as_array().size()) + " users");
    }
    
    return 0;
}


// ============================================
// 📌 CMAKE ИНТЕГРАЦИЯ
// ============================================

// ========== CMakeLists.txt (минимальный) ==========

/*
cmake_minimum_required(VERSION 3.28)  # Требуется 3.28+
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем экспериментальную поддержку модулей
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

# Библиотека с модулями
add_library(math_module)
target_sources(math_module
  PUBLIC
    FILE_SET CXX_MODULES FILES
      math.cppm
      geometry.cppm
      geometry.shapes.cppm
)

# Исполняемый файл
add_executable(app main.cpp)
target_link_libraries(app PRIVATE math_module)
*/


// ========== CMakeLists.txt (продвинутый) ==========

/*
cmake_minimum_required(VERSION 3.28)
project(AdvancedProject CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")

# Опции для разных компиляторов
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/experimental:module)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fmodules)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-fmodules-ts)
endif()

# Модульная библиотека
add_library(core_modules)
target_sources(core_modules
  PUBLIC
    FILE_SET CXX_MODULES FILES
      core/logger.cppm
      core/config.cppm
      utils/string_utils.cppm
      utils/file_utils.cppm
)

# Зависимости между модулями обрабатываются автоматически
add_library(network_modules)
target_sources(network_modules
  PUBLIC
    FILE_SET CXX_MODULES FILES
      network/http_client.cppm
      network/websocket.cppm
)
target_link_libraries(network_modules PRIVATE core_modules)

# Приложение
add_executable(my_app src/main.cpp)
target_link_libraries(my_app 
  PRIVATE 
    core_modules
    network_modules
)
*/


// ========== Структура проекта ==========

/*
project/
├── CMakeLists.txt
├── core/
│   ├── logger.cppm
│   └── config.cppm
├── utils/
│   ├── string_utils.cppm
│   └── file_utils.cppm
├── network/
│   ├── http_client.cppm
│   └── websocket.cppm
└── src/
    └── main.cpp
*/


// ========== Сборка ==========

/*
mkdir build && cd build
cmake ..
cmake --build .

# Или с Ninja (быстрее)
cmake -G Ninja ..
ninja
*/


// ВАЖНО:
// • CMake 3.28+ обязателен
// • FILE_SET CXX_MODULES для модулей
// • CMake автоматически определяет зависимости
// • Порядок компиляции модулей определяется автоматически


// ============================================
// 📌 ПРОИЗВОДИТЕЛЬНОСТЬ
// ============================================

// СРАВНЕНИЕ: HEADERS vs MODULES

// ========== С заголовочными файлами ==========
//
// Проект: 100 файлов, каждый включает <iostream>, <vector>, <string>
//
// Время компиляции:
//   Clean build:      120 секунд
//   Incremental:      45 секунд
//   Total includes:   100 * 3 = 300 раз парсятся STL заголовки


// ========== С модулями ==========
//
// import <iostream>;  // Компилируется ОДИН раз в BMI
// import <vector>;    // Binary Module Interface
// import <string>;
//
// Время компиляции:
//   Clean build:      45 секунд  (на 62% быстрее!)
//   Incremental:      5 секунд   (на 89% быстрее!)
//   Total includes:   0 (модули не "включаются")


// ПОЧЕМУ МОДУЛИ БЫСТРЕЕ:
//
// 1. Binary Module Interface (BMI):
//    • Модуль компилируется в бинарный формат
//    • При импорте загружается BMI, а не парсится исходник
//    • BMI создаётся один раз, используется многократно
//
// 2. Отсутствие дублирования:
//    Headers:  каждый .cpp файл парсит все #include
//    Modules:  каждый модуль компилируется ровно один раз
//
// 3. Изоляция:
//    • Изменение приватной части модуля не требует 
//      перекомпиляции пользователей
//    • Headers: изменение .h → перекомпиляция всех .cpp
//
// 4. Параллелизм:
//    • Модули с независимыми зависимостями компилируются параллельно
//    • CMake/Ninja автоматически определяют DAG зависимостей


// РЕАЛЬНЫЕ ЦИФРЫ (LLVM проект):
//
// Headers:
//   - Время clean build: ~45 минут
//   - Размер объектных файлов: 8 GB
//
// Modules:
//   - Время clean build: ~15 минут (3x быстрее!)
//   - Размер BMI: 1.2 GB (меньше!)


// ЛУЧШИЕ ПРАКТИКИ ДЛЯ ПРОИЗВОДИТЕЛЬНОСТИ:
//
// ✓ Используйте модули для больших библиотек
// ✓ Импортируйте стандартную библиотеку как модули
// ✓ Разделяйте большие модули на partitions
// ✓ Минимизируйте зависимости между модулями
// ✓ Кешируйте BMI файлы в CI/CD


// ============================================
// 📌 BEST PRACTICES
// ============================================

// 1. ИМЕНОВАНИЕ МОДУЛЕЙ:
//    ✓ module_name (snake_case для простых)
//    ✓ company.project.component (точечная нотация)
//    ✓ std, std.core (стандартная библиотека)
//    ✗ ModuleName (не CamelCase)

// 2. СТРУКТУРА ФАЙЛОВ:
//    ✓ .cppm или .ixx расширение для module interface
//    ✓ Один модуль = один файл (или с partitions)
//    ✓ Группируйте связанные partitions в папке

// 3. ЭКСПОРТ:
//    ✓ Экспортируйте только публичный API
//    ✓ Используйте export class/enum/namespace
//    ✓ Приватные детали в module :private;

// 4. ИМПОРТ:
//    ✓ import module; в начале файла
//    ✓ Импортируйте то, что используете
//    ✗ Не делайте export import без нужды

// 5. ЗАВИСИМОСТИ:
//    ✓ Минимизируйте зависимости между модулями
//    ✓ Используйте forward declarations где возможно
//    ✗ Избегайте циклических зависимостей

// 6. МИГРАЦИЯ:
//    ✓ Начните с новых компонентов
//    ✓ Используйте header units для legacy
//    ✓ Постепенно конвертируйте листовые модули

// 7. TOOLING:
//    ✓ CMake 3.28+ для build системы
//    ✓ clang-format для форматирования
//    ✓ clangd/IntelliSense для автодополнения


// ПРИМЕРЫ:

// ✓ ХОРОШО: Чистый интерфейс
export module math;

export namespace math {
    int add(int a, int b);
    int multiply(int a, int b);
}


// ✗ ПЛОХО: Экспорт всего подряд
export module bad_example;

export {
    #include <iostream>  // ❌ Не экспортируйте STL!
    using namespace std;  // ❌ Никогда!
    
    void some_function();
    void internal_helper();  // ❌ Приватная функция!
}


// ✓ ХОРОШО: Partition для организации
export module graphics:shapes;  // partition
export module graphics:colors;  // partition
export module graphics;          // primary interface

export import :shapes;
export import :colors;


// КОМПИЛЯТОРЫ (2024-2025):
//
// ✓ MSVC 19.28+    (отличная поддержка)
// ✓ Clang 16+      (хорошая поддержка)
// ⚠ GCC 14+        (экспериментальная, улучшается)
//
// Проверяйте документацию компилятора!


// ============================================
// 📌 ЧАСТЫЕ ОШИБКИ
// ============================================

// ОШИБКА 1: Экспорт макросов
// ❌ НЕ РАБОТАЕТ
export module config;

export #define MAX_SIZE 1000  // ❌ Макросы не экспортируются!

// ✓ ПРАВИЛЬНО
export module config;

export constexpr int MAX_SIZE = 1000;  // ✓ Используй constexpr


// ОШИБКА 2: Циклические зависимости
// ❌ module A imports B, module B imports A
export module A;
import B;  // ❌ Цикл!

export void func_a();

// ✓ ПРАВИЛЬНО: Разбейте на 3 модуля
export module common;
export void common_func();

export module A;
import common;

export module B;
import common;


// ОШИБКА 3: Забыли module; перед #include
// ❌ НЕПРАВИЛЬНО
export module utils;

#include <iostream>  // ❌ Ошибка!

// ✓ ПРАВИЛЬНО
module;

#include <iostream>  // ✓ В global fragment

export module utils;


// ОШИБКА 4: Конфликт имён
// ❌ Два модуля с одинаковым именем
export module utils;  // file1.cppm
export module utils;  // file2.cppm  ❌ Конфликт!

// ✓ Используйте пространства имён
export module company.utils;
export module myproject.utils;


// ОШИБКА 5: Неправильный порядок компиляции
// ❌ Компиляция main.cpp до модуля
// g++ main.cpp         # ❌ import math; - модуль не найден!
// g++ math.cppm

// ✓ Сначала модуль, потом использование
// g++ -std=c++20 -fmodules-ts -xc++-module math.cppm
// g++ -std=c++20 -fmodules-ts main.cpp math.o


// ОШИБКА 6: Export using namespace
// ❌ ПЛОХАЯ ПРАКТИКА
export module bad;

export using namespace std;  // ❌ Загрязняет namespace!

// ✓ Экспортируйте конкретные сущности
export module good;

export std::vector<int> get_data();


// ОШИБКА 7: Смешивание export и module :private;
export module example;

export void public_func();

module :private;

export void another_func();  // ❌ Нельзя export в private!

// ✓ Всё export должно быть до module :private;


// ============================================
// 📌 РЕЗЮМЕ
// ============================================

// ✓ Модули заменяют заголовочные файлы
// ✓ export module name; объявляет модуль
// ✓ import module_name; импортирует модуль
// ✓ Partitions для организации больших модулей
// ✓ Header units для совместимости с legacy
// ✓ Binary Module Interface (BMI) ускоряет компиляцию
// ✓ CMake 3.28+ с FILE_SET CXX_MODULES
// ✓ До 10x быстрее компиляция в больших проектах

// КОМПИЛЯТОРЫ:
//   MSVC 19.28+  ✓✓✓ (отлично)
//   Clang 16+    ✓✓  (хорошо)
//   GCC 14+      ✓   (экспериментально)

// РЕСУРСЫ:
//   • https://en.cppreference.com/w/cpp/language/modules
//   • https://clang.llvm.org/docs/StandardCPlusPlusModules.html
//   • https://learn.microsoft.com/en-us/cpp/cpp/modules-cpp