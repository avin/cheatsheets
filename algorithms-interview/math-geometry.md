# Численные методы и геометрия

Основные формулы и алгоритмы, которые часто всплывают в задачах на собеседованиях.

## Простые числа и решето

- Решето Эратосфена работает за `O(n log log n)`.
- Для экономии памяти используйте `std::vector<bool>` или битсет.

```cpp
#include <vector>

std::vector<bool> sieve(int n) {
    std::vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (int multiple = p * p; multiple <= n; multiple += p) {
                is_prime[multiple] = false;
            }
        }
    }
    return is_prime;
}
```

## Быстрое возведение в степень и обратный элемент

- Возведение по модулю за `O(log n)` через двоичное возведение.
- Обратный элемент по модулю простого числа: `a^{mod-2} mod mod`.

```cpp
long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1LL) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1LL;
    }
    return result;
}

long long mod_inv(long long a, long long mod) {
    return mod_pow(a, mod - 2, mod);
}
```

## Векторная геометрия

- Точка представляется структурой с координатами double.
- Скаляное и векторное произведения помогают определять ориентацию и проекции.

```cpp
#include <cmath>

struct Point {
    double x;
    double y;
};

double dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

double cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

double length(const Point& v) {
    return std::hypot(v.x, v.y);
}
```

## Проверка пересечения отрезков

- Используйте ориентации тройки точек и проверку пересечения проекций.

```cpp
#include <algorithm>
#include <cmath>

int orientation(const Point& a, const Point& b, const Point& c) {
    double val = cross({b.x - a.x, b.y - a.y}, {c.x - a.x, c.y - a.y});
    if (std::abs(val) < 1e-9) return 0;
    return val > 0 ? 1 : -1;
}

bool on_segment(const Point& a, const Point& b, const Point& p) {
    return std::min(a.x, b.x) - 1e-9 <= p.x && p.x <= std::max(a.x, b.x) + 1e-9 &&
           std::min(a.y, b.y) - 1e-9 <= p.y && p.y <= std::max(a.y, b.y) + 1e-9;
}

bool segments_intersect(const Point& a, const Point& b, const Point& c, const Point& d) {
    int o1 = orientation(a, b, c);
    int o2 = orientation(a, b, d);
    int o3 = orientation(c, d, a);
    int o4 = orientation(c, d, b);
    if (o1 != o2 && o3 != o4) {
        return true;
    }
    if (o1 == 0 && on_segment(a, b, c)) return true;
    if (o2 == 0 && on_segment(a, b, d)) return true;
    if (o3 == 0 && on_segment(c, d, a)) return true;
    if (o4 == 0 && on_segment(c, d, b)) return true;
    return false;
}
```

## Площадь многоугольника

- Формула шнуровки: суммируйте `x_i * y_{i+1} - x_{i+1} * y_i`.

```cpp
#include <vector>

double polygon_area(const std::vector<Point>& pts) {
    double area = 0.0;
    int n = static_cast<int>(pts.size());
    for (int i = 0; i < n; ++i) {
        const Point& a = pts[i];
        const Point& b = pts[(i + 1) % n];
        area += a.x * b.y - b.x * a.y;
    }
    return std::abs(area) * 0.5;
}
```

## Полярная сортировка

- Сортируйте по квадрату длины и знаку векторного произведения.
- В C++ удобно реализовать компаратор, который сравнивает квадранты и кросс-продукт.
