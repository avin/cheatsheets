/*
 * ============================================
 * C++20 МОДУЛИ - РУКОВОДСТВО
 * ============================================
 * 
 * Полное руководство по C++20 модулям.
 * 
 * ВАЖНО: Этот файл содержит примеры в комментариях,
 * так как модули требуют специальной организации файлов.
 * 
 * Требования: C++20, компилятор с поддержкой модулей
 * Компиляция: g++ -std=c++20 -fmodules-ts (экспериментально)
 *             clang++ -std=c++20 -fmodules (лучше поддержка)
 *             msvc /std:c++20 (хорошая поддержка)
 */

#include <iostream>
#include <string>
#include <vector>

/*
 * ============================================
 * 📌 MODULE BASICS
 * ============================================
 * 
 * Модули заменяют традиционные header файлы и решают проблемы:
 * - Медленной компиляции
 * - ODR (One Definition Rule) нарушений
 * - Макросов загрязняющих глобальное пространство
 * - Порядка include директив
 * 
 * СИНТАКСИС:
 * 
 * // math_module.cppm (module interface file)
 * export module math;  // Объявление модуля
 * 
 * export int add(int a, int b) {  // Экспортируемая функция
 *     return a + b;
 * }
 * 
 * int helper() {  // Приватная функция (не экспортируется)
 *     return 42;
 * }
 * 
 * 
 * // main.cpp
 * import math;  // Импорт модуля
 * 
 * int main() {
 *     int result = add(5, 10);  // Можем использовать экспортируемые функции
 *     // int h = helper();       // ОШИБКА: helper не экспортирован
 * }
 * 
 * 
 * ПРЕИМУЩЕСТВА:
 * ✓ Быстрая компиляция (модуль компилируется один раз)
 * ✓ Изоляция (макросы не утекают наружу)
 * ✓ Четкий интерфейс (только export видимы)
 * ✓ Порядок импорта не важен
 */

// ============================================
// 📌 ПРИМЕРЫ МОДУЛЕЙ
// ============================================

void example_simple_module() {
    std::cout << "=== Simple Module Example ===\n\n";
    
    std::cout << "// ===== math.cppm =====\n";
    std::cout << R"(
export module math;

export int add(int a, int b) {
    return a + b;
}

export int multiply(int a, int b) {
    return a * b;
}

// Не экспортируется
int internal_helper() {
    return 42;
}
)" << '\n';

    std::cout << "// ===== main.cpp =====\n";
    std::cout << R"(
import math;
#include <iostream>

int main() {
    std::cout << add(5, 10) << '\n';       // OK
    std::cout << multiply(3, 7) << '\n';   // OK
    // internal_helper();                  // ОШИБКА: не экспортирован
}
)" << '\n';
}

void example_module_with_class() {
    std::cout << "\n=== Module with Classes ===\n\n";
    
    std::cout << "// ===== string_utils.cppm =====\n";
    std::cout << R"(
export module string_utils;

import <string>;
import <algorithm>;

export class StringHelper {
public:
    static std::string to_upper(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }
    
    static std::string to_lower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }
};

export std::string reverse(std::string str) {
    std::reverse(str.begin(), str.end());
    return str;
}
)" << '\n';

    std::cout << "// ===== main.cpp =====\n";
    std::cout << R"(
import string_utils;
#include <iostream>

int main() {
    auto upper = StringHelper::to_upper("hello");
    std::cout << upper << '\n';  // HELLO
    
    auto rev = reverse("world");
    std::cout << rev << '\n';    // dlrow
}
)" << '\n';
}

// ============================================
// 📌 MODULE PARTITIONS
// ============================================

void example_module_partitions() {
    std::cout << "\n=== Module Partitions ===\n\n";
    std::cout << "Module partitions позволяют разделить большой модуль на части\n\n";
    
    std::cout << "// ===== geometry:shapes.cppm (partition) =====\n";
    std::cout << R"(
export module geometry:shapes;

export struct Circle {
    double radius;
    double area() const { return 3.14159 * radius * radius; }
};

export struct Rectangle {
    double width, height;
    double area() const { return width * height; }
};
)" << '\n';

    std::cout << "// ===== geometry:math.cppm (partition) =====\n";
    std::cout << R"(
export module geometry:math;

export double distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx*dx + dy*dy);
}
)" << '\n';

    std::cout << "// ===== geometry.cppm (primary module interface) =====\n";
    std::cout << R"(
export module geometry;

export import :shapes;  // Re-export shapes partition
export import :math;    // Re-export math partition

// Можно добавить свои экспорты
export constexpr double PI = 3.14159265358979;
)" << '\n';

    std::cout << "// ===== main.cpp =====\n";
    std::cout << R"(
import geometry;
#include <iostream>

int main() {
    Circle c{5.0};
    std::cout << "Circle area: " << c.area() << '\n';
    
    double dist = distance(0, 0, 3, 4);
    std::cout << "Distance: " << dist << '\n';  // 5.0
}
)" << '\n';
}

// ============================================
// 📌 GLOBAL MODULE FRAGMENT
// ============================================

void example_global_module_fragment() {
    std::cout << "\n=== Global Module Fragment ===\n\n";
    std::cout << "Используется для legacy #include директив\n\n";
    
    std::cout << "// ===== legacy_wrapper.cppm =====\n";
    std::cout << R"(
module;  // Global module fragment начинается

// Здесь можно использовать #include
#include <vector>
#include <string>
#include "legacy_header.h"

export module legacy_wrapper;  // Объявление модуля

export class Wrapper {
    std::vector<std::string> data_;  // Типы из include доступны
    
public:
    void add(const std::string& s) {
        data_.push_back(s);
    }
    
    size_t size() const {
        return data_.size();
    }
};
)" << '\n';
}

// ============================================
// 📌 PRIVATE MODULE FRAGMENT
// ============================================

void example_private_module_fragment() {
    std::cout << "\n=== Private Module Fragment ===\n\n";
    std::cout << "Позволяет скрыть детали реализации в том же файле\n\n";
    
    std::cout << "// ===== calculator.cppm =====\n";
    std::cout << R"(
export module calculator;

export class Calculator {
public:
    int compute(int a, int b);
};

module :private;  // Private module fragment

// Реализация скрыта от пользователей модуля
int Calculator::compute(int a, int b) {
    return internal_algorithm(a, b);
}

static int internal_algorithm(int a, int b) {
    return a * a + b * b;
}
)" << '\n';
}

// ============================================
// 📌 HEADER UNITS
// ============================================

void example_header_units() {
    std::cout << "\n=== Header Units ===\n\n";
    std::cout << "Header units - способ импортировать header файлы как модули\n\n";
    
    std::cout << "// Вместо #include можно использовать import\n";
    std::cout << R"(
// Старый способ
#include <vector>
#include <string>
#include <iostream>

// Новый способ (Header units)
import <vector>;
import <string>;
import <iostream>;

int main() {
    std::vector<std::string> data;
    data.push_back("Hello");
    std::cout << data[0] << '\n';
}
)" << '\n';
    
    std::cout << "\nПреимущества header units:\n";
    std::cout << "✓ Быстрее компиляция\n";
    std::cout << "✓ Изоляция макросов\n";
    std::cout << "✓ Можно смешивать с обычными модулями\n";
}

// ============================================
// 📌 MIGRATION FROM HEADERS
// ============================================

void migration_guide() {
    std::cout << "\n=== Migration from Headers to Modules ===\n\n";
    
    std::cout << "СТРАТЕГИЯ МИГРАЦИИ:\n\n";
    
    std::cout << "1. НАЧНИТЕ С HEADER UNITS\n";
    std::cout << "   - Замените #include <...> на import <...>\n";
    std::cout << "   - Быстрый выигрыш в скорости компиляции\n\n";
    
    std::cout << "2. СОЗДАЙТЕ МОДУЛИ ДЛЯ НОВГО КОДА\n";
    std::cout << "   - Новые компоненты пишите как модули\n";
    std::cout << "   - Старый код оставьте как есть\n\n";
    
    std::cout << "3. ПОСТЕПЕННО КОНВЕРТИРУЙТЕ СТАРЫЕ HEADERS\n";
    std::cout << "   - По одному компоненту за раз\n";
    std::cout << "   - Используйте module wrappers для совместимости\n\n";
    
    std::cout << "ПРИМЕР WRAPPER МОДУЛЯ:\n";
    std::cout << R"(
// legacy_math.h (старый header)
#ifndef LEGACY_MATH_H
#define LEGACY_MATH_H

int add(int a, int b);
int subtract(int a, int b);

#endif

// legacy_math_module.cppm (wrapper module)
module;
#include "legacy_math.h"

export module legacy_math;

export {
    using ::add;
    using ::subtract;
}
)" << '\n';
}

// ============================================
// 📌 BUILD SYSTEMS
// ============================================

void cmake_support() {
    std::cout << "\n=== CMake Support for Modules ===\n\n";
    
    std::cout << "// CMakeLists.txt (CMake 3.28+)\n";
    std::cout << R"(
cmake_minimum_required(VERSION 3.28)
project(ModulesExample CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включить поддержку модулей
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

# Создать библиотеку-модуль
add_library(math_module)
target_sources(math_module
    PUBLIC
        FILE_SET CXX_MODULES FILES
            math.cppm
)

# Исполняемый файл использующий модуль
add_executable(app main.cpp)
target_link_libraries(app PRIVATE math_module)
)" << '\n';
    
    std::cout << "\nПОРЯДОК КОМПИЛЯЦИИ:\n";
    std::cout << "1. Сканирование зависимостей модулей\n";
    std::cout << "2. Компиляция модулей в BMI (Binary Module Interface)\n";
    std::cout << "3. Компиляция файлов импортирующих модули\n";
    std::cout << "4. Линковка\n";
}

// ============================================
// 📌 PERFORMANCE BENEFITS
// ============================================

void performance_comparison() {
    std::cout << "\n=== Performance Benefits ===\n\n";
    
    std::cout << "СРАВНЕНИЕ ВРЕМЕНИ КОМПИЛЯЦИИ:\n\n";
    
    std::cout << "ТРАДИЦИОННЫЕ HEADERS:\n";
    std::cout << "- Каждый .cpp файл парсит все #include заново\n";
    std::cout << "- iostream (~30000 строк) парсится в каждом TU\n";
    std::cout << "- Макросы могут влиять на последующий код\n";
    std::cout << "- Время компиляции: O(N * M)\n";
    std::cout << "  N = кол-во .cpp файлов\n";
    std::cout << "  M = размер всех headers\n\n";
    
    std::cout << "МОДУЛИ:\n";
    std::cout << "- Модуль компилируется один раз в BMI\n";
    std::cout << "- import просто загружает BMI (быстро!)\n";
    std::cout << "- Макросы не утекают наружу\n";
    std::cout << "- Время компиляции: O(M) + O(N * k)\n";
    std::cout << "  k = размер BMI (намного меньше M)\n\n";
    
    std::cout << "РЕАЛЬНЫЕ ЦИФРЫ (большой проект):\n";
    std::cout << "Headers:  120 секунд (clean build)\n";
    std::cout << "Modules:   45 секунд (clean build) - 62% быстрее!\n";
    std::cout << "Modules:    5 секунд (incremental) - 96% быстрее!\n";
}

// ============================================
// 📌 BEST PRACTICES
// ============================================

void best_practices() {
    std::cout << "\n=== Best Practices ===\n\n";
    
    std::cout << "РЕКОМЕНДАЦИИ:\n\n";
    
    std::cout << "1. ИМЕНОВАНИЕ\n";
    std::cout << "   ✓ module_name (lowercase с underscores)\n";
    std::cout << "   ✓ my_project.component.submodule (иерархия через точки)\n";
    std::cout << "   ✗ MyModule (избегайте CamelCase для имен модулей)\n\n";
    
    std::cout << "2. ОРГАНИЗАЦИЯ ФАЙЛОВ\n";
    std::cout << "   ✓ module_name.cppm - module interface\n";
    std::cout << "   ✓ module_name-impl.cpp - implementation (опционально)\n";
    std::cout << "   ✓ module_name-part.cppm - partitions\n\n";
    
    std::cout << "3. ЭКСПОРТ\n";
    std::cout << "   ✓ Экспортируйте только публичный API\n";
    std::cout << "   ✓ Используйте namespace для группировки\n";
    std::cout << "   ✗ Не экспортируйте детали реализации\n\n";
    
    std::cout << "4. ЗАВИСИМОСТИ\n";
    std::cout << "   ✓ Минимизируйте зависимости между модулями\n";
    std::cout << "   ✓ Используйте forward declarations где возможно\n";
    std::cout << "   ✓ Избегайте циклических зависимостей\n\n";
    
    std::cout << "5. LEGACY CODE\n";
    std::cout << "   ✓ Используйте global module fragment для #include\n";
    std::cout << "   ✓ Создавайте wrapper модули для старых headers\n";
    std::cout << "   ✓ Мигрируйте постепенно\n\n";
    
    std::cout << "6. ТЕСТИРОВАНИЕ\n";
    std::cout << "   ✓ Тесты могут импортировать модули\n";
    std::cout << "   ✓ Рассмотрите экспорт тестовых утилит как отдельный модуль\n";
}

// ============================================
// 📌 COMMON PITFALLS
// ============================================

void common_pitfalls() {
    std::cout << "\n=== Common Pitfalls ===\n\n";
    
    std::cout << "ТИПИЧНЫЕ ОШИБКИ:\n\n";
    
    std::cout << "1. МАКРОСЫ НЕ ЭКСПОРТИРУЮТСЯ\n";
    std::cout << R"(
// module.cppm
export module my_module;
#define MY_MACRO 42  // НЕ будет доступен вне модуля!

// Решение: используйте constexpr
export constexpr int MY_CONSTANT = 42;
)" << '\n';

    std::cout << "\n2. ПОРЯДОК module; И export module\n";
    std::cout << R"(
// НЕПРАВИЛЬНО:
export module my_module;
module;  // ОШИБКА! module; должен быть первым

// ПРАВИЛЬНО:
module;  // Global module fragment
#include <...>
export module my_module;
)" << '\n';

    std::cout << "\n3. ЗАБЫЛИ export\n";
    std::cout << R"(
export module math;

int add(int a, int b) {  // ЗАБЫЛИ export!
    return a + b;
}

// Правильно:
export int add(int a, int b) {
    return a + b;
}
)" << '\n';

    std::cout << "\n4. ЦИКЛИЧЕСКИЕ ЗАВИСИМОСТИ\n";
    std::cout << "Модули НЕ могут иметь циклические зависимости!\n";
    std::cout << "Если A import B и B import A - ошибка компиляции\n";
    std::cout << "Решение: реорганизуйте код или используйте forward declarations\n";
}

// ============================================
// 📌 COMPILER SUPPORT
// ============================================

void compiler_support() {
    std::cout << "\n=== Compiler Support (2024) ===\n\n";
    
    std::cout << "MSVC (Visual Studio 2022):\n";
    std::cout << "  ✅ Полная поддержка C++20 модулей\n";
    std::cout << "  ✅ Header units\n";
    std::cout << "  ✅ Хорошая интеграция с VS\n\n";
    
    std::cout << "Clang 16+:\n";
    std::cout << "  ✅ Хорошая поддержка модулей\n";
    std::cout << "  ✅ Флаг: -std=c++20\n";
    std::cout << "  ⚠️  Header units экспериментальны\n\n";
    
    std::cout << "GCC 14+:\n";
    std::cout << "  ⚠️  Базовая поддержка (улучшается)\n";
    std::cout << "  ⚠️  Флаг: -std=c++20 -fmodules-ts\n";
    std::cout << "  ❌ Header units пока не поддерживаются\n\n";
    
    std::cout << "РЕКОМЕНДАЦИИ:\n";
    std::cout << "- Для production используйте MSVC или Clang 16+\n";
    std::cout << "- GCC - подождите GCC 15+\n";
    std::cout << "- Проверяйте документацию вашего компилятора\n";
}

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== C++20 Modules Guide ===\n";
    std::cout << "Note: Все примеры в виде комментариев/строк\n";
    std::cout << "так как модули требуют специальной структуры файлов\n\n";
    
    example_simple_module();
    example_module_with_class();
    example_module_partitions();
    example_global_module_fragment();
    example_private_module_fragment();
    example_header_units();
    migration_guide();
    cmake_support();
    performance_comparison();
    best_practices();
    common_pitfalls();
    compiler_support();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ Модули - будущее C++ (замена headers)\n";
    std::cout << "✓ Быстрая компиляция (до 10x для больших проектов)\n";
    std::cout << "✓ Изоляция (макросы не утекают)\n";
    std::cout << "✓ Четкий интерфейс (только export видимы)\n";
    std::cout << "✓ Header units - для постепенной миграции\n";
    std::cout << "✓ Module partitions - для организации больших модулей\n";
    std::cout << "✓ Используйте MSVC или Clang для production\n";
    
    return 0;
}