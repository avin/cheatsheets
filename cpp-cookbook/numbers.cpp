#include <vector>
#include <cmath>
#include <string>
#include <string_view>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <print>  // C++23
#include <charconv>
#include <optional>

// ---------------------------------------------------
// 📌 Округление
// ---------------------------------------------------
void example_rounding(double x) {
    double rd = std::round(x);    // до ближайшего
    double up = std::ceil(x);     // вверх
    double down = std::floor(x);  // вниз
}

// ---------------------------------------------------
// 📌 Целочисленное деление и остаток
// ---------------------------------------------------
void example_div_mod(int x, int y) {
    int div = x / y;   // целочисленное деление
    int rem = x % y;   // остаток от деления
}

// ---------------------------------------------------
// 📌 Знаки и модуль
// ---------------------------------------------------
void example_sign_abs(double x) {
    double negative = -std::abs(x);
    double absolute = std::abs(x);
    int sign = (x > 0) - (x < 0); // 1, 0 или -1
}

// ---------------------------------------------------
// 📌 Диапазон чисел
// ---------------------------------------------------
std::vector<int> example_range(int start, int n) {
    std::vector<int> range;
    range.reserve(n);
    for (int i = 0; i < n; ++i) {
        range.push_back(start + i);
    }
    return range;
}

// ---------------------------------------------------
// 📌 Преобразование типов
// ---------------------------------------------------
void example_conversion(const std::string& str) {
    int num = std::stoi(str);         // преобразование строки в число
    std::string s2 = std::to_string(num); // преобразование числа в строку
    // использование ostringstream
    std::ostringstream oss;
    oss << "Число: " << num;
    std::string formatted = oss.str();
}

// ---------------------------------------------------
// 📌 Парсинг числа из std::string_view (std::from_chars)
// ---------------------------------------------------
// Плюсы:
// - без аллокаций, работает напрямую по буферу
// - быстрый и детерминированный, без локали
// - без исключений, явная проверка ошибок
std::optional<int> example_from_chars(std::string_view sv) {
    int timestamp = 0;
    auto* first = sv.data();
    auto* last = sv.data() + sv.size();

    if (auto [ptr, ec] = std::from_chars(first, last, timestamp);
        ec != std::errc{} || ptr != last) {
        return std::nullopt; // ошибка парсинга
    }

    return timestamp;
}

// ---------------------------------------------------
// 📌 Генерация случайного числа (std::rand, std::srand)
// ---------------------------------------------------
#include <cstdlib>
#include <ctime>
int example_rand_seeded() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    return std::rand(); // в диапазоне 0..RAND_MAX
}

// ---------------------------------------------------
// 📌 Литералы для size_t (C++23) - суффикс uz/zu
// ---------------------------------------------------
void example_size_t_literals() {
    // До C++23
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // Старый способ - требует явного приведения или может быть warning
    for (size_t i = 0; i < vec.size(); ++i) {
        std::println("{}", vec[i]);
    }
    
    // C++23: литералы size_t с суффиксом uz
    auto size = 42uz;          // size_t
    auto zero = 0uz;           // size_t(0)
    auto large = 1000000uz;    // size_t
    
    // Использование в сравнениях (избегает signed/unsigned warnings)
    if (vec.size() > 3uz) {
        std::println("Vector has more than 3 elements");
    }
    
    // В циклах
    for (auto i = 0uz; i < vec.size(); ++i) {
        std::println("Element {}: {}", i, vec[i]);
    }
    
    // С массивами
    int arr[10uz];  // массив из 10 элементов
    
    // Альтернативный суффикс zu (эквивалентен uz)
    auto size_zu = 42zu;
}

// ---------------------------------------------------
// 📌 Другие числовые литералы (для справки)
// ---------------------------------------------------
void example_other_literals() {
    // Целочисленные литералы
    auto a = 42;        // int
    auto b = 42u;       // unsigned int
    auto c = 42l;       // long
    auto d = 42ul;      // unsigned long
    auto e = 42ll;      // long long
    auto f = 42ull;     // unsigned long long
    auto g = 42uz;      // size_t (C++23)
    
    // Литералы с плавающей точкой
    auto x = 3.14;      // double
    auto y = 3.14f;     // float
    auto z = 3.14l;     // long double
    
    // Бинарные литералы (C++14)
    auto bin = 0b1010;  // 10 в десятичной
    
    // Разделители цифр (C++14)
    auto million = 1'000'000;
    auto binary = 0b1111'0000'1010;
    auto hex = 0xDEAD'BEEF;
}
