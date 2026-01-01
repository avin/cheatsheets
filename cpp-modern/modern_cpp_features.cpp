/*
 * ============================================
 * MODERN C++ FEATURES (C++11-C++23)
 * ============================================
 * 
 * Обзор ключевых возможностей современного C++:
 * - C++20: Concepts, Ranges, Coroutines, Modules, Spaceship
 * - C++23: std::expected, print, deducing this, mdspan
 * 
 * Требования: C++20 минимум, C++23 для новейших фич
 * Компиляция: g++ -std=c++23 modern_cpp_features.cpp
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ranges>
#include <concepts>
#include <compare>
#include <chrono>
#include <optional>

// C++20 format (если доступно)
#if __has_include(<format>)
#include <format>
#define HAS_FORMAT 1
#else
#define HAS_FORMAT 0
#endif

// C++23 expected (если доступно)
#if __has_include(<expected>)
#include <expected>
#define HAS_EXPECTED 1
#else
#define HAS_EXPECTED 0
#endif

// ============================================
// 📌 C++20: CONCEPTS
// ============================================

namespace cpp20_concepts {

// Кастомный concept
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

// Использование concepts
template<Numeric T>
T square(T value) {
    return value * value;
}

// Concept-based overloading
void process(std::integral auto value) {
    std::cout << "Processing integer: " << value << '\n';
}

void process(std::floating_point auto value) {
    std::cout << "Processing float: " << value << '\n';
}

// Составные concepts
template<typename T>
concept Number = Numeric<T> && Addable<T>;

void demo() {
    std::cout << "=== C++20 Concepts ===\n";
    
    std::cout << "square(5): " << square(5) << '\n';
    std::cout << "square(2.5): " << square(2.5) << '\n';
    
    process(42);
    process(3.14);
}

} // namespace cpp20_concepts

// ============================================
// 📌 C++20: RANGES
// ============================================

namespace cpp20_ranges {

void basic_ranges() {
    std::cout << "\n=== C++20 Ranges ===\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Filter + Transform
    auto result = numbers 
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; });
    
    std::cout << "Even squares: ";
    for (int n : result) {
        std::cout << n << " ";
    }
    std::cout << '\n';
    
    // Take + Drop
    auto middle = numbers 
        | std::views::drop(2)
        | std::views::take(5);
    
    std::cout << "Middle elements: ";
    for (int n : middle) {
        std::cout << n << " ";
    }
    std::cout << '\n';
}

void range_algorithms() {
    std::cout << "\n=== Range Algorithms ===\n";
    
    std::vector<int> vec = {5, 2, 8, 1, 9};
    
    // Сортировка с проекцией
    std::ranges::sort(vec, std::greater{});
    
    std::cout << "Sorted descending: ";
    for (int n : vec) {
        std::cout << n << " ";
    }
    std::cout << '\n';
    
    // Find с проекцией
    struct Person {
        std::string name;
        int age;
    };
    
    std::vector<Person> people = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };
    
    auto it = std::ranges::find(people, "Bob", &Person::name);
    if (it != people.end()) {
        std::cout << "Found: " << it->name << ", age " << it->age << '\n';
    }
}

void demo() {
    basic_ranges();
    range_algorithms();
}

} // namespace cpp20_ranges

// ============================================
// 📌 C++20: THREE-WAY COMPARISON (SPACESHIP)
// ============================================

namespace cpp20_spaceship {

// Автоматическая генерация операторов сравнения
struct Point {
    int x, y;
    
    // Одна строка для всех операторов сравнения!
    auto operator<=>(const Point&) const = default;
};

// Кастомная реализация
struct Person {
    std::string name;
    int age;
    
    auto operator<=>(const Person& other) const {
        // Сначала по имени, потом по возрасту
        if (auto cmp = name <=> other.name; cmp != 0) {
            return cmp;
        }
        return age <=> other.age;
    }
    
    // Equality все равно нужно определить
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

void demo() {
    std::cout << "\n=== C++20 Spaceship Operator ===\n";
    
    Point p1{1, 2};
    Point p2{1, 3};
    
    std::cout << "p1 < p2: " << std::boolalpha << (p1 < p2) << '\n';
    std::cout << "p1 == p2: " << (p1 == p2) << '\n';
    std::cout << "p1 != p2: " << (p1 != p2) << '\n';
    
    Person alice{"Alice", 30};
    Person bob{"Bob", 25};
    
    std::cout << "alice < bob: " << (alice < bob) << '\n';
}

} // namespace cpp20_spaceship

// ============================================
// 📌 C++20: FORMAT LIBRARY
// ============================================

namespace cpp20_format {

void basic_format() {
    #if HAS_FORMAT
    std::cout << "\n=== C++20 std::format ===\n";
    
    std::string msg = std::format("Hello, {}!", "World");
    std::cout << msg << '\n';
    
    // Позиционные аргументы
    std::cout << std::format("{1} {0}", "World", "Hello") << '\n';
    
    // Форматирование чисел
    std::cout << std::format("Hex: {:#x}", 255) << '\n';
    std::cout << std::format("Binary: {:b}", 42) << '\n';
    std::cout << std::format("Float: {:.2f}", 3.14159) << '\n';
    
    // Ширина и выравнивание
    std::cout << std::format("|{:<10}|", "left") << '\n';
    std::cout << std::format("|{:>10}|", "right") << '\n';
    std::cout << std::format("|{:^10}|", "center") << '\n';
    #else
    std::cout << "\nstd::format not available\n";
    #endif
}

void demo() {
    basic_format();
}

} // namespace cpp20_format

// ============================================
// 📌 C++20: КАЛЕНДАРЬ И ВРЕМЕННЫЕ ЗОНЫ
// ============================================

namespace cpp20_calendar {

void calendar_demo() {
    std::cout << "\n=== C++20 Calendar ===\n";
    
    using namespace std::chrono;
    
    // Создание дат
    auto today = year_month_day{2024y, January, 15d};
    std::cout << "Date: " << today << '\n';
    
    // Арифметика с датами
    auto next_month = today + months{1};
    std::cout << "Next month: " << next_month << '\n';
    
    // День недели
    auto ymd = year_month_day{2024y, December, 25d};
    auto weekday = year_month_weekday{ymd}.weekday();
    std::cout << "Christmas 2024 is on: " << weekday << '\n';
}

void demo() {
    calendar_demo();
}

} // namespace cpp20_calendar

// ============================================
// 📌 C++20: ДРУГИЕ ВОЗМОЖНОСТИ
// ============================================

namespace cpp20_other {

// constexpr improvements
constexpr int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// consteval - гарантированно compile-time
consteval int square(int n) {
    return n * n;
}

// constinit - статическая инициализация
constinit int global_value = 42;

// [[likely]] и [[unlikely]]
int process_value(int x) {
    if (x > 0) [[likely]] {
        return x * 2;
    } else [[unlikely]] {
        return 0;
    }
}

// using enum (C++20)
enum class Color { Red, Green, Blue };

void print_color(Color c) {
    using enum Color;  // Теперь можно использовать без префикса
    
    switch (c) {
        case Red: std::cout << "Red\n"; break;
        case Green: std::cout << "Green\n"; break;
        case Blue: std::cout << "Blue\n"; break;
    }
}

// Designated initializers
struct Point {
    int x;
    int y;
    int z;
};

// Template lambda
void template_lambda_demo() {
    auto lambda = []<typename T>(T value) {
        std::cout << "Value: " << value << '\n';
    };
    
    lambda(42);
    lambda(3.14);
    lambda("hello");
}

void demo() {
    std::cout << "\n=== C++20 Other Features ===\n";
    
    constexpr int fact5 = factorial(5);
    std::cout << "factorial(5): " << fact5 << '\n';
    
    constexpr int sq10 = square(10);
    std::cout << "square(10): " << sq10 << '\n';
    
    print_color(Color::Red);
    
    // Designated initializers
    Point p = {.x = 10, .y = 20, .z = 30};
    std::cout << "Point: " << p.x << ", " << p.y << ", " << p.z << '\n';
    
    template_lambda_demo();
}

} // namespace cpp20_other

// ============================================
// 📌 C++23: PRINT LIBRARY
// ============================================

namespace cpp23_print {

void demo() {
    std::cout << "\n=== C++23 Print Library ===\n";
    
    // std::print и std::println доступны в C++23
    // Для демонстрации используем cout
    std::cout << "std::print/println - type-safe альтернатива printf\n";
    std::cout << "Использует std::format под капотом\n";
    
    #if __cpp_lib_print >= 202207L
    std::print("Hello, {}!\n", "World");
    std::println("Number: {}", 42);
    #endif
}

} // namespace cpp23_print

// ============================================
// 📌 C++23: std::expected
// ============================================

namespace cpp23_expected {

enum class Error {
    NotFound,
    PermissionDenied,
    InvalidInput
};

// Функция возвращающая expected
#if HAS_EXPECTED
std::expected<int, Error> divide(int a, int b) {
    if (b == 0) {
        return std::unexpected(Error::InvalidInput);
    }
    return a / b;
}

void monadic_operations() {
    std::cout << "\n=== C++23 std::expected ===\n";
    
    auto result = divide(10, 2);
    
    if (result) {
        std::cout << "Result: " << *result << '\n';
    } else {
        std::cout << "Error occurred\n";
    }
    
    // value_or
    std::cout << "Value or default: " << divide(10, 0).value_or(-1) << '\n';
    
    // and_then
    auto doubled = divide(10, 2)
        .and_then([](int val) { return divide(val, 1); });
    
    if (doubled) {
        std::cout << "Doubled: " << *doubled << '\n';
    }
}
#endif

void demo() {
    #if HAS_EXPECTED
    monadic_operations();
    #else
    std::cout << "\nstd::expected not available (requires C++23)\n";
    #endif
}

} // namespace cpp23_expected

// ============================================
// 📌 C++23: MULTIDIMENSIONAL SUBSCRIPT
// ============================================

namespace cpp23_multidim_subscript {

// Матрица с operator[i, j]
template<typename T>
class Matrix {
    std::vector<T> data_;
    size_t rows_, cols_;
    
public:
    Matrix(size_t rows, size_t cols)
        : data_(rows * cols), rows_(rows), cols_(cols) {}
    
    // C++23: multidimensional subscript
    T& operator[](size_t i, size_t j) {
        return data_[i * cols_ + j];
    }
    
    const T& operator[](size_t i, size_t j) const {
        return data_[i * cols_ + j];
    }
};

void demo() {
    std::cout << "\n=== C++23 Multidimensional Subscript ===\n";
    
    Matrix<int> mat(3, 3);
    
    // Инициализация
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            mat[i, j] = i * 3 + j;  // C++23 syntax!
        }
    }
    
    std::cout << "Matrix[1, 1] = " << mat[1, 1] << '\n';
}

} // namespace cpp23_multidim_subscript

// ============================================
// 📌 C++23: DEDUCING THIS
// ============================================

namespace cpp23_deducing_this {

// Explicit object parameter
struct Counter {
    int value = 0;
    
    // C++23: деduction this для избежания дублирования const/non-const
    #if __cpp_explicit_this_parameter >= 202110L
    auto& increment(this auto& self) {
        ++self.value;
        return self;
    }
    #else
    Counter& increment() {
        ++value;
        return *this;
    }
    #endif
};

// Рекурсивные лямбды становятся проще
void recursive_lambda_demo() {
    #if __cpp_explicit_this_parameter >= 202110L
    auto fibonacci = [](this auto self, int n) -> int {
        if (n <= 1) return n;
        return self(n - 1) + self(n - 2);
    };
    
    std::cout << "Fibonacci(10): " << fibonacci(10) << '\n';
    #endif
}

void demo() {
    std::cout << "\n=== C++23 Deducing This ===\n";
    
    Counter c;
    c.increment().increment();
    std::cout << "Counter value: " << c.value << '\n';
    
    #if __cpp_explicit_this_parameter >= 202110L
    recursive_lambda_demo();
    #else
    std::cout << "Deducing this not available\n";
    #endif
}

} // namespace cpp23_deducing_this

// ============================================
// 📌 C++23: RANGES IMPROVEMENTS
// ============================================

namespace cpp23_ranges {

void new_views() {
    std::cout << "\n=== C++23 Ranges Improvements ===\n";
    
    std::vector<int> vec1 = {1, 2, 3};
    std::vector<int> vec2 = {4, 5, 6};
    
    // zip (если доступно)
    #if __cpp_lib_ranges_zip >= 202110L
    for (auto [a, b] : std::views::zip(vec1, vec2)) {
        std::cout << a << " + " << b << " = " << (a + b) << '\n';
    }
    #endif
    
    // chunk
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    #if __cpp_lib_ranges_chunk >= 202202L
    for (auto chunk : numbers | std::views::chunk(3)) {
        std::cout << "Chunk: ";
        for (int n : chunk) {
            std::cout << n << " ";
        }
        std::cout << '\n';
    }
    #endif
}

void demo() {
    new_views();
}

} // namespace cpp23_ranges

// ============================================
// 📌 C++23: ДРУГИЕ ВОЗМОЖНОСТИ
// ============================================

namespace cpp23_other {

// std::unreachable()
[[noreturn]] void handle_error(int code) {
    if (code == 1) {
        throw std::runtime_error("Error 1");
    } else if (code == 2) {
        throw std::runtime_error("Error 2");
    }
    
    #if __cpp_lib_unreachable >= 202202L
    std::unreachable();  // Подсказка компилятору
    #endif
}

// if consteval
constexpr int compute(int n) {
    #if __cpp_if_consteval >= 202106L
    if consteval {
        // Compile-time ветка
        return n * n;
    } else {
        // Runtime ветка
        return n + n;
    }
    #else
    return n * n;
    #endif
}

// Size_t literals
void size_t_literals() {
    #if __cpp_size_t_suffix >= 202011L
    auto size = 42uz;  // size_t literal
    std::cout << "Size_t literal: " << size << '\n';
    #endif
}

// string::contains() (C++23)
void string_contains() {
    std::string text = "Hello, World!";
    
    #if __cpp_lib_string_contains >= 202011L
    if (text.contains("World")) {
        std::cout << "Contains 'World'\n";
    }
    #else
    if (text.find("World") != std::string::npos) {
        std::cout << "Contains 'World'\n";
    }
    #endif
}

// std::to_underlying
enum class Status : int {
    Active = 1,
    Inactive = 0
};

void to_underlying_demo() {
    #if __cpp_lib_to_underlying >= 202102L
    auto value = std::to_underlying(Status::Active);
    std::cout << "Underlying value: " << value << '\n';
    #else
    auto value = static_cast<int>(Status::Active);
    std::cout << "Underlying value: " << value << '\n';
    #endif
}

void demo() {
    std::cout << "\n=== C++23 Other Features ===\n";
    
    constexpr int val = compute(5);
    std::cout << "compute(5): " << val << '\n';
    
    string_contains();
    to_underlying_demo();
}

} // namespace cpp23_other

// ============================================
// 📌 C++11-C++17: КРАТКИЙ ОБЗОР
// ============================================

namespace cpp11_17_summary {

void cpp11_features() {
    std::cout << "\n=== C++11 Key Features ===\n";
    std::cout << "✓ auto keyword\n";
    std::cout << "✓ Range-based for loops\n";
    std::cout << "✓ Lambda expressions\n";
    std::cout << "✓ Smart pointers (unique_ptr, shared_ptr)\n";
    std::cout << "✓ Move semantics (&&)\n";
    std::cout << "✓ constexpr\n";
    std::cout << "✓ nullptr\n";
    std::cout << "✓ Thread library\n";
}

void cpp14_features() {
    std::cout << "\n=== C++14 Key Features ===\n";
    std::cout << "✓ Generic lambdas\n";
    std::cout << "✓ Return type deduction\n";
    std::cout << "✓ Binary literals (0b...)\n";
    std::cout << "✓ std::make_unique\n";
    std::cout << "✓ Variable templates\n";
}

void cpp17_features() {
    std::cout << "\n=== C++17 Key Features ===\n";
    std::cout << "✓ Structured bindings (auto [a, b] = ...)\n";
    std::cout << "✓ if/switch with initializer\n";
    std::cout << "✓ std::optional\n";
    std::cout << "✓ std::variant\n";
    std::cout << "✓ std::filesystem\n";
    std::cout << "✓ constexpr if\n";
    std::cout << "✓ Fold expressions\n";
    std::cout << "✓ Class template argument deduction (CTAD)\n";
}

void demo() {
    cpp11_features();
    cpp14_features();
    cpp17_features();
}

} // namespace cpp11_17_summary

// ============================================
// 📌 ПРАКТИЧЕСКИЕ СОВЕТЫ
// ============================================

/*
 * РЕКОМЕНДАЦИИ ПО ИСПОЛЬЗОВАНИЮ MODERN C++:
 * 
 * C++20 (ШИРОКО ДОСТУПЕН):
 * ✓ Concepts - везде вместо SFINAE
 * ✓ Ranges - для работы с последовательностями
 * ✓ Spaceship operator - для типов с полным порядком
 * ✓ std::format - вместо iostream для форматирования
 * ✓ constexpr улучшения - максимум в compile-time
 * 
 * C++23 (НОВОЕ):
 * ✓ std::expected - для явной обработки ошибок
 * ✓ std::print - для вывода
 * ✓ Deducing this - для избежания дублирования
 * ✓ Ranges improvements - zip, chunk, enumerate
 * ✓ Multidimensional subscript - для матриц
 * 
 * ОБЩИЕ СОВЕТЫ:
 * 1. Используйте auto где возможно
 * 2. Prefer range-based for
 * 3. Smart pointers вместо raw pointers
 * 4. Structured bindings для кортежей/пар
 * 5. constexpr для compile-time вычислений
 * 6. std::optional для nullable значений
 * 7. Concepts для ограничения шаблонов
 * 8. Ranges для ленивых вычислений
 */

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== Modern C++ Features Overview ===\n";
    
    // C++20
    cpp20_concepts::demo();
    cpp20_ranges::demo();
    cpp20_spaceship::demo();
    cpp20_format::demo();
    cpp20_calendar::demo();
    cpp20_other::demo();
    
    // C++23
    cpp23_print::demo();
    cpp23_expected::demo();
    cpp23_multidim_subscript::demo();
    cpp23_deducing_this::demo();
    cpp23_ranges::demo();
    cpp23_other::demo();
    
    // Summary
    cpp11_17_summary::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ C++20 - Concepts, Ranges, Coroutines, Modules, Spaceship\n";
    std::cout << "✓ C++23 - std::expected, print, deducing this, mdspan\n";
    std::cout << "✓ Используйте modern features для более безопасного кода\n";
    std::cout << "✓ Компилятор - ваш друг (concepts дают лучшие ошибки)\n";
    std::cout << "✓ Constexpr everything - производительность бесплатно\n";
    
    return 0;
}