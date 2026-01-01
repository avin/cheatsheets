/*
 * ============================================
 * ОПТИМИЗАЦИЯ ПРОИЗВОДИТЕЛЬНОСТИ C++
 * ============================================
 * 
 * Практические техники оптимизации:
 * - Move semantics и copy elision
 * - Memory optimization (cache-friendly code)
 * - Compile-time computation
 * - Branch prediction
 * - Profiling и измерения
 * 
 * Компиляция: g++ -std=c++20 -O3 performance_tricks.cpp
 */

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <memory>
#include <numeric>
#include <cstring>

// ============================================
// 📌 MOVE SEMANTICS OPTIMIZATION
// ============================================

namespace move_semantics {

// ❌ Без move semantics
class BigDataOld {
    int* data_;
    size_t size_;
    
public:
    BigDataOld(size_t size) : size_(size) {
        data_ = new int[size_];
        std::cout << "Allocating " << size_ << " ints\n";
    }
    
    ~BigDataOld() { delete[] data_; }
    
    // Copy - дорого!
    BigDataOld(const BigDataOld& other) : size_(other.size_) {
        data_ = new int[size_];
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "Copying " << size_ << " ints\n";
    }
};

// ✅ С move semantics
class BigData {
    int* data_;
    size_t size_;
    
public:
    BigData(size_t size) : size_(size) {
        data_ = new int[size_];
    }
    
    ~BigData() { delete[] data_; }
    
    // Copy constructor
    BigData(const BigData& other) : size_(other.size_) {
        data_ = new int[size_];
        std::copy(other.data_, other.data_ + size_, data_);
    }
    
    // Move constructor - дешево!
    BigData(BigData&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    // Move assignment
    BigData& operator=(BigData&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
};

// RVO (Return Value Optimization) - гарантирован в C++17
BigData create_big_data(size_t size) {
    return BigData(size);  // Никакого копирования!
}

// Perfect forwarding
template<typename T>
void process(T&& value) {
    // Передает lvalue как lvalue, rvalue как rvalue
    auto result = std::forward<T>(value);
}

void demo() {
    std::cout << "=== Move Semantics ===\n";
    
    // Move вместо copy
    BigData data1(1000);
    BigData data2 = std::move(data1);  // Move, не copy!
    
    // RVO
    auto data3 = create_big_data(1000);  // Никакого копирования
}

} // namespace move_semantics

// ============================================
// 📌 MEMORY OPTIMIZATION
// ============================================

namespace memory_optimization {

// ❌ Плохо для кэша - "Array of Structures"
struct ParticleAoS {
    float x, y, z;      // Позиция
    float vx, vy, vz;   // Скорость
    int id;             // ID
    float padding;      // Выравнивание
};

// ✅ Хорошо для кэша - "Structure of Arrays"
struct ParticlesSoA {
    std::vector<float> x, y, z;       // Позиции
    std::vector<float> vx, vy, vz;    // Скорости
    std::vector<int> id;              // IDs
    
    void resize(size_t n) {
        x.resize(n); y.resize(n); z.resize(n);
        vx.resize(n); vy.resize(n); vz.resize(n);
        id.resize(n);
    }
    
    // Обновление позиций - последовательный доступ к памяти!
    void update_positions(float dt) {
        for (size_t i = 0; i < x.size(); ++i) {
            x[i] += vx[i] * dt;
            y[i] += vy[i] * dt;
            z[i] += vz[i] * dt;
        }
    }
};

// Memory alignment для SIMD
struct alignas(32) AlignedData {
    float data[8];  // Выровнено для AVX
};

// Демонстрация padding
struct BadPacking {
    char c;    // 1 byte
    // 3 bytes padding
    int i;     // 4 bytes
    char c2;   // 1 byte
    // 3 bytes padding
};  // Итого: 12 bytes

struct GoodPacking {
    int i;     // 4 bytes
    char c;    // 1 byte
    char c2;   // 1 byte
    // 2 bytes padding
};  // Итого: 8 bytes

void demo() {
    std::cout << "\n=== Memory Optimization ===\n";
    
    std::cout << "BadPacking size: " << sizeof(BadPacking) << " bytes\n";
    std::cout << "GoodPacking size: " << sizeof(GoodPacking) << " bytes\n";
    
    // SoA лучше для кэша при последовательной обработке
    ParticlesSoA particles;
    particles.resize(10000);
    particles.update_positions(0.016f);  // Кэш-френдли!
}

} // namespace memory_optimization

// ============================================
// 📌 COMPILE-TIME COMPUTATION
// ============================================

namespace compile_time {

// constexpr - вычисляется в compile-time
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// consteval - ГАРАНТИРОВАННО в compile-time (C++20)
consteval int square(int n) {
    return n * n;
}

// Compile-time string hash
constexpr size_t hash_string(const char* str) {
    size_t hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + *str++;
    }
    return hash;
}

// Использование для switch на строках
void process_command(const char* cmd) {
    switch (hash_string(cmd)) {
        case hash_string("start"):
            std::cout << "Starting...\n";
            break;
        case hash_string("stop"):
            std::cout << "Stopping...\n";
            break;
        default:
            std::cout << "Unknown command\n";
    }
}

void demo() {
    std::cout << "\n=== Compile-Time Computation ===\n";
    
    constexpr int fact10 = factorial(10);  // Вычислено в compile-time!
    std::cout << "factorial(10) = " << fact10 << '\n';
    
    process_command("start");
}

} // namespace compile_time

// ============================================
// 📌 COPY ELISION (C++17)
// ============================================

namespace copy_elision {

struct Expensive {
    std::vector<int> data;
    
    Expensive(size_t size) : data(size) {
        std::cout << "Constructing Expensive\n";
    }
    
    Expensive(const Expensive&) {
        std::cout << "Copying Expensive\n";
    }
    
    Expensive(Expensive&&) noexcept {
        std::cout << "Moving Expensive\n";
    }
};

// Гарантированная copy elision (C++17)
Expensive create_expensive() {
    return Expensive(1000);  // Никакого move/copy!
}

Expensive factory(bool flag) {
    if (flag) {
        return Expensive(100);  // NRVO может применяться
    }
    return Expensive(200);      // но не гарантировано
}

void demo() {
    std::cout << "\n=== Copy Elision ===\n";
    
    auto obj = create_expensive();  // Только один конструктор!
}

} // namespace copy_elision

// ============================================
// 📌 SMALL OBJECT OPTIMIZATION
// ============================================

namespace small_object_optimization {

// Упрощенная реализация SSO (Small String Optimization)
class SmallString {
    static constexpr size_t SMALL_SIZE = 15;
    
    union {
        char* heap_ptr;
        char buffer[SMALL_SIZE + 1];
    };
    size_t size_;
    bool is_small_;
    
public:
    SmallString(const char* str) {
        size_ = std::strlen(str);
        
        if (size_ <= SMALL_SIZE) {
            // Маленькая строка - в стек!
            is_small_ = true;
            std::strcpy(buffer, str);
        } else {
            // Большая строка - в кучу
            is_small_ = false;
            heap_ptr = new char[size_ + 1];
            std::strcpy(heap_ptr, str);
        }
    }
    
    ~SmallString() {
        if (!is_small_) {
            delete[] heap_ptr;
        }
    }
    
    const char* c_str() const {
        return is_small_ ? buffer : heap_ptr;
    }
    
    bool uses_heap() const { return !is_small_; }
};

void demo() {
    std::cout << "\n=== Small Object Optimization ===\n";
    
    SmallString small("Hello");           // В стеке
    SmallString large("This is a very long string that won't fit in SSO");  // В куче
    
    std::cout << "Small uses heap: " << std::boolalpha << small.uses_heap() << '\n';
    std::cout << "Large uses heap: " << large.uses_heap() << '\n';
}

} // namespace small_object_optimization

// ============================================
// 📌 BRANCH PREDICTION (C++20)
// ============================================

namespace branch_prediction {

// [[likely]] / [[unlikely]] hints (C++20)
int process_value(int x) {
    if (x > 0) [[likely]] {
        // Обычный случай
        return x * 2;
    } else [[unlikely]] {
        // Редкий случай
        return 0;
    }
}

// Branch-free альтернатива
int clamp_branchfree(int value, int min, int max) {
    // Без if - может быть быстрее
    value = value < min ? min : value;
    value = value > max ? max : value;
    return value;
}

// С ветвлением
int clamp_branchy(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void demo() {
    std::cout << "\n=== Branch Prediction ===\n";
    
    std::cout << "process_value(5): " << process_value(5) << '\n';
    std::cout << "clamp(15, 0, 10): " << clamp_branchfree(15, 0, 10) << '\n';
}

} // namespace branch_prediction

// ============================================
// 📌 STRING OPTIMIZATION
// ============================================

namespace string_optimization {

// ❌ Плохо - много копирований
std::string process_bad(const std::string& input) {
    std::string result = input;     // Копирование
    result = result + " processed"; // Копирование
    return result;                  // Move (хорошо)
}

// ✅ Хорошо - string_view
void process_good(std::string_view input) {
    std::cout << input << " processed\n";  // Никаких копий!
}

// ✅ In-place модификация
void to_upper_inplace(std::string& str) {
    for (char& c : str) {
        c = std::toupper(c);
    }
}

// reserve() для избежания реаллокаций
std::string build_string_optimized(const std::vector<std::string>& parts) {
    // Подсчитываем общий размер
    size_t total_size = 0;
    for (const auto& part : parts) {
        total_size += part.size();
    }
    
    std::string result;
    result.reserve(total_size);  // Одна аллокация!
    
    for (const auto& part : parts) {
        result += part;
    }
    
    return result;
}

void demo() {
    std::cout << "\n=== String Optimization ===\n";
    
    std::string text = "hello";
    process_good(text);  // Эффективно
    
    to_upper_inplace(text);
    std::cout << "Upper: " << text << '\n';
}

} // namespace string_optimization

// ============================================
// 📌 CONTAINER OPTIMIZATION
// ============================================

namespace container_optimization {

void vector_reserve_demo() {
    std::cout << "\n=== Vector Reserve ===\n";
    
    // ❌ Без reserve - множественные реаллокации
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> vec1;
    for (int i = 0; i < 100000; ++i) {
        vec1.push_back(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // ✅ С reserve - одна аллокация
    start = std::chrono::high_resolution_clock::now();
    std::vector<int> vec2;
    vec2.reserve(100000);
    for (int i = 0; i < 100000; ++i) {
        vec2.push_back(i);
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Without reserve: " << duration1.count() << " μs\n";
    std::cout << "With reserve: " << duration2.count() << " μs\n";
}

void emplace_vs_push() {
    std::cout << "\n=== emplace vs push ===\n";
    
    struct Point {
        int x, y;
        Point(int x_, int y_) : x(x_), y(y_) {
            std::cout << "Point(" << x << ", " << y << ")\n";
        }
    };
    
    std::vector<Point> vec;
    
    // push_back - создает временный объект
    std::cout << "push_back:\n";
    vec.push_back(Point(1, 2));  // Конструктор + move
    
    // emplace_back - конструирует in-place
    std::cout << "emplace_back:\n";
    vec.emplace_back(3, 4);  // Только конструктор!
}

void demo() {
    vector_reserve_demo();
    emplace_vs_push();
}

} // namespace container_optimization

// ============================================
// 📌 PROFILING И BENCHMARKING
// ============================================

namespace profiling {

// Простой benchmark helper
template<typename Func>
auto benchmark(Func f, int iterations = 1000) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        f();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

void demo() {
    std::cout << "\n=== Profiling ===\n";
    
    // Бенчмарк vector vs list
    auto vector_time = benchmark([]() {
        std::vector<int> vec;
        for (int i = 0; i < 1000; ++i) {
            vec.push_back(i);
        }
    });
    
    std::cout << "Vector operation: " << vector_time.count() << " μs\n";
    
    /*
     * ИНСТРУМЕНТЫ ПРОФИЛИРОВАНИЯ:
     * 
     * CPU Profilers:
     * - perf (Linux): perf record ./app && perf report
     * - VTune (Intel): Детальный анализ
     * - gprof (GNU): g++ -pg, затем gprof
     * 
     * Memory Profilers:
     * - Valgrind: valgrind --tool=massif ./app
     * - AddressSanitizer: g++ -fsanitize=address
     * - Heaptrack: heaptrack ./app
     * 
     * Compiler Flags:
     * -O0: No optimization (debug)
     * -O1: Basic optimization
     * -O2: Recommended для production
     * -O3: Aggressive optimization
     * -Os: Optimize for size
     * -Ofast: -O3 + fast-math (может нарушить стандарты)
     * -flto: Link-Time Optimization
     * 
     * Profile-Guided Optimization (PGO):
     * 1. g++ -fprofile-generate -O2 app.cpp
     * 2. ./a.out  (собрать профиль)
     * 3. g++ -fprofile-use -O2 app.cpp
     */
}

} // namespace profiling

// ============================================
// 📌 COMMON PITFALLS
// ============================================

namespace common_pitfalls {

void premature_optimization() {
    std::cout << "\n=== Common Pitfalls ===\n\n";
    
    std::cout << "1. PREMATURE OPTIMIZATION\n";
    std::cout << "   'Premature optimization is the root of all evil' - Donald Knuth\n";
    std::cout << "   ✓ Сначала профилируйте\n";
    std::cout << "   ✓ Оптимизируйте только hotspots\n";
    std::cout << "   ✗ Не оптимизируйте без измерений\n\n";
    
    std::cout << "2. FALSE SHARING\n";
    std::cout << "   Проблема: Разные потоки модифицируют соседние переменные\n";
    std::cout << "   Решение: Padding или alignas(64)\n\n";
    
    std::cout << "3. CACHE MISSES\n";
    std::cout << "   ✗ Разреженный доступ к памяти\n";
    std::cout << "   ✓ Последовательный доступ (SoA)\n\n";
    
    std::cout << "4. МИКРООПТИМИЗАЦИИ\n";
    std::cout << "   ✗ x * 0.5 вместо x / 2 (компилятор сам оптимизирует)\n";
    std::cout << "   ✗ Ручная loop unrolling (компилятор лучше)\n\n";
    
    std::cout << "5. ИЗБЫТОЧНЫЕ АЛЛОКАЦИИ\n";
    std::cout << "   ✓ reserve() для vector\n";
    std::cout << "   ✓ Object pooling для частых аллокаций\n";
    std::cout << "   ✓ Small buffer optimization\n";
}

void demo() {
    premature_optimization();
}

} // namespace common_pitfalls

// ============================================
// 📌 BEST PRACTICES
// ============================================

/*
 * ЗОЛОТЫЕ ПРАВИЛА ОПТИМИЗАЦИИ:
 * 
 * 1. ИЗМЕРЯЙ, НЕ ГАДАЙ
 *    - Профилируй перед оптимизацией
 *    - Измеряй после оптимизации
 *    - Реальные данные, не synthetic benchmarks
 * 
 * 2. ОПТИМИЗИРУЙ АЛГОРИТМ СНАЧАЛА
 *    - O(n²) → O(n log n) важнее микрооптимизаций
 *    - Правильная структура данных = 10x speedup
 * 
 * 3. ПОМНИ О КЭШЕ
 *    - Последовательный доступ к памяти
 *    - Structure of Arrays для больших данных
 *    - Избегай cache line ping-pong
 * 
 * 4. ИСПОЛЬЗУЙ MOVE SEMANTICS
 *    - std::move для rvalues
 *    - noexcept для move constructors
 *    - RVO/NRVO автоматически в большинстве случаев
 * 
 * 5. COMPILE-TIME ГДЕ ВОЗМОЖНО
 *    - constexpr для compile-time вычислений
 *    - Template metaprogramming для zero-overhead
 * 
 * 6. ИЗБЕГАЙ АЛЛОКАЦИЙ
 *    - reserve() для контейнеров
 *    - emplace вместо push + move
 *    - Small buffer optimization
 * 
 * 7. КОМПИЛЯТОР - ТВОЙ ДРУГ
 *    - -O2/-O3 для production
 *    - LTO для межмодульной оптимизации
 *    - PGO для критичного кода
 * 
 * 8. ПАРАЛЛЕЛИЗМ С УМОМ
 *    - Parallel algorithms для больших данных
 *    - Учитывай overhead создания потоков
 *    - Избегай false sharing
 */

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== C++ Performance Optimization ===\n";
    
    move_semantics::demo();
    memory_optimization::demo();
    compile_time::demo();
    copy_elision::demo();
    small_object_optimization::demo();
    branch_prediction::demo();
    string_optimization::demo();
    container_optimization::demo();
    profiling::demo();
    common_pitfalls::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ Профилируй перед оптимизацией\n";
    std::cout << "✓ Алгоритм важнее микрооптимизаций\n";
    std::cout << "✓ Move semantics и copy elision бесплатны\n";
    std::cout << "✓ Cache-friendly структуры данных (SoA)\n";
    std::cout << "✓ constexpr для compile-time вычислений\n";
    std::cout << "✓ reserve() для контейнеров\n";
    std::cout << "✓ emplace вместо push\n";
    std::cout << "✓ string_view вместо const string&\n";
    std::cout << "✓ -O2/-O3 + LTO для production\n";
    
    return 0;
}
// • Lock contention
// • Memory fragmentation
