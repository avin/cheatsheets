#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <forward_list>
#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <iterator>

// ---------------------------------------------------
// 📌 Очередь (FIFO)
// ---------------------------------------------------
void example_queue() {
    int x = 5;
    std::queue<int> q;
    q.push(x);       // добавление
    q.pop();         // удаление
}

// ---------------------------------------------------
// 📌 Стек (LIFO)
// ---------------------------------------------------
void example_stack() {
    int x = 5;
    std::stack<int> st;
    st.push(x);      // добавление
    st.pop();        // удаление
}

// ---------------------------------------------------
// 📌 Двусторонняя очередь (Deque)
// ---------------------------------------------------
void example_deque() {
    int x = 5, y = 3;
    std::deque<int> dq;
    dq.push_back(x);    // в конец
    dq.push_front(y);   // в начало
    dq.pop_back();      // с конца
    dq.pop_front();     // с начала
}

// ---------------------------------------------------
// 📌 Односвязный список (std::forward_list)
// ---------------------------------------------------
void example_forward_list() {
    std::forward_list<int> fl = {1, 2, 3};
    fl.push_front(0);      // вставить в начало
    fl.pop_front();        // удалить первый элемент
    // обход:
    for (int v : fl) {
        // обработка v
    }
}

// ---------------------------------------------------
// 📌 Двусвязный список (std::list)
// ---------------------------------------------------
void example_list() {
    std::list<int> lst = {1, 2};
    lst.push_back(3);     // в конец
    lst.push_front(0);    // в начало
    lst.pop_back();       // удалить с конца
    lst.pop_front();      // удалить с начала
    // удаление конкретного значения:
    lst.remove(2);
    // обход:
    for (int v : lst) {
        // обработка v
    }
}

// ---------------------------------------------------
// 📌 Множество (std::set, std::unordered_set)
// ---------------------------------------------------
void example_set() {
    std::set<int> ordered = {1, 2};
    ordered.insert(3);
    ordered.erase(2);
    bool hasO = ordered.count(1) > 0;

    std::unordered_set<int> unordered = {1, 2};
    unordered.insert(3);
    unordered.erase(2);
    bool hasU = unordered.find(1) != unordered.end();
}

// ---------------------------------------------------
// 📌 Словарь / хэш-таблица (std::map, std::unordered_map)
// ---------------------------------------------------
void example_map() {
    std::map<std::string, int> ordered_map;
    ordered_map["key"] = 123;
    bool hasKeyO = ordered_map.find("key") != ordered_map.end();
    ordered_map.erase("key");

    std::unordered_map<std::string, int> unordered_map;
    unordered_map["key"] = 456;
    bool hasKeyU = unordered_map.find("key") != unordered_map.end();
    unordered_map.erase("key");
}

// ---------------------------------------------------
// 📌 Куча (heap) с использованием std::make_heap, push_heap, pop_heap
// ---------------------------------------------------
void example_heap_manual() {
    std::vector<int> heap = {3, 1, 4, 1, 5};
    std::make_heap(heap.begin(), heap.end(), std::greater<>()); // min-heap
    heap.push_back(2);
    std::push_heap(heap.begin(), heap.end(), std::greater<>());
    std::pop_heap(heap.begin(), heap.end(), std::greater<>());
    heap.pop_back();
}

// ---------------------------------------------------
// 📌 Куча (приоритетная очередь) (MinHeap / MaxHeap)
// ---------------------------------------------------
void example_priority_queue() {
    std::priority_queue<int> maxHeap;
    maxHeap.push(5);
    int maxVal = maxHeap.top();
    maxHeap.pop();

    std::priority_queue<int, std::vector<int>, std::greater<>> minHeap;
    minHeap.push(5);
    int minVal = minHeap.top();
    minHeap.pop();
}

// ---------------------------------------------------
// 📌 Бинарный поиск (std::binary_search, std::lower_bound)
// ---------------------------------------------------
bool example_binary_search(const std::vector<int>& arr, int target) {
    // проверка наличия
    return std::binary_search(arr.begin(), arr.end(), target);
}
int example_lower_bound(const std::vector<int>& arr, int target) {
    // возвращает индекс, если найден, иначе -1
    auto it = std::lower_bound(arr.begin(), arr.end(), target);
    if (it != arr.end() && *it == target) {
        return static_cast<int>(std::distance(arr.begin(), it));
    }
    return -1;
}

// ---------------------------------------------------
// 📌 Обход графа — DFS (стек) / BFS (очередь) на основе списка смежности
// ---------------------------------------------------
void dfs_adj_list(const std::vector<std::vector<int>>& adj, int start) {
    std::vector<bool> visited(adj.size(), false);
    std::stack<int> st;
    st.push(start);
    while (!st.empty()) {
        int u = st.top();
        st.pop();
        if (visited[u]) continue;
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) st.push(v);
        }
    }
}

void bfs_adj_list(const std::vector<std::vector<int>>& adj, int start) {
    std::vector<bool> visited(adj.size(), false);
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
}

// ---------------------------------------------------
// 📌 Рекурсивный DFS
// ---------------------------------------------------
void dfs_recursive(const std::vector<std::vector<int>>& adj, int u, std::vector<bool>& visited) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) dfs_recursive(adj, v, visited);
    }
}

// ---------------------------------------------------
// 📌 Сортировка с пользовательским компаратором (std::sort)
// ---------------------------------------------------
void example_sort_custom(std::vector<std::pair<int, int>>& vec) {
    // сортировка по второму элементу пары, затем по первому
    std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    });
}

// ---------------------------------------------------
// 📌 Разворот, циклический сдвиг и удаление дубликатов (std::reverse, std::rotate, std::unique)
// ---------------------------------------------------
void example_reverse_rotate_unique(std::vector<int>& arr) {
    // разворот
    std::reverse(arr.begin(), arr.end());

    // циклический сдвиг влево на 2 позиции
    if (arr.size() > 2) {
        std::rotate(arr.begin(), arr.begin() + 2, arr.end());
    }

    // удаление дубликатов (требуется отсортированный массив)
    std::sort(arr.begin(), arr.end());
    auto it = std::unique(arr.begin(), arr.end());
    arr.erase(it, arr.end());
}

// ---------------------------------------------------
// 📌 Топологическая сортировка (DFS на графе)
// ---------------------------------------------------
void topoSort(const std::vector<std::vector<int>>& adj, std::vector<int>& result) {
    int n = adj.size();
    std::vector<bool> visited(n, false);
    std::stack<int> st;

    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) dfs(v);
        }
        st.push(u);
    };

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) dfs(i);
    }

    while (!st.empty()) {
        result.push_back(st.top());
        st.pop();
    }
}

// ---------------------------------------------------
// 📌 Проверка на наличие цикла в графе (DFS)
// ---------------------------------------------------
bool hasCycle(const std::vector<std::vector<int>>& adj) {
    int n = adj.size();
    std::vector<int> color(n, 0); // 0 = white, 1 = gray, 2 = black

    std::function<bool(int)> dfs = [&](int u) -> bool {
        color[u] = 1;
        for (int v : adj[u]) {
            if (color[v] == 1) return true; // back edge
            if (color[v] == 0 && dfs(v)) return true;
        }
        color[u] = 2;
        return false;
    };

    for (int i = 0; i < n; ++i) {
        if (color[i] == 0 && dfs(i)) return true;
    }
    return false;
}

// ---------------------------------------------------
// 📌 Нахождение кратчайшего пути (Dijkstra)
// ---------------------------------------------------
std::vector<int> dijkstra(const std::vector<std::vector<std::pair<int, int>>>& adj, int start) {
    int n = adj.size();
    std::vector<int> dist(n, INT_MAX);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}
