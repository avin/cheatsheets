# Графы

Набор ключевых алгоритмов на графах и их реализации на C++.

## Обходы и топологическая сортировка

- DFS подходит для поиска компонент, проверки на циклы, вычисления времен входа/выхода.
- BFS даёт кратчайшие пути в невзвешенных графах.
- Топологическая сортировка применима только к DAG, удобно использовать алгоритм Кана.

```cpp
#include <queue>
#include <stdexcept>
#include <vector>

std::vector<int> topological_sort(const std::vector<std::vector<int>>& graph) {
    int n = static_cast<int>(graph.size());
    std::vector<int> indeg(n, 0);
    for (const auto& edges : graph) {
        for (int v : edges) {
            ++indeg[v];
        }
    }
    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (indeg[i] == 0) {
            q.push(i);
        }
    }
    std::vector<int> order;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);
        for (int nxt : graph[v]) {
            if (--indeg[nxt] == 0) {
                q.push(nxt);
            }
        }
    }
    if (static_cast<int>(order.size()) != n) {
        throw std::runtime_error("Cycle detected");
    }
    return order;
}
```

## Кратчайшие пути

- Dijkstra: без отрицательных весов, `O((V + E) log V)` с приоритетной очередью.
- Bellman-Ford: позволяет обрабатывать отрицательные веса и детектировать циклы.
- Floyd-Warshall: матрица всех кратчайших путей за `O(n^3)`.

```cpp
#include <queue>
#include <vector>
#include <utility>
#include <limits>

std::vector<long long> dijkstra(const std::vector<std::vector<std::pair<int, int>>>& graph, int start) {
    const long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<long long> dist(graph.size(), INF);
    using Node = std::pair<long long, int>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    dist[start] = 0;
    pq.emplace(0, start);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();
        if (d != dist[v]) {
            continue;
        }
        for (auto [to, w] : graph[v]) {
            long long nd = d + w;
            if (nd < dist[to]) {
                dist[to] = nd;
                pq.emplace(nd, to);
            }
        }
    }
    return dist;
}
```

## Минимальные остовные деревья

- Kruskal: сортировка рёбер + DSU.
- Prim: строит остов из выбранной вершины, используя приоритетную очередь.

```cpp
#include <algorithm>
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

long long kruskal(int n, std::vector<std::tuple<int, int, int>> edges) {
    std::sort(edges.begin(), edges.end());
    DSU dsu(n);
    long long total = 0;
    for (auto [w, u, v] : edges) {
        if (dsu.unite(u, v)) {
            total += w;
        }
    }
    return total;
}
```

## Потоки и разрезы

- Edmonds-Karp использует BFS по остаточному графу.
- Dinic строит слоистый граф и выполняет DFS по уровневым путям.
- Частые сценарии: распределение задач, матчинг в двудольном графе, сетевые маршруты.

## Компоненты и циклы

- Strongly Connected Components считаются алгоритмами Kosaraju или Tarjan.
- Эйлеровы маршруты существуют, если число вершин с нечётной степенью равно 0 (цикл) или 2 (путь).
- Гамильтоновы пути NP-полны; для малых `n` применяется DP по маскам.

```cpp
#include <functional>
#include <vector>

std::vector<std::vector<int>> kosaraju(const std::vector<std::vector<int>>& graph) {
    int n = static_cast<int>(graph.size());
    std::vector<bool> used(n, false);
    std::vector<int> order;

    std::function<void(int)> dfs = [&](int v) {
        used[v] = true;
        for (int to : graph[v]) {
            if (!used[to]) {
                dfs(to);
            }
        }
        order.push_back(v);
    };

    for (int v = 0; v < n; ++v) {
        if (!used[v]) {
            dfs(v);
        }
    }

    std::vector<std::vector<int>> rev(n);
    for (int v = 0; v < n; ++v) {
        for (int to : graph[v]) {
            rev[to].push_back(v);
        }
    }

    std::vector<std::vector<int>> components;
    used.assign(n, false);
    std::function<void(int, std::vector<int>&)> dfs_rev = [&](int v, std::vector<int>& comp) {
        used[v] = true;
        comp.push_back(v);
        for (int to : rev[v]) {
            if (!used[to]) {
                dfs_rev(to, comp);
            }
        }
    };

    for (int i = n - 1; i >= 0; --i) {
        int v = order[i];
        if (!used[v]) {
            std::vector<int> comp;
            dfs_rev(v, comp);
            components.push_back(std::move(comp));
        }
    }
    return components;
}
```
