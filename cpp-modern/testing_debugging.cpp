/*
 * ============================================
 * ТЕСТИРОВАНИЕ И ОТЛАДКА C++
 * ============================================
 * 
 * Практические техники тестирования и отладки:
 * - Unit testing frameworks (Catch2, GTest, doctest)
 * - Static analysis (static_assert, concepts)
 * - Runtime debugging (assert, source_location)
 * - Sanitizers (ASan, TSan, UBSan)
 * - Profiling и benchmarking
 * 
 * Компиляция: 
 *   g++ -std=c++20 testing_debugging.cpp
 *   g++ -std=c++20 -fsanitize=address testing_debugging.cpp
 */

#include <iostream>
#include <cassert>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <type_traits>

#ifdef __cpp_lib_source_location
#include <source_location>
#endif

// ====================================================================================================
// 📌 TESTING FRAMEWORKS - ТЕСТОВЫЕ ФРЕЙМВОРКИ
// ====================================================================================================

/*
 * ──────────────────────────────────────────
 * CATCH2 PATTERN:
 * ──────────────────────────────────────────
 * 
 * #include <catch2/catch_test_macros.hpp>
 * 
 * TEST_CASE("Vector operations", "[vector]") {
 *     std::vector<int> vec = {1, 2, 3};
 *     
 *     SECTION("push_back") {
 *         vec.push_back(4);
 *         REQUIRE(vec.size() == 4);
 *     }
 *     
 *     SECTION("pop_back") {
 *         vec.pop_back();
 *         REQUIRE(vec.size() == 2);
 *     }
 * }
 */

/*
 * ──────────────────────────────────────────
 * GOOGLE TEST PATTERN:
 * ──────────────────────────────────────────
 * 
 * #include <gtest/gtest.h>
 * 
 * TEST(VectorTest, PushBack) {
 *     std::vector<int> vec = {1, 2, 3};
 *     vec.push_back(4);
 *     EXPECT_EQ(vec.size(), 4);
 *     ASSERT_EQ(vec.back(), 4);
 * }
 * 
 * int main(int argc, char** argv) {
 *     ::testing::InitGoogleTest(&argc, argv);
 *     return RUN_ALL_TESTS();
 * }
 */

/*
 * ──────────────────────────────────────────
 * DOCTEST PATTERN (header-only):
 * ──────────────────────────────────────────
 * 
 * #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
 * #include <doctest/doctest.h>
 * 
 * TEST_CASE("testing vector") {
 *     std::vector<int> vec;
 *     vec.push_back(1);
 *     CHECK(vec.size() == 1);
 *     REQUIRE(vec.front() == 1);
 * }
 */

// ──────────────────────────────────────────
// Custom Assertions - свои макросы проверки
// ──────────────────────────────────────────

#define EXPECT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "ASSERTION FAILED: " << message << '\n'; \
            std::cerr << "  at " << __FILE__ << ":" << __LINE__ << '\n'; \
        } \
    } while (0)

// Примеры использования
int test_x = 42;
EXPECT(test_x == 42, "x should be 42");
EXPECT(test_x > 0, "x should be positive");

std::cout << "Custom assertions passed!\n";

// ====================================================================================================
// 📌 UNIT TESTING PATTERNS - ПАТТЕРНЫ ЮНИТ-ТЕСТИРОВАНИЯ
// ====================================================================================================

// ──────────────────────────────────────────
// AAA Pattern - Arrange, Act, Assert
// ──────────────────────────────────────────

// Arrange - подготовка
std::vector<int> aaa_numbers = {1, 2, 3, 4, 5};

// Act - действие
int aaa_sum = 0;
for (int n : aaa_numbers) {
    aaa_sum += n;
}

// Assert - проверка
assert(aaa_sum == 15);
std::cout << "AAA Pattern: Sum test passed\n";

// ──────────────────────────────────────────
// Test Fixture - повторяемая настройка тестов
// ──────────────────────────────────────────

class CalculatorFixture {
protected:
    int a = 10;
    int b = 20;
    
    void setup() {
        std::cout << "Setting up test\n";
    }
    
    void teardown() {
        std::cout << "Tearing down test\n";
    }
};

// Использование fixture
CalculatorFixture calc_fixture;
calc_fixture.setup();

int fixture_result = calc_fixture.a + calc_fixture.b;
assert(fixture_result == 30);

calc_fixture.teardown();

// ──────────────────────────────────────────
// Parameterized Tests - параметризованные тесты
// ──────────────────────────────────────────

template<typename T>
void test_container_size() {
    T container;
    assert(container.empty());
    
    container.insert(container.end(), typename T::value_type{});
    assert(!container.empty());
    assert(container.size() == 1);
}

// Запуск с разными типами
test_container_size<std::vector<int>>();
test_container_size<std::vector<std::string>>();

std::cout << "Parameterized tests passed\n";

// ──────────────────────────────────────────
// Mock Object Pattern - тестовые заглушки
// ──────────────────────────────────────────

class DatabaseInterface {
public:
    virtual ~DatabaseInterface() = default;
    virtual std::string query(const std::string& sql) = 0;
};

class MockDatabase : public DatabaseInterface {
    std::string expected_query_;
    std::string response_;
    bool was_called_ = false;
    
public:
    void expect_query(std::string query, std::string response) {
        expected_query_ = std::move(query);
        response_ = std::move(response);
    }
    
    std::string query(const std::string& sql) override {
        was_called_ = true;
        assert(sql == expected_query_);
        return response_;
    }
    
    bool was_called() const { return was_called_; }
};

// Использование mock object
MockDatabase mock_db;
mock_db.expect_query("SELECT * FROM users", "Alice,Bob");

std::string mock_result = mock_db.query("SELECT * FROM users");
assert(mock_result == "Alice,Bob");
assert(mock_db.was_called());

std::cout << "Mock test passed\n";

// ====================================================================================================
// 📌 STATIC ANALYSIS - СТАТИЧЕСКИЙ АНАЛИЗ
// ====================================================================================================

// ──────────────────────────────────────────
// static_assert - compile-time проверки
// ──────────────────────────────────────────

static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
static_assert(sizeof(void*) == 8, "64-bit platform required");

std::cout << "Static assertions passed at compile time\n";

// ──────────────────────────────────────────
// Concepts - концепты для проверки типов
// ──────────────────────────────────────────

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template<Addable T>
T add(T a, T b) {
    return a + b;
}

// Использование
int concept_result = add(10, 20);  // ✅ Компилируется
std::cout << "10 + 20 = " << concept_result << '\n';

// add("hello", "world");  // ❌ Ошибка компиляции - string не Addable

// ──────────────────────────────────────────
// Type Traits - проверка свойств типов
// ──────────────────────────────────────────

template<typename T>
void process_trivial_type(T value) {
    static_assert(std::is_trivially_copyable_v<T>,
                  "T must be trivially copyable");
    
    std::cout << "Processing trivial type\n";
}

// Использование
process_trivial_type(42);  // ✅ int is trivial
// process_trivial_type(std::string("test"));  // ❌ string is not trivial

// ──────────────────────────────────────────
// Constexpr Tests - тесты во время компиляции
// ──────────────────────────────────────────

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr bool test_factorial() {
    return factorial(5) == 120 && factorial(0) == 1;
}

static_assert(test_factorial(), "Factorial test failed");
std::cout << "Constexpr tests passed at compile time\n";

// ====================================================================================================
// 📌 RUNTIME DEBUGGING - ОТЛАДКА ВО ВРЕМЯ ВЫПОЛНЕНИЯ
// ====================================================================================================

// ──────────────────────────────────────────
// assert() - runtime проверки
// ──────────────────────────────────────────

int assert_x = 42;
assert(assert_x > 0);  // Выполняется только в debug builds

std::cout << "Assertions enabled in debug build\n";

// NDEBUG отключает assert
#ifdef NDEBUG
std::cout << "NDEBUG defined - assertions disabled\n";
#else
std::cout << "Debug mode - assertions enabled\n";
#endif

// ──────────────────────────────────────────
// source_location (C++20) - информация о месте вызова
// ──────────────────────────────────────────

#ifdef __cpp_lib_source_location

void log_message(const std::string& msg,
                 const std::source_location& loc = std::source_location::current()) {
    std::cout << "["
              << loc.file_name() << ":"
              << loc.line() << " in "
              << loc.function_name() << "] "
              << msg << '\n';
}

// Использование
log_message("This is a debug message");
log_message("Another message");

#endif

// ──────────────────────────────────────────
// Logging System - система логирования
// ──────────────────────────────────────────

enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

class Logger {
    LogLevel min_level_ = LogLevel::INFO;
    
public:
    void set_level(LogLevel level) { min_level_ = level; }
    
    template<typename... Args>
    void log(LogLevel level, Args&&... args) {
        if (level >= min_level_) {
            std::cout << "[" << level_string(level) << "] ";
            (std::cout << ... << args) << '\n';
        }
    }
    
private:
    const char* level_string(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
        }
        return "UNKNOWN";
    }
};

// Использование Logger
Logger app_logger;

app_logger.log(LogLevel::DEBUG, "Debug message");  // Не выводится (min_level = INFO)
app_logger.log(LogLevel::INFO, "Application started");
app_logger.log(LogLevel::WARNING, "Low memory: ", 512, " MB");
app_logger.log(LogLevel::ERROR, "Failed to open file");

// Включить DEBUG
app_logger.set_level(LogLevel::DEBUG);
app_logger.log(LogLevel::DEBUG, "Now debug is visible");

// ──────────────────────────────────────────
// Debug vs Release Builds
// ──────────────────────────────────────────

#ifdef _DEBUG
std::cout << "DEBUG build\n";
#else
std::cout << "RELEASE build\n";
#endif

// Debug-only код
#ifndef NDEBUG
int debug_counter = 0;
std::cout << "Debug counter: " << debug_counter << '\n';
#endif

// ====================================================================================================
// 📌 SANITIZERS - ИНСТРУМЕНТЫ ОБНАРУЖЕНИЯ ОШИБОК
// ====================================================================================================

/*
 * ──────────────────────────────────────────
 * ADDRESSSANITIZER (ASan) - детектор ошибок памяти
 * ──────────────────────────────────────────
 * Компиляция: g++ -fsanitize=address -g testing_debugging.cpp
 * 
 * Находит:
 * - Use-after-free
 * - Heap buffer overflow
 * - Stack buffer overflow
 * - Memory leaks
 */

// ❌ Пример ошибки (закомментировано для безопасности)
// int* ptr = new int(42);
// delete ptr;
// std::cout << *ptr << '\n';  // Use-after-free!

// ✅ Правильный код
std::unique_ptr<int> asan_ptr = std::make_unique<int>(42);
std::cout << "Value: " << *asan_ptr << '\n';

std::cout << "Compile with -fsanitize=address to detect memory errors\n";

/*
 * ──────────────────────────────────────────
 * THREADSANITIZER (TSan) - детектор data races
 * ──────────────────────────────────────────
 * Компиляция: g++ -fsanitize=thread -g testing_debugging.cpp
 */

std::cout << "Compile with -fsanitize=thread to detect data races\n";

/*
 * ──────────────────────────────────────────
 * UNDEFINEDBEHAVIORSANITIZER (UBSan)
 * ──────────────────────────────────────────
 * Компиляция: g++ -fsanitize=undefined -g testing_debugging.cpp
 * 
 * Находит:
 * - Integer overflow
 * - Division by zero
 * - Null pointer dereference
 * - Invalid shifts
 */

// ✅ Безопасный код
int ubsan_x = 100;
int ubsan_y = 2;
std::cout << "100 / 2 = " << ubsan_x / ubsan_y << '\n';

std::cout << "Compile with -fsanitize=undefined to detect UB\n";

// Available sanitizers
std::cout << "\nAvailable sanitizers:\n";
std::cout << "  -fsanitize=address     (ASan)\n";
std::cout << "  -fsanitize=thread      (TSan)\n";
std::cout << "  -fsanitize=undefined   (UBSan)\n";
std::cout << "  -fsanitize=memory      (MSan, Clang only)\n";
std::cout << "  -fsanitize=leak        (LSan)\n";

// ====================================================================================================
// 📌 PROFILING & BENCHMARKING - ПРОФИЛИРОВАНИЕ
// ====================================================================================================

// ──────────────────────────────────────────
// Benchmark Helper - простой бенчмарк
// ──────────────────────────────────────────

template<typename Func>
auto benchmark(const std::string& name, Func f, int iterations = 1000) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        f();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << name << ": "
              << duration.count() << " μs ("
              << duration.count() / iterations << " μs/iter)\n";
    
    return duration;
}

// ──────────────────────────────────────────
// Примеры бенчмарков
// ──────────────────────────────────────────

// Сравнение vector vs list
benchmark("vector push_back", []() {
    std::vector<int> vec;
    for (int i = 0; i < 100; ++i) {
        vec.push_back(i);
    }
});

benchmark("vector with reserve", []() {
    std::vector<int> vec;
    vec.reserve(100);
    for (int i = 0; i < 100; ++i) {
        vec.push_back(i);
    }
});

// ──────────────────────────────────────────
// Profiling Tools - инструменты профилирования
// ──────────────────────────────────────────

std::cout << R"(
CPU Profilers:
  - perf (Linux): perf record ./app && perf report
  - Valgrind callgrind: valgrind --tool=callgrind ./app
  - gprof: compile with -pg, run, then gprof
  - Intel VTune: GUI-based profiler

Memory Profilers:
  - Valgrind massif: valgrind --tool=massif ./app
  - Heaptrack: heaptrack ./app
  - AddressSanitizer: -fsanitize=address

Benchmarking Libraries:
  - Google Benchmark: industry standard
  - Catch2: built-in benchmarking
  - Hayai: lightweight C++ benchmarking
)";

// ====================================================================================================
// 📌 DEBUGGING TECHNIQUES - ТЕХНИКИ ОТЛАДКИ
// ====================================================================================================

// ──────────────────────────────────────────
// Printf Debugging - отладка через вывод
// ──────────────────────────────────────────

int debug_x = 10;
std::cout << "DEBUG: debug_x = " << debug_x << '\n';

debug_x *= 2;
std::cout << "DEBUG: after multiply, debug_x = " << debug_x << '\n';

#ifdef __cpp_lib_print
// C++23 std::print
// std::print("debug_x = {}\n", debug_x);
#endif

// ──────────────────────────────────────────
// Conditional Compilation - условная компиляция
// ──────────────────────────────────────────

#ifdef DEBUG
std::cout << "Debug code is active\n";
#endif

#if defined(__GNUC__)
std::cout << "Compiled with GCC/G++\n";
#elif defined(_MSC_VER)
std::cout << "Compiled with MSVC\n";
#endif

// ──────────────────────────────────────────
// Debug Macros - отладочные макросы
// ──────────────────────────────────────────

#ifndef NDEBUG
    #define DEBUG_PRINT(x) std::cout << "DEBUG: " << #x << " = " << (x) << '\n'
#else
    #define DEBUG_PRINT(x)
#endif

// Использование
int macro_value = 42;
DEBUG_PRINT(macro_value);
DEBUG_PRINT(macro_value * 2);

// ============================================
// 📌 BEST PRACTICES
// ============================================

/*
 * BEST PRACTICES ДЛЯ ТЕСТИРОВАНИЯ И ОТЛАДКИ:
 * 
 * 1. ТЕСТИРОВАНИЕ
 *    ✅ Используй современный framework (Catch2, doctest)
 *    ✅ AAA pattern (Arrange-Act-Assert)
 *    ✅ Один assert на тест
 *    ✅ Описательные имена тестов
 *    ✅ Покрытие edge cases
 * 
 * 2. STATIC ANALYSIS
 *    ✅ static_assert для compile-time проверок
 *    ✅ Concepts вместо SFINAE
 *    ✅ constexpr unit tests
 *    ✅ Type traits для метапрограммирования
 * 
 * 3. RUNTIME DEBUGGING
 *    ✅ assert() в debug builds
 *    ✅ source_location для логирования
 *    ✅ Structured logging с уровнями
 *    ❌ Избегай printf debugging в production
 * 
 * 4. SANITIZERS
 *    ✅ ASan в каждом CI/CD pipeline
 *    ✅ TSan для многопоточного кода
 *    ✅ UBSan всегда
 *    ⚠️ Не используй в production (overhead)
 * 
 * 5. PROFILING
 *    ✅ Профилируй перед оптимизацией
 *    ✅ Используй perf/VTune для CPU
 *    ✅ Valgrind/Heaptrack для памяти
 *    ✅ Google Benchmark для микробенчмарков
 * 
 * 6. CI/CD
 *    ✅ Автоматический запуск тестов
 *    ✅ Coverage reports
 *    ✅ Sanitizers в CI
 *    ✅ Static analysis (clang-tidy, cppcheck)
 */

// ====================================================================================================
// 📌 ИТОГО: C++ Testing & Debugging
// ====================================================================================================
// • Testing frameworks: Catch2, Google Test, doctest
// • Static analysis: static_assert, concepts
// • Runtime: assert, source_location, логирование
// • Sanitizers: ASan, TSan, UBSan, MSan
// • Profiling: perf, Valgrind, Tracy, benchmarks
// • CI/CD: автоматические тесты + sanitizers
