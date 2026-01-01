/*
 * ============================================
 * ЗАДАЧИ С СОБЕСЕДОВАНИЙ - MODERN C++
 * ============================================
 * 
 * Классические алгоритмические задачи с решениями
 * на современном C++ с подробными объяснениями.
 * 
 * Компиляция: g++ -std=c++20 interview_problems.cpp
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include <numeric>
#include <limits>
#include <memory>
#include <optional>

// ============================================
// 📌 ARRAYS И STRINGS
// ============================================

namespace arrays_strings {

// Two Sum - найти два числа, сумма которых равна target
std::vector<int> two_sum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> seen;  // значение -> индекс
    
    for (int i = 0; i < nums.size(); ++i) {
        int complement = target - nums[i];
        if (auto it = seen.find(complement); it != seen.end()) {
            return {it->second, i};
        }
        seen[nums[i]] = i;
    }
    
    return {};  // Не найдено
}

// Longest Substring Without Repeating Characters
int length_of_longest_substring(const std::string& s) {
    std::unordered_map<char, int> char_index;  // символ -> последний индекс
    int max_len = 0;
    int start = 0;
    
    for (int end = 0; end < s.length(); ++end) {
        char c = s[end];
        
        // Если символ уже встречался, двигаем start
        if (auto it = char_index.find(c); it != char_index.end()) {
            start = std::max(start, it->second + 1);
        }
        
        char_index[c] = end;
        max_len = std::max(max_len, end - start + 1);
    }
    
    return max_len;
}

// Container With Most Water - максимальная площадь
int max_area(const std::vector<int>& height) {
    int left = 0;
    int right = height.size() - 1;
    int max_area = 0;
    
    while (left < right) {
        int h = std::min(height[left], height[right]);
        int width = right - left;
        max_area = std::max(max_area, h * width);
        
        // Двигаем указатель с меньшей высотой
        if (height[left] < height[right]) {
            ++left;
        } else {
            --right;
        }
    }
    
    return max_area;
}

// Trapping Rain Water
int trap(const std::vector<int>& height) {
    if (height.empty()) return 0;
    
    int left = 0, right = height.size() - 1;
    int left_max = 0, right_max = 0;
    int water = 0;
    
    while (left < right) {
        if (height[left] < height[right]) {
            if (height[left] >= left_max) {
                left_max = height[left];
            } else {
                water += left_max - height[left];
            }
            ++left;
        } else {
            if (height[right] >= right_max) {
                right_max = height[right];
            } else {
                water += right_max - height[right];
            }
            --right;
        }
    }
    
    return water;
}

// Product of Array Except Self
std::vector<int> product_except_self(const std::vector<int>& nums) {
    int n = nums.size();
    std::vector<int> result(n, 1);
    
    // Произведение слева
    int left_product = 1;
    for (int i = 0; i < n; ++i) {
        result[i] = left_product;
        left_product *= nums[i];
    }
    
    // Произведение справа
    int right_product = 1;
    for (int i = n - 1; i >= 0; --i) {
        result[i] *= right_product;
        right_product *= nums[i];
    }
    
    return result;
}

void demo() {
    std::cout << "=== Arrays and Strings ===\n";
    
    // Two Sum
    auto indices = two_sum({2, 7, 11, 15}, 9);
    std::cout << "Two Sum: [" << indices[0] << ", " << indices[1] << "]\n";
    
    // Longest Substring
    std::cout << "Longest substring: " 
              << length_of_longest_substring("abcabcbb") << '\n';
    
    // Container With Most Water
    std::cout << "Max area: " << max_area({1,8,6,2,5,4,8,3,7}) << '\n';
    
    // Trapping Rain Water
    std::cout << "Trapped water: " << trap({0,1,0,2,1,0,1,3,2,1,2,1}) << '\n';
}

} // namespace arrays_strings

// ============================================
// 📌 LINKED LISTS
// ============================================

namespace linked_lists {

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Reverse Linked List
ListNode* reverse_list(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* current = head;
    
    while (current) {
        ListNode* next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    return prev;
}

// Detect Cycle - Floyd's Cycle Detection
bool has_cycle(ListNode* head) {
    if (!head) return false;
    
    ListNode* slow = head;
    ListNode* fast = head;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            return true;  // Цикл найден
        }
    }
    
    return false;
}

// Merge Two Sorted Lists
ListNode* merge_two_lists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* current = &dummy;
    
    while (l1 && l2) {
        if (l1->val < l2->val) {
            current->next = l1;
            l1 = l1->next;
        } else {
            current->next = l2;
            l2 = l2->next;
        }
        current = current->next;
    }
    
    current->next = l1 ? l1 : l2;
    return dummy.next;
}

// Remove Nth Node From End
ListNode* remove_nth_from_end(ListNode* head, int n) {
    ListNode dummy(0);
    dummy.next = head;
    
    ListNode* fast = &dummy;
    ListNode* slow = &dummy;
    
    // Сдвигаем fast на n+1 шагов вперед
    for (int i = 0; i <= n; ++i) {
        fast = fast->next;
    }
    
    // Двигаем оба указателя
    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }
    
    // Удаляем узел
    ListNode* to_delete = slow->next;
    slow->next = slow->next->next;
    delete to_delete;
    
    return dummy.next;
}

// Copy List with Random Pointer
struct Node {
    int val;
    Node* next;
    Node* random;
    Node(int x) : val(x), next(nullptr), random(nullptr) {}
};

Node* copy_random_list(Node* head) {
    if (!head) return nullptr;
    
    std::unordered_map<Node*, Node*> old_to_new;
    
    // Первый проход - создаем копии узлов
    Node* current = head;
    while (current) {
        old_to_new[current] = new Node(current->val);
        current = current->next;
    }
    
    // Второй проход - связываем узлы
    current = head;
    while (current) {
        Node* new_node = old_to_new[current];
        new_node->next = old_to_new[current->next];
        new_node->random = old_to_new[current->random];
        current = current->next;
    }
    
    return old_to_new[head];
}

void demo() {
    std::cout << "\n=== Linked Lists ===\n";
    
    // Создаем список: 1 -> 2 -> 3
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    
    // Reverse
    head = reverse_list(head);
    std::cout << "Reversed list: " << head->val << " -> " 
              << head->next->val << " -> " << head->next->next->val << '\n';
    
    // Cleanup (упрощенно)
    // В реальном коде используйте умные указатели!
}

} // namespace linked_lists

// ============================================
// 📌 TREES
// ============================================

namespace trees {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Inorder Traversal (рекурсивный)
void inorder(TreeNode* root, std::vector<int>& result) {
    if (!root) return;
    inorder(root->left, result);
    result.push_back(root->val);
    inorder(root->right, result);
}

// Inorder Traversal (итеративный)
std::vector<int> inorder_iterative(TreeNode* root) {
    std::vector<int> result;
    std::stack<TreeNode*> stack;
    TreeNode* current = root;
    
    while (current || !stack.empty()) {
        // Идем влево до конца
        while (current) {
            stack.push(current);
            current = current->left;
        }
        
        current = stack.top();
        stack.pop();
        result.push_back(current->val);
        
        current = current->right;
    }
    
    return result;
}

// Level Order Traversal (BFS)
std::vector<std::vector<int>> level_order(TreeNode* root) {
    if (!root) return {};
    
    std::vector<std::vector<int>> result;
    std::queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        int level_size = q.size();
        std::vector<int> level;
        
        for (int i = 0; i < level_size; ++i) {
            TreeNode* node = q.front();
            q.pop();
            
            level.push_back(node->val);
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        
        result.push_back(level);
    }
    
    return result;
}

// Maximum Depth
int max_depth(TreeNode* root) {
    if (!root) return 0;
    return 1 + std::max(max_depth(root->left), max_depth(root->right));
}

// Validate BST
bool is_valid_bst_helper(TreeNode* root, long min_val, long max_val) {
    if (!root) return true;
    
    if (root->val <= min_val || root->val >= max_val) {
        return false;
    }
    
    return is_valid_bst_helper(root->left, min_val, root->val) &&
           is_valid_bst_helper(root->right, root->val, max_val);
}

bool is_valid_bst(TreeNode* root) {
    return is_valid_bst_helper(root, LONG_MIN, LONG_MAX);
}

// Lowest Common Ancestor
TreeNode* lowest_common_ancestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) {
        return root;
    }
    
    TreeNode* left = lowest_common_ancestor(root->left, p, q);
    TreeNode* right = lowest_common_ancestor(root->right, p, q);
    
    if (left && right) return root;  // p и q в разных поддеревьях
    return left ? left : right;      // Оба в одном поддереве
}

void demo() {
    std::cout << "\n=== Trees ===\n";
    
    // Создаем дерево:
    //       1
    //      / \
    //     2   3
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    
    std::cout << "Max depth: " << max_depth(root) << '\n';
    std::cout << "Is valid BST: " << std::boolalpha << is_valid_bst(root) << '\n';
}

} // namespace trees

// ============================================
// 📌 GRAPHS
// ============================================

namespace graphs {

// DFS рекурсивный
void dfs(int node, const std::vector<std::vector<int>>& graph, 
         std::vector<bool>& visited) {
    visited[node] = true;
    std::cout << node << " ";
    
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor, graph, visited);
        }
    }
}

// BFS
void bfs(int start, const std::vector<std::vector<int>>& graph) {
    std::vector<bool> visited(graph.size(), false);
    std::queue<int> q;
    
    q.push(start);
    visited[start] = true;
    
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        
        std::cout << node << " ";
        
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

// Dijkstra's Algorithm
std::vector<int> dijkstra(const std::vector<std::vector<std::pair<int, int>>>& graph, 
                          int start) {
    int n = graph.size();
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    dist[start] = 0;
    
    // Min-heap: (расстояние, узел)
    std::priority_queue<std::pair<int, int>,
                       std::vector<std::pair<int, int>>,
                       std::greater<>> pq;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;  // Уже нашли лучший путь
        
        for (auto [v, weight] : graph[u]) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    
    return dist;
}

// Topological Sort (DFS-based)
void topological_sort_dfs(int node, const std::vector<std::vector<int>>& graph,
                         std::vector<bool>& visited, std::stack<int>& stack) {
    visited[node] = true;
    
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            topological_sort_dfs(neighbor, graph, visited, stack);
        }
    }
    
    stack.push(node);
}

std::vector<int> topological_sort(const std::vector<std::vector<int>>& graph) {
    int n = graph.size();
    std::vector<bool> visited(n, false);
    std::stack<int> stack;
    
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            topological_sort_dfs(i, graph, visited, stack);
        }
    }
    
    std::vector<int> result;
    while (!stack.empty()) {
        result.push_back(stack.top());
        stack.pop();
    }
    
    return result;
}

// Detect Cycle in Directed Graph
bool has_cycle_dfs(int node, const std::vector<std::vector<int>>& graph,
                  std::vector<int>& color) {
    color[node] = 1;  // Gray (в процессе)
    
    for (int neighbor : graph[node]) {
        if (color[neighbor] == 1) {
            return true;  // Обратное ребро - цикл!
        }
        if (color[neighbor] == 0 && has_cycle_dfs(neighbor, graph, color)) {
            return true;
        }
    }
    
    color[node] = 2;  // Black (завершен)
    return false;
}

bool has_cycle(const std::vector<std::vector<int>>& graph) {
    int n = graph.size();
    std::vector<int> color(n, 0);  // 0=white, 1=gray, 2=black
    
    for (int i = 0; i < n; ++i) {
        if (color[i] == 0 && has_cycle_dfs(i, graph, color)) {
            return true;
        }
    }
    
    return false;
}

void demo() {
    std::cout << "\n=== Graphs ===\n";
    
    // Граф (список смежности)
    std::vector<std::vector<int>> graph = {
        {1, 2},    // 0 -> 1, 2
        {3},       // 1 -> 3
        {3},       // 2 -> 3
        {}         // 3
    };
    
    std::cout << "DFS: ";
    std::vector<bool> visited(4, false);
    dfs(0, graph, visited);
    std::cout << '\n';
    
    std::cout << "BFS: ";
    bfs(0, graph);
    std::cout << '\n';
}

} // namespace graphs

// ============================================
// 📌 DYNAMIC PROGRAMMING
// ============================================

namespace dynamic_programming {

// Fibonacci (с мемоизацией)
int fibonacci_memo(int n, std::vector<int>& memo) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];
    
    memo[n] = fibonacci_memo(n - 1, memo) + fibonacci_memo(n - 2, memo);
    return memo[n];
}

int fibonacci(int n) {
    std::vector<int> memo(n + 1, -1);
    return fibonacci_memo(n, memo);
}

// Longest Common Subsequence
int lcs(const std::string& s1, const std::string& s2) {
    int m = s1.length(), n = s2.length();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    return dp[m][n];
}

// 0/1 Knapsack
int knapsack(const std::vector<int>& weights, const std::vector<int>& values, 
             int capacity) {
    int n = weights.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));
    
    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= capacity; ++w) {
            if (weights[i - 1] <= w) {
                dp[i][w] = std::max(
                    dp[i - 1][w],
                    dp[i - 1][w - weights[i - 1]] + values[i - 1]
                );
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    
    return dp[n][capacity];
}

// Coin Change
int coin_change(const std::vector<int>& coins, int amount) {
    std::vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    
    for (int i = 1; i <= amount; ++i) {
        for (int coin : coins) {
            if (i >= coin) {
                dp[i] = std::min(dp[i], dp[i - coin] + 1);
            }
        }
    }
    
    return dp[amount] > amount ? -1 : dp[amount];
}

// Word Break
bool word_break(const std::string& s, const std::unordered_set<std::string>& dict) {
    int n = s.length();
    std::vector<bool> dp(n + 1, false);
    dp[0] = true;
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (dp[j] && dict.count(s.substr(j, i - j))) {
                dp[i] = true;
                break;
            }
        }
    }
    
    return dp[n];
}

void demo() {
    std::cout << "\n=== Dynamic Programming ===\n";
    
    std::cout << "Fibonacci(10): " << fibonacci(10) << '\n';
    std::cout << "LCS('abcde', 'ace'): " << lcs("abcde", "ace") << '\n';
    std::cout << "Coin change for 11: " 
              << coin_change({1, 2, 5}, 11) << '\n';
}

} // namespace dynamic_programming

// ============================================
// 📌 SORTING И SEARCHING
// ============================================

namespace sorting_searching {

// Binary Search
int binary_search(const std::vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;  // Не найдено
}

// Find First and Last Position
std::pair<int, int> search_range(const std::vector<int>& nums, int target) {
    auto lower = std::lower_bound(nums.begin(), nums.end(), target);
    auto upper = std::upper_bound(nums.begin(), nums.end(), target);
    
    if (lower == nums.end() || *lower != target) {
        return {-1, -1};
    }
    
    return {
        static_cast<int>(lower - nums.begin()),
        static_cast<int>(upper - nums.begin() - 1)
    };
}

// Merge Sort
void merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    
    for (int i = 0; i < temp.size(); ++i) {
        arr[left + i] = temp[i];
    }
}

void merge_sort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    
    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Quick Sort
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quick_sort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

// Kth Largest Element (Quick Select)
int find_kth_largest(std::vector<int>& nums, int k) {
    // nth_element помещает k-й элемент на правильное место
    std::nth_element(nums.begin(), nums.begin() + nums.size() - k, nums.end());
    return nums[nums.size() - k];
}

void demo() {
    std::cout << "\n=== Sorting and Searching ===\n";
    
    std::vector<int> arr = {5, 2, 8, 1, 9};
    merge_sort(arr, 0, arr.size() - 1);
    
    std::cout << "Sorted: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << '\n';
    
    std::cout << "Binary search for 8: " << binary_search(arr, 8) << '\n';
}

} // namespace sorting_searching

// ============================================
// 📌 BIT MANIPULATION
// ============================================

namespace bit_manipulation {

// Single Number - найти число, которое встречается один раз
int single_number(const std::vector<int>& nums) {
    int result = 0;
    for (int num : nums) {
        result ^= num;  // XOR всех чисел
    }
    return result;  // Повторяющиеся числа обнулятся
}

// Number of 1 Bits
int hamming_weight(uint32_t n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

// Reverse Bits
uint32_t reverse_bits(uint32_t n) {
    uint32_t result = 0;
    for (int i = 0; i < 32; ++i) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    return result;
}

// Missing Number
int missing_number(const std::vector<int>& nums) {
    int n = nums.size();
    int expected_sum = n * (n + 1) / 2;
    int actual_sum = std::accumulate(nums.begin(), nums.end(), 0);
    return expected_sum - actual_sum;
}

// Power of Two
bool is_power_of_two(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

void demo() {
    std::cout << "\n=== Bit Manipulation ===\n";
    
    std::cout << "Single number in {4,1,2,1,2}: " 
              << single_number({4, 1, 2, 1, 2}) << '\n';
    std::cout << "Hamming weight of 11: " << hamming_weight(11) << '\n';
    std::cout << "Missing number in {0,1,3}: " 
              << missing_number({0, 1, 3}) << '\n';
}

} // namespace bit_manipulation

// ============================================
// 📌 MODERN C++ SPECIFIC
// ============================================

namespace modern_cpp_problems {

// Простая реализация unique_ptr
template<typename T>
class SimpleUniquePtr {
    T* ptr_;
    
public:
    explicit SimpleUniquePtr(T* p = nullptr) : ptr_(p) {}
    
    ~SimpleUniquePtr() { delete ptr_; }
    
    // Запрещаем копирование
    SimpleUniquePtr(const SimpleUniquePtr&) = delete;
    SimpleUniquePtr& operator=(const SimpleUniquePtr&) = delete;
    
    // Разрешаем перемещение
    SimpleUniquePtr(SimpleUniquePtr&& other) noexcept
        : ptr_(std::exchange(other.ptr_, nullptr)) {}
    
    SimpleUniquePtr& operator=(SimpleUniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = std::exchange(other.ptr_, nullptr);
        }
        return *this;
    }
    
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    
    T* release() {
        return std::exchange(ptr_, nullptr);
    }
    
    void reset(T* p = nullptr) {
        delete ptr_;
        ptr_ = p;
    }
};

// Thread-safe Singleton (C++11+)
class Singleton {
    Singleton() = default;
    
public:
    static Singleton& instance() {
        static Singleton instance;  // Thread-safe с C++11
        return instance;
    }
    
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    
    void do_something() {
        std::cout << "Singleton method called\n";
    }
};

// LRU Cache
class LRUCache {
    struct Node {
        int key, value;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };
    
    int capacity_;
    std::unordered_map<int, Node*> cache_;
    Node* head_;  // Dummy head
    Node* tail_;  // Dummy tail
    
    void remove_node(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    void add_to_head(Node* node) {
        node->next = head_->next;
        node->prev = head_;
        head_->next->prev = node;
        head_->next = node;
    }
    
    void move_to_head(Node* node) {
        remove_node(node);
        add_to_head(node);
    }
    
    Node* remove_tail() {
        Node* node = tail_->prev;
        remove_node(node);
        return node;
    }
    
public:
    LRUCache(int capacity) : capacity_(capacity) {
        head_ = new Node(0, 0);
        tail_ = new Node(0, 0);
        head_->next = tail_;
        tail_->prev = head_;
    }
    
    ~LRUCache() {
        Node* current = head_;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    
    int get(int key) {
        if (auto it = cache_.find(key); it != cache_.end()) {
            Node* node = it->second;
            move_to_head(node);
            return node->value;
        }
        return -1;
    }
    
    void put(int key, int value) {
        if (auto it = cache_.find(key); it != cache_.end()) {
            Node* node = it->second;
            node->value = value;
            move_to_head(node);
        } else {
            Node* node = new Node(key, value);
            cache_[key] = node;
            add_to_head(node);
            
            if (cache_.size() > capacity_) {
                Node* tail = remove_tail();
                cache_.erase(tail->key);
                delete tail;
            }
        }
    }
};

void demo() {
    std::cout << "\n=== Modern C++ Specific ===\n";
    
    // Singleton
    Singleton::instance().do_something();
    
    // LRU Cache
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    std::cout << "Get 1: " << cache.get(1) << '\n';
    cache.put(3, 3);  // Вытесняет ключ 2
    std::cout << "Get 2: " << cache.get(2) << '\n';  // -1 (не найден)
    
    // Simple unique_ptr
    SimpleUniquePtr<int> ptr(new int(42));
    std::cout << "Unique ptr value: " << *ptr << '\n';
}

} // namespace modern_cpp_problems

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== Interview Problems - Modern C++ ===\n\n";
    
    arrays_strings::demo();
    linked_lists::demo();
    trees::demo();
    graphs::demo();
    dynamic_programming::demo();
    sorting_searching::demo();
    bit_manipulation::demo();
    modern_cpp_problems::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ Arrays/Strings - Two Sum, Longest Substring, Water Trapping\n";
    std::cout << "✓ Linked Lists - Reverse, Cycle Detection, Merge\n";
    std::cout << "✓ Trees - Traversals, BST Validation, LCA\n";
    std::cout << "✓ Graphs - DFS/BFS, Dijkstra, Topological Sort\n";
    std::cout << "✓ Dynamic Programming - Fibonacci, LCS, Knapsack\n";
    std::cout << "✓ Sorting/Searching - Binary Search, Merge Sort, Quick Sort\n";
    std::cout << "✓ Bit Manipulation - XOR tricks, Hamming weight\n";
    std::cout << "✓ Modern C++ - Smart pointers, Singleton, LRU Cache\n";
    
    return 0;
}