# Алгоритмические шаблоны

Чек-лист часто используемых техник и приёмов на собеседованиях.

## Два указателя и скользящее окно

- Подходит для массивов и строк, где важно поддерживать свойства текущего подотрезка.
- Для подсчёта различных символов используйте `std::unordered_map<char, int>` или фиксированный массив.

```cpp
#include <string>
#include <unordered_map>

int longest_substring_k_distinct(const std::string& s, int k) {
    std::unordered_map<char, int> freq;
    int left = 0;
    int best = 0;
    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        ++freq[s[right]];
        while (static_cast<int>(freq.size()) > k) {
            char ch = s[left++];
            if (--freq[ch] == 0) {
                freq.erase(ch);
            }
        }
        best = std::max(best, right - left + 1);
    }
    return best;
}
```

## Префиксные суммы и разности

- Постройте массив префиксов, чтобы отвечать на запрос `sum(l, r)` за `O(1)`.
- Разностный массив полезен для массовых обновлений диапазонов.

```cpp
#include <algorithm>
#include <numeric>
#include <vector>

long long range_sum(const std::vector<long long>& prefix, int left, int right) {
    return prefix[right + 1] - prefix[left];
}
```

## Бинарный поиск по ответу

- Ищите минимальное значение, удовлетворяющее условию; функция-предикат должна быть монотонной.
- Шаблон полезен для задач на распределение нагрузки, выделение ресурсов, оптимизацию времени.

```cpp
#include <vector>

int min_capacity(const std::vector<int>& weights, int days) {
    int lo = *std::max_element(weights.begin(), weights.end());
    int hi = std::accumulate(weights.begin(), weights.end(), 0);
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        int need = 1;
        int current = 0;
        for (int w : weights) {
            if (current + w > mid) {
                ++need;
                current = 0;
            }
            current += w;
        }
        if (need <= days) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}
```

## Meet-in-the-middle

- Делит задачу на две половины, сокращая сложность с `O(2^n)` до `O(2^{n/2})`.
- Подходит для подсчёта сумм подмножеств, задач на XOR и количества вариантов.

```cpp
#include <algorithm>
#include <vector>

bool subset_sum(const std::vector<int>& nums, int target) {
    int n = static_cast<int>(nums.size());
    int mid = n / 2;
    std::vector<int> left(nums.begin(), nums.begin() + mid);
    std::vector<int> right(nums.begin() + mid, nums.end());

    std::vector<int> sums_left;
    for (int mask = 0; mask < (1 << static_cast<int>(left.size())); ++mask) {
        int sum = 0;
        for (int i = 0; i < static_cast<int>(left.size()); ++i) {
            if (mask & (1 << i)) {
                sum += left[i];
            }
        }
        sums_left.push_back(sum);
    }

    std::vector<int> sums_right;
    for (int mask = 0; mask < (1 << static_cast<int>(right.size())); ++mask) {
        int sum = 0;
        for (int i = 0; i < static_cast<int>(right.size()); ++i) {
            if (mask & (1 << i)) {
                sum += right[i];
            }
        }
        sums_right.push_back(sum);
    }
    std::sort(sums_right.begin(), sums_right.end());

    for (int s : sums_left) {
        int need = target - s;
        auto it = std::lower_bound(sums_right.begin(), sums_right.end(), need);
        if (it != sums_right.end() && *it == need) {
            return true;
        }
    }
    return false;
}
```

## Sweep line

- Обрабатывайте события в отсортированном порядке, поддерживая активное множество.
- Применяется для интервалов, геометрии, анализа временных отрезков.

```cpp
#include <vector>
#include <algorithm>

int max_overlap(const std::vector<std::pair<int, int>>& intervals) {
    std::vector<std::pair<int, int>> events;
    events.reserve(intervals.size() * 2);
    for (auto [start, end] : intervals) {
        events.emplace_back(start, 1);
        events.emplace_back(end, -1);
    }
    std::sort(events.begin(), events.end());
    int cur = 0;
    int best = 0;
    for (auto [_, delta] : events) {
        cur += delta;
        best = std::max(best, cur);
    }
    return best;
}
```

## Backtracking и битовые маски

- Backtracking генерирует решения по шагам, откатываясь при конфликте.
- DP по битовым маскам эффективно для `n ≤ 20`, где состояние кодируется набором использованных элементов.

```cpp
#include <vector>

long long count_hamiltonian_paths(const std::vector<std::vector<int>>& graph) {
    int n = static_cast<int>(graph.size());
    std::vector<std::vector<long long>> dp(1 << n, std::vector<long long>(n, 0));
    dp[1][0] = 1;
    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1 << u)) || dp[mask][u] == 0) {
                continue;
            }
            for (int v : graph[u]) {
                if (mask & (1 << v)) {
                    continue;
                }
                dp[mask | (1 << v)][v] += dp[mask][u];
            }
        }
    }
    long long total = 0;
    for (int v = 0; v < n; ++v) {
        total += dp[(1 << n) - 1][v];
    }
    return total;
}
```
