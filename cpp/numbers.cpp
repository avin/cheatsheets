#include <vector>
#include <cmath>
#include <string>
#include <sstream>

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
// 📌 Генерация случайного числа (std::rand, std::srand)
// ---------------------------------------------------
#include <cstdlib>
#include <ctime>
int example_rand_seeded() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    return std::rand(); // в диапазоне 0..RAND_MAX
}