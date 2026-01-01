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

// ============================================
// 📌 TESTING FRAMEWORKS PATTERNS
// ============================================

namespace testing_frameworks {

/*
 * CATCH2 PATTERN:
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
 * GOOGLE TEST PATTERN:
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
 * DOCTEST PATTERN (header-only):
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

// Custom assertion macro
#define EXPECT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "ASSERTION FAILED: " << message << '\n'; \
            std::cerr << "  at " << __FILE__ << ":" << __LINE__ << '\n'; \
        } \
    } while (0)

void demo_custom_assertions() {
    std::cout << "=== Custom Assertions ===\n";
    
    int x = 42;
    EXPECT(x == 42, "x should be 42");
    EXPECT(x > 0, "x should be positive");
    
    std::cout << "Assertions passed!\n";
}

void demo() {
    std::cout << "=== Testing Frameworks Patterns ===\n";
    demo_custom_assertions();
}

} // namespace testing_frameworks

// ============================================
// 📌 UNIT TESTING PATTERNS
// ============================================

namespace unit_testing_patterns {

// AAA Pattern (Arrange-Act-Assert)
void demo_aaa_pattern() {
    std::cout << "\n=== AAA Pattern ===\n";
    
    // Arrange
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // Act
    int sum = 0;
    for (int n : numbers) {
        sum += n;
    }
    
    // Assert
    assert(sum == 15);
    std::cout << "Sum test passed\n";
}

// Test fixture
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

void demo_fixtures() {
    std::cout << "\n=== Test Fixtures ===\n";
    
    CalculatorFixture fixture;
    fixture.setup();
    
    // Test
    int result = fixture.a + fixture.b;
    assert(result == 30);
    
    fixture.teardown();
}

// Параметризованные тесты (pattern)
template<typename T>
void test_container_size() {
    T container;
    assert(container.empty());
    
    container.insert(container.end(), typename T::value_type{});
    assert(!container.empty());
    assert(container.size() == 1);
}

void demo_parameterized() {
    std::cout << "\n=== Parameterized Tests ===\n";
    
    test_container_size<std::vector<int>>();
    test_container_size<std::vector<std::string>>();
    
    std::cout << "Parameterized tests passed\n";
}

// Mock object pattern
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

void demo_mocking() {
    std::cout << "\n=== Mocking ===\n";
    
    MockDatabase mock;
    mock.expect_query("SELECT * FROM users", "Alice,Bob");
    
    std::string result = mock.query("SELECT * FROM users");
    assert(result == "Alice,Bob");
    assert(mock.was_called());
    
    std::cout << "Mock test passed\n";
}

void demo() {
    std::cout << "\n=== Unit Testing Patterns ===\n";
    demo_aaa_pattern();
    demo_fixtures();
    demo_parameterized();
    demo_mocking();
}

} // namespace unit_testing_patterns

// ============================================
// 📌 STATIC ANALYSIS
// ============================================

namespace static_analysis {

// static_assert - compile-time проверки
void demo_static_assert() {
    std::cout << "\n=== static_assert ===\n";
    
    static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
    static_assert(sizeof(void*) == 8, "64-bit platform required");
    
    std::cout << "Static assertions passed at compile time\n";
}

// Concepts для compile-time проверок
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template<Addable T>
T add(T a, T b) {
    return a + b;
}

void demo_concepts() {
    std::cout << "\n=== Concepts ===\n";
    
    int result = add(10, 20);  // ✅ Компилируется
    std::cout << "10 + 20 = " << result << '\n';
    
    // add("hello", "world");  // ❌ Ошибка компиляции
}

// Type trait assertions
template<typename T>
void process_trivial_type(T value) {
    static_assert(std::is_trivially_copyable_v<T>,
                  "T must be trivially copyable");
    
    std::cout << "Processing trivial type\n";
}

void demo_type_traits() {
    std::cout << "\n=== Type Traits ===\n";
    
    process_trivial_type(42);  // ✅ int is trivial
    // process_trivial_type(std::string("test"));  // ❌ string is not trivial
}

// constexpr unit tests
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr bool test_factorial() {
    return factorial(5) == 120 && factorial(0) == 1;
}

void demo_constexpr_tests() {
    std::cout << "\n=== Constexpr Tests ===\n";
    
    static_assert(test_factorial(), "Factorial test failed");
    std::cout << "Constexpr tests passed at compile time\n";
}

void demo() {
    std::cout << "\n=== Static Analysis ===\n";
    demo_static_assert();
    demo_concepts();
    demo_type_traits();
    demo_constexpr_tests();
}

} // namespace static_analysis

// ============================================
// 📌 RUNTIME DEBUGGING
// ============================================

namespace runtime_debugging {

// assert() - runtime проверки
void demo_assert() {
    std::cout << "\n=== assert() ===\n";
    
    int x = 42;
    assert(x > 0);  // Выполняется только в debug builds
    
    std::cout << "Assertions enabled in debug build\n";
    
    // NDEBUG отключает assert
    #ifdef NDEBUG
    std::cout << "NDEBUG defined - assertions disabled\n";
    #else
    std::cout << "Debug mode - assertions enabled\n";
    #endif
}

// source_location (C++20) - информация о месте вызова
#ifdef __cpp_lib_source_location
void log_message(const std::string& msg,
                 const std::source_location& loc = std::source_location::current()) {
    std::cout << "["
              << loc.file_name() << ":"
              << loc.line() << " in "
              << loc.function_name() << "] "
              << msg << '\n';
}

void demo_source_location() {
    std::cout << "\n=== source_location ===\n";
    
    log_message("This is a debug message");
    log_message("Another message");
}
#endif

// Логирование
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

void demo_logging() {
    std::cout << "\n=== Logging ===\n";
    
    Logger logger;
    
    logger.log(LogLevel::DEBUG, "Debug message");  // Не выводится
    logger.log(LogLevel::INFO, "Application started");
    logger.log(LogLevel::WARNING, "Low memory: ", 512, " MB");
    logger.log(LogLevel::ERROR, "Failed to open file");
    
    // Включить DEBUG
    logger.set_level(LogLevel::DEBUG);
    logger.log(LogLevel::DEBUG, "Now debug is visible");
}

// Debug vs Release builds
void demo_debug_builds() {
    std::cout << "\n=== Debug Builds ===\n";
    
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
}

void demo() {
    std::cout << "\n=== Runtime Debugging ===\n";
    demo_assert();
    #ifdef __cpp_lib_source_location
    demo_source_location();
    #endif
    demo_logging();
    demo_debug_builds();
}

} // namespace runtime_debugging

// ============================================
// 📌 SANITIZERS
// ============================================

namespace sanitizers {

/*
 * ADDRESSSANITIZER (ASan) - детектор ошибок памяти
 * Компиляция: g++ -fsanitize=address -g testing_debugging.cpp
 * 
 * Находит:
 * - Use-after-free
 * - Heap buffer overflow
 * - Stack buffer overflow
 * - Memory leaks
 */

void demo_asan() {
    std::cout << "\n=== AddressSanitizer ===\n";
    
    // ❌ Пример ошибки (закомментировано для безопасности)
    // int* ptr = new int(42);
    // delete ptr;
    // std::cout << *ptr << '\n';  // Use-after-free!
    
    // ✅ Правильный код
    std::unique_ptr<int> ptr = std::make_unique<int>(42);
    std::cout << "Value: " << *ptr << '\n';
    
    std::cout << "Compile with -fsanitize=address to detect memory errors\n";
}

/*
 * THREADSANITIZER (TSan) - детектор data races
 * Компиляция: g++ -fsanitize=thread -g testing_debugging.cpp
 */

void demo_tsan() {
    std::cout << "\n=== ThreadSanitizer ===\n";
    std::cout << "Compile with -fsanitize=thread to detect data races\n";
}

/*
 * UNDEFINEDBEHAVIORSANITIZER (UBSan)
 * Компиляция: g++ -fsanitize=undefined -g testing_debugging.cpp
 * 
 * Находит:
 * - Integer overflow
 * - Division by zero
 * - Null pointer dereference
 * - Invalid shifts
 */

void demo_ubsan() {
    std::cout << "\n=== UndefinedBehaviorSanitizer ===\n";
    
    // ✅ Безопасный код
    int x = 100;
    int y = 2;
    std::cout << "100 / 2 = " << x / y << '\n';
    
    std::cout << "Compile with -fsanitize=undefined to detect UB\n";
}

void demo() {
    std::cout << "\n=== Sanitizers ===\n";
    demo_asan();
    demo_tsan();
    demo_ubsan();
    
    std::cout << "\nAvailable sanitizers:\n";
    std::cout << "  -fsanitize=address     (ASan)\n";
    std::cout << "  -fsanitize=thread      (TSan)\n";
    std::cout << "  -fsanitize=undefined   (UBSan)\n";
    std::cout << "  -fsanitize=memory      (MSan, Clang only)\n";
    std::cout << "  -fsanitize=leak        (LSan)\n";
}

} // namespace sanitizers

// ============================================
// 📌 PROFILING И BENCHMARKING
// ============================================

namespace profiling {

// Простой benchmark helper
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

void demo_benchmarking() {
    std::cout << "\n=== Benchmarking ===\n";
    
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
}

void demo_profiling_tools() {
    std::cout << "\n=== Profiling Tools ===\n";
    
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
}

void demo() {
    std::cout << "\n=== Profiling & Benchmarking ===\n";
    demo_benchmarking();
    demo_profiling_tools();
}

} // namespace profiling

// ============================================
// 📌 DEBUGGING TECHNIQUES
// ============================================

namespace debugging_techniques {

// Printf debugging
void demo_printf_debugging() {
    std::cout << "\n=== Printf Debugging ===\n";
    
    int x = 10;
    std::cout << "DEBUG: x = " << x << '\n';
    
    x *= 2;
    std::cout << "DEBUG: after multiply, x = " << x << '\n';
    
    #ifdef __cpp_lib_print
    // C++23 std::print
    // std::print("x = {}\n", x);
    #endif
}

// Conditional compilation
void demo_conditional_compilation() {
    std::cout << "\n=== Conditional Compilation ===\n";
    
    #ifdef DEBUG
    std::cout << "Debug code is active\n";
    #endif
    
    #if defined(__GNUC__)
    std::cout << "Compiled with GCC/G++\n";
    #elif defined(_MSC_VER)
    std::cout << "Compiled with MSVC\n";
    #endif
}

// Debug macros
#ifndef NDEBUG
    #define DEBUG_PRINT(x) std::cout << "DEBUG: " << #x << " = " << (x) << '\n'
#else
    #define DEBUG_PRINT(x)
#endif

void demo_debug_macros() {
    std::cout << "\n=== Debug Macros ===\n";
    
    int value = 42;
    DEBUG_PRINT(value);
    DEBUG_PRINT(value * 2);
}

void demo() {
    std::cout << "\n=== Debugging Techniques ===\n";
    demo_printf_debugging();
    demo_conditional_compilation();
    demo_debug_macros();
}

} // namespace debugging_techniques

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

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== C++ Testing & Debugging ===\n";
    
    testing_frameworks::demo();
    unit_testing_patterns::demo();
    static_analysis::demo();
    runtime_debugging::demo();
    sanitizers::demo();
    profiling::demo();
    debugging_techniques::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ Testing frameworks: Catch2, GTest, doctest\n";
    std::cout << "✓ Static analysis: static_assert, concepts\n";
    std::cout << "✓ Runtime: assert, source_location, logging\n";
    std::cout << "✓ Sanitizers: ASan, TSan, UBSan\n";
    std::cout << "✓ Profiling: perf, Valgrind, benchmarks\n";
    std::cout << "✓ CI/CD: автоматические тесты + sanitizers\n";
    
    return 0;
}