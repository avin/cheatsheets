/*
 * ============================================
 * ШАБЛОНЫ И МЕТАПРОГРАММИРОВАНИЕ C++
 * ============================================
 * 
 * Полное руководство по template metaprogramming:
 * - Concepts (C++20)
 * - Type traits
 * - constexpr/consteval/constinit
 * - SFINAE vs Concepts
 * - Advanced patterns (CRTP, type erasure)
 * 
 * Требования: C++20 для concepts
 * Компиляция: g++ -std=c++20 metaprogramming.cpp
 */

#include <iostream>
#include <type_traits>
#include <concepts>
#include <vector>
#include <string>
#include <memory>
#include <utility>

// ============================================
// 📌 TEMPLATE BASICS
// ============================================

namespace template_basics {

// Function template
template<typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

// Class template
template<typename T>
class Stack {
    std::vector<T> data_;
    
public:
    void push(const T& value) { data_.push_back(value); }
    void pop() { data_.pop_back(); }
    const T& top() const { return data_.back(); }
    bool empty() const { return data_.empty(); }
};

// Variable template (C++14)
template<typename T>
constexpr T pi = T(3.1415926535897932385);

// Template template parameter
template<typename T, template<typename> class Container>
class MyContainer {
    Container<T> data_;
    
public:
    void add(const T& value) { data_.push_back(value); }
};

// Variadic templates
template<typename... Args>
void print(Args... args) {
    // Fold expression (C++17)
    (std::cout << ... << args) << '\n';
}

template<typename... Args>
auto sum(Args... args) {
    // Fold expression
    return (... + args);
}

void demo() {
    std::cout << "=== Template Basics ===\n";
    
    std::cout << "max(5, 10): " << max(5, 10) << '\n';
    std::cout << "pi<double>: " << pi<double> << '\n';
    
    print("Hello", " ", "World", "!");
    std::cout << "sum(1, 2, 3, 4, 5): " << sum(1, 2, 3, 4, 5) << '\n';
}

} // namespace template_basics

// ============================================
// 📌 CONCEPTS (C++20)
// ============================================

namespace concepts_demo {

// Простой concept
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

// Concept с requires
template<typename T>
concept Printable = requires(T t, std::ostream& os) {
    { os << t } -> std::convertible_to<std::ostream&>;
};

// Составной concept
template<typename T>
concept Incrementable = requires(T t) {
    { ++t } -> std::same_as<T&>;
    { t++ } -> std::same_as<T>;
};

// Concept с несколькими требованиями
template<typename T>
concept Container = requires(T t) {
    typename T::value_type;
    typename T::iterator;
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
    { t.size() } -> std::convertible_to<std::size_t>;
};

// Использование concepts в функциях

// Способ 1: Requires clause
template<typename T>
    requires Numeric<T>
T add(T a, T b) {
    return a + b;
}

// Способ 2: Trailing requires
template<typename T>
T multiply(T a, T b) requires Numeric<T> {
    return a * b;
}

// Способ 3: Abbreviated function template
auto divide(Numeric auto a, Numeric auto b) {
    return a / b;
}

// Concept-based overloading
void process(Numeric auto value) {
    std::cout << "Processing number: " << value << '\n';
}

void process(Printable auto value) {
    std::cout << "Processing printable: " << value << '\n';
}

// Subsumption - более специфичный concept выбирается
template<typename T>
concept SignedNumeric = Numeric<T> && std::is_signed_v<T>;

void handle(Numeric auto value) {
    std::cout << "Numeric\n";
}

void handle(SignedNumeric auto value) {
    std::cout << "Signed numeric\n";  // Выберется для signed типов
}

// Кастомный concept для итераторов
template<typename T>
concept RandomAccessIterator = std::random_access_iterator<T>;

template<RandomAccessIterator Iter>
void sort_range(Iter begin, Iter end) {
    std::cout << "Sorting with random access iterator\n";
}

void demo() {
    std::cout << "\n=== Concepts ===\n";
    
    std::cout << add(5, 10) << '\n';
    std::cout << multiply(2.5, 4.0) << '\n';
    std::cout << divide(10, 2) << '\n';
    
    handle(5);      // Signed numeric
    handle(5u);     // Numeric
    
    process(42);
    process("text");
}

} // namespace concepts_demo

// ============================================
// 📌 TYPE TRAITS
// ============================================

namespace type_traits_demo {

// Кастомный type trait
template<typename T>
struct is_pointer_like {
    static constexpr bool value = std::is_pointer_v<T>;
};

template<typename T>
struct is_pointer_like<std::unique_ptr<T>> {
    static constexpr bool value = true;
};

template<typename T>
struct is_pointer_like<std::shared_ptr<T>> {
    static constexpr bool value = true;
};

template<typename T>
inline constexpr bool is_pointer_like_v = is_pointer_like<T>::value;

// Type transformation trait
template<typename T>
struct remove_all_pointers {
    using type = T;
};

template<typename T>
struct remove_all_pointers<T*> {
    using type = typename remove_all_pointers<T>::type;
};

template<typename T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;

// Value metafunction
template<typename T>
struct type_size {
    static constexpr std::size_t value = sizeof(T);
};

// Type metafunction
template<bool Condition, typename TrueType, typename FalseType>
struct conditional {
    using type = TrueType;
};

template<typename TrueType, typename FalseType>
struct conditional<false, TrueType, FalseType> {
    using type = FalseType;
};

template<bool Condition, typename TrueType, typename FalseType>
using conditional_t = typename conditional<Condition, TrueType, FalseType>::type;

void demo() {
    std::cout << "\n=== Type Traits ===\n";
    
    // Standard traits
    std::cout << "int is integral: " 
              << std::boolalpha << std::is_integral_v<int> << '\n';
    std::cout << "double is floating point: " 
              << std::is_floating_point_v<double> << '\n';
    
    // Custom traits
    std::cout << "int* is pointer-like: " 
              << is_pointer_like_v<int*> << '\n';
    std::cout << "unique_ptr is pointer-like: " 
              << is_pointer_like_v<std::unique_ptr<int>> << '\n';
    
    // Type transformations
    static_assert(std::is_same_v<remove_all_pointers_t<int***>, int>);
    std::cout << "remove_all_pointers works correctly\n";
    
    // Conditional type
    using type = conditional_t<true, int, double>;
    static_assert(std::is_same_v<type, int>);
}

} // namespace type_traits_demo

// ============================================
// 📌 SFINAE VS CONCEPTS
// ============================================

namespace sfinae_vs_concepts {

// ❌ Старый способ - SFINAE с enable_if
template<typename T>
std::enable_if_t<std::is_integral_v<T>, void>
print_old(T value) {
    std::cout << "Integer: " << value << '\n';
}

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, void>
print_old(T value) {
    std::cout << "Float: " << value << '\n';
}

// ✅ Современный способ - Concepts
template<std::integral T>
void print_new(T value) {
    std::cout << "Integer: " << value << '\n';
}

template<std::floating_point T>
void print_new(T value) {
    std::cout << "Float: " << value << '\n';
}

// SFINAE для проверки наличия метода
template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> 
    : std::true_type {};

template<typename T>
inline constexpr bool has_size_v = has_size<T>::value;

// То же через concept (лучше!)
template<typename T>
concept HasSize = requires(T t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

void demo() {
    std::cout << "\n=== SFINAE vs Concepts ===\n";
    
    print_new(42);
    print_new(3.14);
    
    std::cout << "vector has size: " 
              << std::boolalpha << has_size_v<std::vector<int>> << '\n';
    std::cout << "int has size: " 
              << has_size_v<int> << '\n';
}

} // namespace sfinae_vs_concepts

// ============================================
// 📌 COMPILE-TIME PROGRAMMING
// ============================================

namespace compile_time {

// constexpr функция
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// constexpr переменная
constexpr int value = factorial(5);

// constexpr if (C++17)
template<typename T>
auto get_value(T t) {
    if constexpr (std::is_pointer_v<T>) {
        return *t;  // Для указателей
    } else {
        return t;   // Для не-указателей
    }
}

// consteval (C++20) - только compile-time
consteval int square(int n) {
    return n * n;
}

// constinit (C++20) - гарантирует статическую инициализацию
constinit int global = 42;

// C++20: constexpr new/delete
constexpr int dynamic_allocation() {
    int* ptr = new int(42);
    int value = *ptr;
    delete ptr;
    return value;
}

// Compile-time строки
template<std::size_t N>
struct CompileTimeString {
    char data[N];
    
    constexpr CompileTimeString(const char (&str)[N]) {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] = str[i];
        }
    }
};

// C++20: constexpr vector (!)
constexpr int vector_sum() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    int sum = 0;
    for (int x : vec) {
        sum += x;
    }
    return sum;
}

void demo() {
    std::cout << "\n=== Compile-time Programming ===\n";
    
    std::cout << "factorial(5): " << value << '\n';
    
    int x = 10;
    std::cout << "get_value(&x): " << get_value(&x) << '\n';
    std::cout << "get_value(x): " << get_value(x) << '\n';
    
    constexpr int sq = square(10);  // Вычисляется в compile-time
    std::cout << "square(10): " << sq << '\n';
    
    constexpr int dyn = dynamic_allocation();
    std::cout << "dynamic allocation result: " << dyn << '\n';
    
    constexpr int sum = vector_sum();
    std::cout << "vector_sum(): " << sum << '\n';
}

} // namespace compile_time

// ============================================
// 📌 ADVANCED TEMPLATE TECHNIQUES
// ============================================

namespace advanced_patterns {

// CRTP - Curiously Recurring Template Pattern
template<typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
    
    void common_functionality() {
        std::cout << "Common functionality\n";
    }
};

class Derived : public Base<Derived> {
public:
    void implementation() {
        std::cout << "Derived implementation\n";
    }
};

// CRTP для mixin
template<typename Derived>
class Printable {
public:
    void print() const {
        std::cout << static_cast<const Derived*>(this)->to_string() << '\n';
    }
};

class MyClass : public Printable<MyClass> {
    int value_;
    
public:
    MyClass(int v) : value_(v) {}
    
    std::string to_string() const {
        return "MyClass(" + std::to_string(value_) + ")";
    }
};

// Tag dispatch
struct input_iterator_tag {};
struct random_access_iterator_tag : input_iterator_tag {};

template<typename Iter>
void advance_impl(Iter& it, int n, input_iterator_tag) {
    std::cout << "Advancing input iterator\n";
    while (n--) ++it;
}

template<typename Iter>
void advance_impl(Iter& it, int n, random_access_iterator_tag) {
    std::cout << "Advancing random access iterator\n";
    it += n;
}

template<typename Iter>
void advance(Iter& it, int n) {
    advance_impl(it, n, typename std::iterator_traits<Iter>::iterator_category{});
}

// Policy-based design
template<typename OutputPolicy>
class Logger : private OutputPolicy {
public:
    void log(const std::string& msg) {
        this->output(msg);
    }
};

class ConsoleOutput {
protected:
    void output(const std::string& msg) {
        std::cout << "Console: " << msg << '\n';
    }
};

class FileOutput {
protected:
    void output(const std::string& msg) {
        std::cout << "File: " << msg << '\n';
    }
};

// Type erasure (упрощенная версия std::function)
class AnyCallable {
    struct CallableBase {
        virtual ~CallableBase() = default;
        virtual void call() = 0;
    };
    
    template<typename F>
    struct CallableImpl : CallableBase {
        F func_;
        CallableImpl(F f) : func_(std::move(f)) {}
        void call() override { func_(); }
    };
    
    std::unique_ptr<CallableBase> callable_;
    
public:
    template<typename F>
    AnyCallable(F f) : callable_(std::make_unique<CallableImpl<F>>(std::move(f))) {}
    
    void operator()() {
        callable_->call();
    }
};

void demo() {
    std::cout << "\n=== Advanced Patterns ===\n";
    
    // CRTP
    Derived d;
    d.interface();
    d.common_functionality();
    
    // Mixin
    MyClass obj(42);
    obj.print();
    
    // Policy-based design
    Logger<ConsoleOutput> console_logger;
    console_logger.log("Hello");
    
    Logger<FileOutput> file_logger;
    file_logger.log("World");
    
    // Type erasure
    AnyCallable callable([]{ std::cout << "Lambda called\n"; });
    callable();
}

} // namespace advanced_patterns

// ============================================
// 📌 TEMPLATE SPECIALIZATION
// ============================================

namespace template_specialization {

// Primary template
template<typename T>
class Serializer {
public:
    static std::string serialize(const T& value) {
        return "Generic serialization";
    }
};

// Full specialization для int
template<>
class Serializer<int> {
public:
    static std::string serialize(const int& value) {
        return "int: " + std::to_string(value);
    }
};

// Full specialization для std::string
template<>
class Serializer<std::string> {
public:
    static std::string serialize(const std::string& value) {
        return "string: \"" + value + "\"";
    }
};

// Partial specialization для указателей
template<typename T>
class Serializer<T*> {
public:
    static std::string serialize(T* const& ptr) {
        if (ptr) {
            return "pointer to: " + Serializer<T>::serialize(*ptr);
        }
        return "nullptr";
    }
};

// Variable template specialization
template<typename T>
constexpr bool is_small = sizeof(T) <= 4;

template<>
constexpr bool is_small<std::string> = false;  // Всегда большой

void demo() {
    std::cout << "\n=== Template Specialization ===\n";
    
    std::cout << Serializer<int>::serialize(42) << '\n';
    std::cout << Serializer<std::string>::serialize("hello") << '\n';
    
    int value = 100;
    std::cout << Serializer<int*>::serialize(&value) << '\n';
    
    std::cout << "int is small: " << std::boolalpha << is_small<int> << '\n';
    std::cout << "string is small: " << is_small<std::string> << '\n';
}

} // namespace template_specialization

// ============================================
// 📌 METAFUNCTIONS
// ============================================

namespace metafunctions {

// Value metafunction - возвращает значение
template<int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template<>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

template<>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

// Type metafunction - возвращает тип
template<typename... Types>
struct type_list {};

template<typename List>
struct front;

template<typename Head, typename... Tail>
struct front<type_list<Head, Tail...>> {
    using type = Head;
};

template<typename List>
using front_t = typename front<List>::type;

// Higher-order metafunction
template<template<typename> class F, typename List>
struct transform;

template<template<typename> class F, typename... Types>
struct transform<F, type_list<Types...>> {
    using type = type_list<typename F<Types>::type...>;
};

// Пример использования
template<typename T>
struct add_pointer {
    using type = T*;
};

// Compile-time list operations
template<typename List, typename T>
struct append;

template<typename... Types, typename T>
struct append<type_list<Types...>, T> {
    using type = type_list<Types..., T>;
};

template<typename List, typename T>
using append_t = typename append<List, T>::type;

// Compile-time вычисления
template<int... Values>
struct sum {
    static constexpr int value = (... + Values);
};

void demo() {
    std::cout << "\n=== Metafunctions ===\n";
    
    std::cout << "Fibonacci<10>: " << Fibonacci<10>::value << '\n';
    
    using list = type_list<int, double, char>;
    using first = front_t<list>;
    static_assert(std::is_same_v<first, int>);
    std::cout << "front_t works correctly\n";
    
    using transformed = transform<add_pointer, list>::type;
    using transformed_first = front_t<transformed>;
    static_assert(std::is_same_v<transformed_first, int*>);
    std::cout << "transform works correctly\n";
    
    std::cout << "sum<1,2,3,4,5>: " << sum<1,2,3,4,5>::value << '\n';
}

} // namespace metafunctions

// ============================================
// 📌 ПРАКТИЧЕСКИЕ ПРИМЕРЫ
// ============================================

namespace practical_examples {

// Compile-time string hash
constexpr std::size_t hash_string(const char* str) {
    std::size_t hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + *str++;
    }
    return hash;
}

// Использование для switch на строках
enum class MessageType {
    Hello = hash_string("hello"),
    Goodbye = hash_string("goodbye"),
    Error = hash_string("error")
};

void process_message(const char* msg) {
    switch (hash_string(msg)) {
        case static_cast<std::size_t>(MessageType::Hello):
            std::cout << "Hello message\n";
            break;
        case static_cast<std::size_t>(MessageType::Goodbye):
            std::cout << "Goodbye message\n";
            break;
        default:
            std::cout << "Unknown message\n";
    }
}

// Static reflection helper
template<typename T>
constexpr auto type_name() {
    #ifdef __GNUG__
    constexpr auto prefix = "constexpr auto practical_examples::type_name() [with T = ";
    constexpr auto suffix = "]";
    constexpr auto function = __PRETTY_FUNCTION__;
    #elif defined(_MSC_VER)
    constexpr auto prefix = "auto __cdecl practical_examples::type_name<";
    constexpr auto suffix = ">(void)";
    constexpr auto function = __FUNCSIG__;
    #endif
    
    return std::string_view(function);
}

// Compile-time unit system
template<int Mass, int Length, int Time>
struct Unit {
    double value;
    
    constexpr Unit(double v) : value(v) {}
    
    // Сложение только для одинаковых единиц
    constexpr Unit operator+(Unit other) const {
        return Unit(value + other.value);
    }
};

// Определяем типы
using Scalar = Unit<0, 0, 0>;
using Length = Unit<0, 1, 0>;
using Time = Unit<0, 0, 1>;
using Velocity = Unit<0, 1, -1>;  // Length / Time

template<int M1, int L1, int T1, int M2, int L2, int T2>
constexpr auto operator*(Unit<M1, L1, T1> a, Unit<M2, L2, T2> b) {
    return Unit<M1+M2, L1+L2, T1+T2>(a.value * b.value);
}

template<int M1, int L1, int T1, int M2, int L2, int T2>
constexpr auto operator/(Unit<M1, L1, T1> a, Unit<M2, L2, T2> b) {
    return Unit<M1-M2, L1-L2, T1-T2>(a.value / b.value);
}

void demo() {
    std::cout << "\n=== Practical Examples ===\n";
    
    // String hash
    process_message("hello");
    process_message("unknown");
    
    // Type name
    std::cout << "Type of int: " << type_name<int>() << '\n';
    
    // Unit system
    constexpr Length distance(100.0);  // 100 метров
    constexpr Time time(10.0);         // 10 секунд
    constexpr auto velocity = distance / time;  // Velocity
    
    std::cout << "Velocity: " << velocity.value << " m/s\n";
}

} // namespace practical_examples

// ============================================
// 📌 BEST PRACTICES
// ============================================

/*
 * РЕКОМЕНДАЦИИ ПО МЕТАПРОГРАММИРОВАНИЮ:
 * 
 * 1. ИСПОЛЬЗУЙТЕ CONCEPTS ВМЕСТО SFINAE
 *    ✅ template<std::integral T>
 *    ❌ template<typename T, std::enable_if_t<...>>
 * 
 * 2. constexpr ДЛЯ COMPILE-TIME ВЫЧИСЛЕНИЙ
 *    - Используйте constexpr везде где возможно
 *    - consteval для гарантированно compile-time функций
 * 
 * 3. ИЗБЕГАЙТЕ СЛОЖНЫХ SFINAE ТРЮКОВ
 *    - Concepts делают код читаемее
 *    - SFINAE только для legacy кода
 * 
 * 4. TYPE TRAITS ИЗ STD
 *    - Используйте стандартные traits где возможно
 *    - Кастомные только при необходимости
 * 
 * 5. CRTP ДЛЯ STATIC POLYMORPHISM
 *    - Производительнее виртуальных функций
 *    - Но сложнее для понимания
 * 
 * 6. TEMPLATE SPECIALIZATION
 *    - Используйте умеренно
 *    - Может усложнить понимание кода
 * 
 * 7. COMPILE-TIME СООБЩЕНИЯ ОБ ОШИБКАХ
 *    - static_assert с понятными сообщениями
 *    - Concepts дают лучшие ошибки чем SFINAE
 */

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== Modern C++ Template Metaprogramming ===\n\n";
    
    template_basics::demo();
    concepts_demo::demo();
    type_traits_demo::demo();
    sfinae_vs_concepts::demo();
    compile_time::demo();
    advanced_patterns::demo();
    template_specialization::demo();
    metafunctions::demo();
    practical_examples::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ Templates - function, class, variable, variadic\n";
    std::cout << "✓ Concepts (C++20) - type constraints with readable syntax\n";
    std::cout << "✓ Type traits - compile-time type introspection\n";
    std::cout << "✓ SFINAE vs Concepts - prefer concepts in modern code\n";
    std::cout << "✓ constexpr/consteval - compile-time computation\n";
    std::cout << "✓ CRTP - static polymorphism pattern\n";
    std::cout << "✓ Policy-based design - compile-time configuration\n";
    std::cout << "✓ Type erasure - runtime polymorphism without inheritance\n";
    std::cout << "✓ Metafunctions - type-level programming\n";
    
    return 0;
}
