# Динамическое программирование

Методика разбиения задачи на перекрывающиеся подзадачи с запоминанием промежуточных результатов.

## Классификация задач

- Линейное DP: одна размерность, переход из предыдущих состояний.
- DP на масках: работа с подмножествами, где состояние кодируется битами.
- DP на графах: вычисления в топологическом порядке или по глубине.

```cpp
int climb_stairs(int n) {
    if (n <= 2) {
        return n;
    }
    int prev1 = 1;
    int prev2 = 2;
    for (int i = 3; i <= n; ++i) {
        int cur = prev1 + prev2;
        prev1 = prev2;
        prev2 = cur;
    }
    return prev2;
}
```

## DP на строках

- Используйте двумерные таблицы или оптимизируйте до одной строки.
- Типичные задачи: LCS, расстояние Левенштейна, палиндромные подстроки.

```cpp
#include <algorithm>
#include <string>
#include <vector>

int lcs(const std::string& a, const std::string& b) {
    int n = static_cast<int>(a.size());
    int m = static_cast<int>(b.size());
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n][m];
}
```

## DP на интервалах

- Перебирайте длину интервала и левые границы.
- Для задач вроде «burst balloons» перебирайте последнюю операцию внутри интервала.

```cpp
#include <algorithm>
#include <vector>

int burst_balloons(std::vector<int> nums) {
    nums.insert(nums.begin(), 1);
    nums.push_back(1);
    int n = static_cast<int>(nums.size());
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
    for (int len = 2; len < n; ++len) {
        for (int left = 0; left + len < n; ++left) {
            int right = left + len;
            for (int k = left + 1; k < right; ++k) {
                dp[left][right] = std::max(
                    dp[left][right],
                    nums[left] * nums[k] * nums[right] + dp[left][k] + dp[k][right]
                );
            }
        }
    }
    return dp[0][n - 1];
}
```

## DP на масках

- Состояние включает позицию и множество уже посещённых элементов.
- Используется в TSP, назначениях, задачах по перебору подмножеств.

```cpp
#include <algorithm>
#include <limits>
#include <vector>

int tsp(const std::vector<std::vector<int>>& dist) {
    int n = static_cast<int>(dist.size());
    const int INF = std::numeric_limits<int>::max() / 4;
    std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, INF));
    dp[1][0] = 0;
    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1 << u)) || dp[mask][u] == INF) {
                continue;
            }
            for (int v = 0; v < n; ++v) {
                if (mask & (1 << v)) {
                    continue;
                }
                int next_mask = mask | (1 << v);
                dp[next_mask][v] = std::min(dp[next_mask][v], dp[mask][u] + dist[u][v]);
            }
        }
    }
    int answer = INF;
    for (int v = 1; v < n; ++v) {
        answer = std::min(answer, dp[(1 << n) - 1][v] + dist[v][0]);
    }
    return answer;
}
```

## Оптимизации переходов

- Knuth optimization: ограничивает диапазон поиска оптимального `k` в DP на интервалах.
- Convex Hull Trick: ускоряет переходы вида `dp[i] = min(dp[j] + m_j * x_i + b_j)`.
- Divide and Conquer optimization: работает, когда оптимальное `k` монотонно растёт вместе с индексами.
