# Базовые структуры данных

Повторение стандартных контейнеров в C++ и типовых сценариев их использования.

## Динамические массивы и списки

- `std::vector` обеспечивает амортизированное `O(1)` для `push_back` и произвольный доступ.
- `std::deque` хранит данные блоками, даёт быстрый доступ к обоим концам.
- `std::list` полезен для частых вставок в середину, но лишён случайного доступа.

```cpp
#include <deque>
#include <vector>

std::deque<int> window;
for (int x : stream) {
    window.push_back(x);
    if (static_cast<int>(window.size()) > k) {
        window.pop_front();
    }
}
```

## Стек и очередь

- `std::stack` строится поверх любого контейнера (по умолчанию `deque`), используйте для backtracking.
- `std::queue` — FIFO, подходит для BFS и планирования задач.
- Для двусторонней структуры применяйте `std::deque`.

```cpp
#include <queue>
#include <unordered_map>
#include <vector>

std::unordered_map<int, int> bfs(const std::vector<std::vector<int>>& graph, int start) {
    std::queue<int> q;
    std::unordered_map<int, int> dist;
    q.push(start);
    dist[start] = 0;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int nxt : graph[node]) {
            if (!dist.count(nxt)) {
                dist[nxt] = dist[node] + 1;
                q.push(nxt);
            }
        }
    }
    return dist;
}
```

## Deque и скользящие окна

- Для задач на максимум/минимум в окне поддерживайте монотонный `std::deque<int>` с индексами.
- Следите, чтобы индексы вне текущего окна удалялись из головы.

```cpp
#include <deque>
#include <vector>

std::vector<int> sliding_max(const std::vector<int>& nums, int k) {
    std::deque<int> dq;
    std::vector<int> ans;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        dq.push_back(i);
        if (dq.front() <= i - k) {
            dq.pop_front();
        }
        if (i >= k - 1) {
            ans.push_back(nums[dq.front()]);
        }
    }
    return ans;
}
```

## Хэш-таблицы и множества

- `std::unordered_map` и `std::unordered_set` дают амортизированное `O(1)` на операции.
- Для составных ключей реализуйте `struct` с `operator==` и пользовательский `std::hash`.
- Для подсчёта частот достаточно хранить значения и обновлять максимум.

```cpp
#include <unordered_map>
#include <vector>

int majority_element(const std::vector<int>& nums) {
    std::unordered_map<int, int> freq;
    int best = 0;
    int best_count = 0;
    for (int x : nums) {
        int count = ++freq[x];
        if (count > best_count) {
            best = x;
            best_count = count;
        }
    }
    return best;
}
```

## Очередь с приоритетом

- `std::priority_queue` по умолчанию — max-heap; для min-heap используйте `std::greater`.
- Помните про lazy deletion: храните пары `(значение, версия)` и проверяйте актуальность при извлечении.
- Для ограниченного множества минимальных элементов используйте max-heap фиксированного размера.

```cpp
#include <queue>
#include <vector>

std::vector<int> k_smallest(const std::vector<int>& nums, int k) {
    std::priority_queue<int> heap;
    for (int x : nums) {
        if (static_cast<int>(heap.size()) < k) {
            heap.push(x);
        } else if (x < heap.top()) {
            heap.pop();
            heap.push(x);
        }
    }
    std::vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top());
        heap.pop();
    }
    return result; // элементы возвращаются в произвольном порядке
}
```
