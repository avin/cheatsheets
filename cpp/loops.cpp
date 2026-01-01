#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <print>  // C++23

// ---------------------------------------------------
// 📌 Циклы, условия, тернарный оператор
// ---------------------------------------------------
void example_loops() {
    // Цикл for
    for (int i = 0; i < 10; i++) {
        std::println("{}", i);
    }

    // Диапазонный цикл for (range-based for)
    std::vector<int> arr = {1, 2, 3};
    for (int x : arr) {
        std::println("{}", x);
    }

    // Цикл while
    int i = 0;
    while (i <= 10) {
        i++;
    }

    // do...while
    do {
        i--;
    } while (i > 0);

    // forEach с индексом (эмуляция)
    for (size_t idx = 0; idx < arr.size(); ++idx) {
        std::println("index: {}, value: {}", idx, arr[idx]);
    }

    // Условия if / else
    bool b = true;
    if (i == 0) {
        std::println("0");
    } else {
        std::println("non 0");
    }

    // switch-case
    switch (i) {
        case 0:
            std::println("=0");
            break;
        case 1:
            std::println("=1");
            break;
        default:
            std::println("other");
            break;
    }

    // break / continue
    for (int j = 0; j < 10; j++) {
        if (j % 2 == 0) continue;
        if (j > 8) break;
        std::println("{}", j);
    }

    // Множественные условия
    if (i && b) std::println("both");
    if (i || b) std::println("one of");
    else std::println("no one");

    // Тернарный оператор
    int bar = 2;
    std::string foo = (bar == 2) ? "the 2" : "other";
}

// ---------------------------------------------------
// 📌 Перебор с шагом (for с шагом, пример применения std::iota и std::for_each)
// ---------------------------------------------------
void example_for_step() {
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 0); // заполняем 0..9
    for (int x : v) {
        if (x % 2 == 0) {
            std::println("Чётное: {}", x);
        }
    }
}

// ---------------------------------------------------
// 📌 if consteval (C++23) - проверка compile-time контекста
// ---------------------------------------------------
consteval int compile_time_only(int x) {
    return x * x;
}

constexpr int flexible_function(int x) {
    // if consteval позволяет выполнить разный код в зависимости от контекста
    if consteval {
        // Этот код выполняется только во время компиляции
        return x * x;
    } else {
        // Этот код выполняется во время выполнения
        std::println("Runtime computation");
        return x * x;
    }
}

// Более сложный пример с if consteval
constexpr int smart_computation(int x) {
    if consteval {
        // Во время компиляции используем более дорогой, но точный алгоритм
        int result = 0;
        for (int i = 0; i < x; ++i) {
            result += i * i;
        }
        return result;
    } else {
        // Во время выполнения используем быструю аппроксимацию
        return (x * (x - 1) * (2 * x - 1)) / 6;
    }
}

// if consteval vs if constexpr:
// - if constexpr: проверяет условие времени компиляции
// - if consteval: проверяет, выполняется ли код во время компиляции

void example_if_consteval() {
    // Вызов во время компиляции
    constexpr int result1 = flexible_function(5);
    
    // Вызов во время выполнения
    int runtime_val = 10;
    int result2 = flexible_function(runtime_val);
    
    // if consteval позволяет оптимизировать код для разных контекстов
    constexpr int comp_result = smart_computation(10);
}

// ---------------------------------------------------
// 📌 if constexpr vs if consteval (сравнение)
// ---------------------------------------------------
template<typename T>
constexpr void process(T value) {
    // if constexpr - проверяет тип во время компиляции
    if constexpr (std::is_integral_v<T>) {
        std::println("Processing integer");
    } else {
        std::println("Processing non-integer");
    }
    
    // if consteval - проверяет контекст вызова
    if consteval {
        // Compile-time код
    } else {
        // Runtime код
    }
}
