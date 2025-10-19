# Продвинутые структуры данных

Структуры, которые чаще всего всплывают на продвинутых собеседованиях и в задачах высокого уровня.

## Сбалансированные деревья поиска

- Используйте, когда нужен упорядоченный набор с логарифмическими вставками и удалениями.
- В стандартной библиотеке C++ доступны `std::set`, `std::map`, `std::multiset`, `std::multimap` (красно-чёрные деревья).
- Для операций `order_of_key` и `find_by_order` применяйте расширение GNU PBDS.

```cpp
#include <functional>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

template <class T>
using ordered_set = __gnu_pbds::tree<T,
                                     __gnu_pbds::null_type,
                                     std::less<T>,
                                     __gnu_pbds::rb_tree_tag,
                                     __gnu_pbds::tree_order_statistics_node_update>;
```

## Trie (префиксное дерево)

- Эффективно для префиксных запросов, автодополнения и поиска по словарю.
- Храните счётчик узлов, чтобы быстро отвечать на количество слов с данным префиксом.

```cpp
#include <array>
#include <string>

struct TrieNode {
    std::array<int, 26> next{};
    bool is_word = false;
    int prefix_count = 0;

    TrieNode() {
        next.fill(-1);
    }
};

struct Trie {
    std::vector<TrieNode> nodes = {TrieNode()};

    void insert(const std::string& word) {
        int cur = 0;
        for (char ch : word) {
            int idx = ch - 'a';
            if (nodes[cur].next[idx] == -1) {
                nodes[cur].next[idx] = static_cast<int>(nodes.size());
                nodes.emplace_back();
            }
            cur = nodes[cur].next[idx];
            ++nodes[cur].prefix_count;
        }
        nodes[cur].is_word = true;
    }

    int count_prefix(const std::string& prefix) const {
        int cur = 0;
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (nodes[cur].next[idx] == -1) {
                return 0;
            }
            cur = nodes[cur].next[idx];
        }
        return nodes[cur].prefix_count;
    }
};
```

## DSU (Disjoint Set Union)

- Используется для динамического объединения множеств и проверки принадлежности.
- Комбинация сжатия путей и объединения по рангу даёт почти константное время.

```cpp
#include <numeric>
#include <vector>

struct DSU {
    std::vector<int> parent;
    std::vector<int> rank;

    explicit DSU(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) {
            return false;
        }
        if (rank[ra] < rank[rb]) {
            std::swap(ra, rb);
        }
        parent[rb] = ra;
        if (rank[ra] == rank[rb]) {
            ++rank[ra];
        }
        return true;
    }
};
```

## Fenwick (Binary Indexed Tree)

- Поддерживает префиксные суммы и точечные обновления за `O(log n)`.
- Легко расширяется до двухмерного варианта.

```cpp
#include <vector>

struct Fenwick {
    int n;
    std::vector<long long> bit;

    explicit Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void update(int idx, long long delta) {
        for (; idx <= n; idx += idx & -idx) {
            bit[idx] += delta;
        }
    }

    long long query(int idx) const {
        long long sum = 0;
        for (; idx > 0; idx -= idx & -idx) {
            sum += bit[idx];
        }
        return sum;
    }

    long long range_query(int l, int r) const {
        return query(r) - query(l - 1);
    }
};
```

## Segment Tree

- Подходит для диапазонных запросов и обновлений; легко адаптируется под минимум, максимум, gcd и т.п.
- Для `lazy propagation` храните дополнительное дерево с отложенными модификациями.

```cpp
#include <vector>

struct SegmentTree {
    int n;
    std::vector<long long> tree;

    explicit SegmentTree(const std::vector<long long>& data) {
        n = 1;
        while (n < static_cast<int>(data.size())) {
            n <<= 1;
        }
        tree.assign(2 * n, 0);
        for (int i = 0; i < static_cast<int>(data.size()); ++i) {
            tree[n + i] = data[i];
        }
        for (int i = n - 1; i > 0; --i) {
            tree[i] = tree[2 * i] + tree[2 * i + 1];
        }
    }

    void update(int idx, long long value) {
        int pos = n + idx;
        tree[pos] = value;
        for (pos >>= 1; pos > 0; pos >>= 1) {
            tree[pos] = tree[2 * pos] + tree[2 * pos + 1];
        }
    }

    long long query(int left, int right) const {
        long long res = 0;
        int l = n + left;
        int r = n + right;
        while (l <= r) {
            if (l % 2 == 1) {
                res += tree[l++];
            }
            if (r % 2 == 0) {
                res += tree[r--];
            }
            l >>= 1;
            r >>= 1;
        }
        return res;
    }
};
```

## Паттерны кеширования

- LRU можно реализовать через двусвязный список и `std::unordered_map`.
- LFU требует дополнительной структуры (map частот + списки), но базовая идея аналогична.

```cpp
#include <list>
#include <unordered_map>

class LRUCache {
public:
    explicit LRUCache(int capacity) : capacity_(capacity) {}

    int get(int key) {
        auto it = map_.find(key);
        if (it == map_.end()) {
            return -1;
        }
        touch(it);
        return it->second.first;
    }

    void put(int key, int value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second.first = value;
            touch(it);
            return;
        }
        if (static_cast<int>(order_.size()) == capacity_) {
            int lru_key = order_.back();
            order_.pop_back();
            map_.erase(lru_key);
        }
        order_.push_front(key);
        map_[key] = {value, order_.begin()};
    }

private:
    int capacity_;
    std::list<int> order_;
    std::unordered_map<int, std::pair<int, std::list<int>::iterator>> map_;

    void touch(std::unordered_map<int, std::pair<int, std::list<int>::iterator>>::iterator it) {
        order_.erase(it->second.second);
        order_.push_front(it->first);
        it->second.second = order_.begin();
    }
};
```
