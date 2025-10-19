# Строковые алгоритмы

Подборка основных приёмов обработки строк на C++.

## Префикс-функция и KMP

- Префикс-функция `pi[i]` — длина наибольшего собственного префикса, совпадающего с суффиксом.
- Алгоритм Кнута—Морриса—Пратта использует `pi` для линейного поиска подстроки.

```cpp
#include <string>
#include <utility>
#include <vector>

std::vector<int> prefix_function(const std::string& s) {
    std::vector<int> pi(s.size(), 0);
    for (size_t i = 1; i < s.size(); ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            ++j;
        }
        pi[i] = j;
    }
    return pi;
}
```

## Z-функция

- `z[i]` — длина наибольшего подстрочного совпадения с началом строки.
- Полезна для поиска образца, проверки палиндромов, задач на бордеры.

```cpp
#include <string>
#include <vector>

std::vector<int> z_function(const std::string& s) {
    std::vector<int> z(s.size(), 0);
    int l = 0;
    int r = 0;
    for (size_t i = 1; i < s.size(); ++i) {
        if (static_cast<int>(i) <= r) {
            z[i] = std::min(r - static_cast<int>(i) + 1, z[i - l]);
        }
        while (i + z[i] < s.size() && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (static_cast<int>(i) + z[i] - 1 > r) {
            l = static_cast<int>(i);
            r = static_cast<int>(i) + z[i] - 1;
        }
    }
    z[0] = static_cast<int>(s.size());
    return z;
}
```

## Rolling hash

- Позволяет вычислять хеш подстроки за `O(1)` после подготовки префиксов.
- Используйте пару модулей/оснований для снижения вероятности коллизий.

```cpp
#include <string>
#include <vector>

struct RollingHash {
    static constexpr long long MOD = 1'000'000'007LL;
    static constexpr long long BASE = 911382323LL;
    std::vector<long long> h;
    std::vector<long long> p;

    explicit RollingHash(const std::string& s) : h(s.size() + 1, 0), p(s.size() + 1, 1) {
        for (size_t i = 0; i < s.size(); ++i) {
            h[i + 1] = (h[i] * BASE + s[i]) % MOD;
            p[i + 1] = (p[i] * BASE) % MOD;
        }
    }

    long long get(int l, int r) const { // [l, r)
        return (h[r] - h[l] * p[r - l] % MOD + MOD) % MOD;
    }
};
```

## Суффиксный массив и LCP

- Суффиксный массив сортирует все суффиксы строки, LCP ускоряет подсчёт общих префиксов.
- Классический подход — итеративное удвоение длины сравниваемых фрагментов.

```cpp
#include <algorithm>
#include <string>
#include <vector>

std::vector<int> suffix_array(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<int> sa(n), rank(n), temp(n);
    for (int i = 0; i < n; ++i) {
        sa[i] = i;
        rank[i] = s[i];
    }
    for (int k = 1;; k <<= 1) {
        auto key = [&](int i) {
            return std::pair<int, int>{rank[i], i + k < n ? rank[i + k] : -1};
        };
        std::sort(sa.begin(), sa.end(), [&](int a, int b) { return key(a) < key(b); });
        temp[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            temp[sa[i]] = temp[sa[i - 1]] + (key(sa[i - 1]) != key(sa[i]));
        }
        rank = temp;
        if (rank[sa.back()] == n - 1) {
            break;
        }
    }
    return sa;
}
```

## Парсинг и разбор

- Для арифметических выражений удобно использовать обратную польскую запись или алгоритм «сортировочной станции».
- На собеседованиях часто требуют реализацию калькулятора в RPN.

```cpp
#include <stack>
#include <string>
#include <vector>

int eval_rpn(const std::vector<std::string>& tokens) {
    std::stack<int> st;
    for (const std::string& token : tokens) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();
            if (token == "+") st.push(a + b);
            else if (token == "-") st.push(a - b);
            else if (token == "*") st.push(a * b);
            else st.push(a / b);
        } else {
            st.push(std::stoi(token));
        }
    }
    return st.top();
}
```
