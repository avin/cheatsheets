# JavaScript Алгоритмические Паттерны - Шпаргалка

> **[📖 Краткое руководство по паттернам](./PATTERNS_GUIDE.md)** - словесное описание каждого паттерна

## 1. Arrays & Hashing

### Hash Map/Set для поиска O(1)
```javascript
// Two Sum - найти два числа с заданной суммой
function twoSum(nums, target) {
    const map = new Map(); // число -> индекс
    for (let i = 0; i < nums.length; i++) {
        const complement = target - nums[i];
        if (map.has(complement)) {
            return [map.get(complement), i];
        }
        map.set(nums[i], i);
    }
    return [];
}

// Проверка на дубликаты
function containsDuplicate(nums) {
    return new Set(nums).size !== nums.length;
}
```

### Prefix Sum (префиксные суммы)
```javascript
// Range Sum Query - сумма элементов в диапазоне [i, j]
class PrefixSum {
    constructor(nums) {
        this.prefix = [0];
        for (let num of nums) {
            this.prefix.push(this.prefix[this.prefix.length - 1] + num);
        }
    }

    sumRange(left, right) {
        return this.prefix[right + 1] - this.prefix[left];
    }
}

// Subarray Sum Equals K
function subarraySum(nums, k) {
    const map = new Map([[0, 1]]); // prefixSum -> count
    let sum = 0, count = 0;
    for (let num of nums) {
        sum += num;
        if (map.has(sum - k)) count += map.get(sum - k);
        map.set(sum, (map.get(sum) || 0) + 1);
    }
    return count;
}
```

### Counting/Frequency Map
```javascript
// Valid Anagram
function isAnagram(s, t) {
    if (s.length !== t.length) return false;
    const count = {};
    for (let char of s) count[char] = (count[char] || 0) + 1;
    for (let char of t) {
        if (!count[char]) return false;
        count[char]--;
    }
    return true;
}

// Top K Frequent Elements
function topKFrequent(nums, k) {
    const freq = new Map();
    for (let num of nums) {
        freq.set(num, (freq.get(num) || 0) + 1);
    }
    return [...freq.entries()]
        .sort((a, b) => b[1] - a[1])
        .slice(0, k)
        .map(x => x[0]);
}
```

---

## 2. Two Pointers (Два указателя)

### Встречные указатели (left/right)
```javascript
// Valid Palindrome
function isPalindrome(s) {
    s = s.toLowerCase().replace(/[^a-z0-9]/g, '');
    let left = 0, right = s.length - 1;
    while (left < right) {
        if (s[left] !== s[right]) return false;
        left++;
        right--;
    }
    return true;
}

// Two Sum II - отсортированный массив
function twoSumSorted(numbers, target) {
    let left = 0, right = numbers.length - 1;
    while (left < right) {
        const sum = numbers[left] + numbers[right];
        if (sum === target) return [left + 1, right + 1];
        if (sum < target) left++;
        else right--;
    }
    return [];
}

// Container With Most Water
function maxArea(height) {
    let left = 0, right = height.length - 1, maxArea = 0;
    while (left < right) {
        const area = Math.min(height[left], height[right]) * (right - left);
        maxArea = Math.max(maxArea, area);
        if (height[left] < height[right]) left++;
        else right--;
    }
    return maxArea;
}
```

### Быстрый и медленный указатель
```javascript
// Remove Duplicates from Sorted Array
function removeDuplicates(nums) {
    let slow = 0;
    for (let fast = 1; fast < nums.length; fast++) {
        if (nums[fast] !== nums[slow]) {
            slow++;
            nums[slow] = nums[fast];
        }
    }
    return slow + 1;
}

// Move Zeroes
function moveZeroes(nums) {
    let slow = 0;
    for (let fast = 0; fast < nums.length; fast++) {
        if (nums[fast] !== 0) {
            [nums[slow], nums[fast]] = [nums[fast], nums[slow]];
            slow++;
        }
    }
}
```

### Sorted array + two pointers
```javascript
// 3Sum
function threeSum(nums) {
    nums.sort((a, b) => a - b);
    const result = [];

    for (let i = 0; i < nums.length - 2; i++) {
        if (i > 0 && nums[i] === nums[i - 1]) continue; // skip duplicates

        let left = i + 1, right = nums.length - 1;
        while (left < right) {
            const sum = nums[i] + nums[left] + nums[right];
            if (sum === 0) {
                result.push([nums[i], nums[left], nums[right]]);
                while (left < right && nums[left] === nums[left + 1]) left++;
                while (left < right && nums[right] === nums[right - 1]) right--;
                left++;
                right--;
            } else if (sum < 0) {
                left++;
            } else {
                right--;
            }
        }
    }
    return result;
}
```

---

## 3. Sliding Window (Скользящее окно)

### Fixed size window
```javascript
// Maximum Average Subarray I (размер k)
function findMaxAverage(nums, k) {
    let sum = 0;
    for (let i = 0; i < k; i++) sum += nums[i];
    let maxSum = sum;

    for (let i = k; i < nums.length; i++) {
        sum += nums[i] - nums[i - k];
        maxSum = Math.max(maxSum, sum);
    }
    return maxSum / k;
}
```

### Variable size window
```javascript
// Longest Substring Without Repeating Characters
function lengthOfLongestSubstring(s) {
    const seen = new Set();
    let left = 0, maxLen = 0;

    for (let right = 0; right < s.length; right++) {
        while (seen.has(s[right])) {
            seen.delete(s[left]);
            left++;
        }
        seen.add(s[right]);
        maxLen = Math.max(maxLen, right - left + 1);
    }
    return maxLen;
}

// Minimum Size Subarray Sum
function minSubArrayLen(target, nums) {
    let left = 0, sum = 0, minLen = Infinity;

    for (let right = 0; right < nums.length; right++) {
        sum += nums[right];
        while (sum >= target) {
            minLen = Math.min(minLen, right - left + 1);
            sum -= nums[left];
            left++;
        }
    }
    return minLen === Infinity ? 0 : minLen;
}
```

### Substring problems
```javascript
// Minimum Window Substring
function minWindow(s, t) {
    const need = new Map();
    for (let char of t) need.set(char, (need.get(char) || 0) + 1);

    let left = 0, right = 0, valid = 0;
    let start = 0, minLen = Infinity;
    const window = new Map();

    while (right < s.length) {
        const c = s[right];
        right++;
        if (need.has(c)) {
            window.set(c, (window.get(c) || 0) + 1);
            if (window.get(c) === need.get(c)) valid++;
        }

        while (valid === need.size) {
            if (right - left < minLen) {
                start = left;
                minLen = right - left;
            }
            const d = s[left];
            left++;
            if (need.has(d)) {
                if (window.get(d) === need.get(d)) valid--;
                window.set(d, window.get(d) - 1);
            }
        }
    }
    return minLen === Infinity ? "" : s.substring(start, start + minLen);
}
```

---

## 4. Stack

### Monotonic Stack (монотонный стек)
```javascript
// Next Greater Element I
function nextGreaterElement(nums1, nums2) {
    const map = new Map();
    const stack = [];

    for (let num of nums2) {
        while (stack.length && stack[stack.length - 1] < num) {
            map.set(stack.pop(), num);
        }
        stack.push(num);
    }

    return nums1.map(num => map.get(num) || -1);
}

// Daily Temperatures
function dailyTemperatures(temperatures) {
    const result = new Array(temperatures.length).fill(0);
    const stack = []; // индексы

    for (let i = 0; i < temperatures.length; i++) {
        while (stack.length && temperatures[i] > temperatures[stack[stack.length - 1]]) {
            const idx = stack.pop();
            result[idx] = i - idx;
        }
        stack.push(i);
    }
    return result;
}
```

### Valid Parentheses
```javascript
function isValid(s) {
    const stack = [];
    const pairs = { '(': ')', '[': ']', '{': '}' };

    for (let char of s) {
        if (char in pairs) {
            stack.push(char);
        } else {
            if (!stack.length || pairs[stack.pop()] !== char) {
                return false;
            }
        }
    }
    return stack.length === 0;
}
```

### Evaluate expressions
```javascript
// Evaluate Reverse Polish Notation
function evalRPN(tokens) {
    const stack = [];
    const ops = {
        '+': (a, b) => a + b,
        '-': (a, b) => a - b,
        '*': (a, b) => a * b,
        '/': (a, b) => Math.trunc(a / b)
    };

    for (let token of tokens) {
        if (token in ops) {
            const b = stack.pop();
            const a = stack.pop();
            stack.push(ops[token](a, b));
        } else {
            stack.push(Number(token));
        }
    }
    return stack[0];
}
```

---

## 5. Binary Search

### На отсортированном массиве
```javascript
// Binary Search (базовый)
function binarySearch(nums, target) {
    let left = 0, right = nums.length - 1;

    while (left <= right) {
        const mid = Math.floor((left + right) / 2);
        if (nums[mid] === target) return mid;
        if (nums[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// Search Insert Position
function searchInsert(nums, target) {
    let left = 0, right = nums.length;
    while (left < right) {
        const mid = Math.floor((left + right) / 2);
        if (nums[mid] < target) left = mid + 1;
        else right = mid;
    }
    return left;
}
```

### Binary search on answer
```javascript
// Koko Eating Bananas
function minEatingSpeed(piles, h) {
    let left = 1, right = Math.max(...piles);

    const canFinish = (speed) => {
        let hours = 0;
        for (let pile of piles) {
            hours += Math.ceil(pile / speed);
        }
        return hours <= h;
    };

    while (left < right) {
        const mid = Math.floor((left + right) / 2);
        if (canFinish(mid)) right = mid;
        else left = mid + 1;
    }
    return left;
}
```

### Rotated sorted array
```javascript
// Search in Rotated Sorted Array
function searchRotated(nums, target) {
    let left = 0, right = nums.length - 1;

    while (left <= right) {
        const mid = Math.floor((left + right) / 2);
        if (nums[mid] === target) return mid;

        // Левая часть отсортирована
        if (nums[left] <= nums[mid]) {
            if (nums[left] <= target && target < nums[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        // Правая часть отсортирована
        else {
            if (nums[mid] < target && target <= nums[right]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    return -1;
}
```

---

## 6. Linked List

```javascript
// Определение ListNode
class ListNode {
    constructor(val = 0, next = null) {
        this.val = val;
        this.next = next;
    }
}
```

### Fast & Slow Pointers (Floyd's Cycle)
```javascript
// Linked List Cycle
function hasCycle(head) {
    let slow = head, fast = head;
    while (fast && fast.next) {
        slow = slow.next;
        fast = fast.next.next;
        if (slow === fast) return true;
    }
    return false;
}

// Middle of Linked List
function middleNode(head) {
    let slow = head, fast = head;
    while (fast && fast.next) {
        slow = slow.next;
        fast = fast.next.next;
    }
    return slow;
}
```

### Reverse linked list
```javascript
// Reverse Linked List (итеративно)
function reverseList(head) {
    let prev = null, curr = head;
    while (curr) {
        const next = curr.next;
        curr.next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

// Reverse Linked List (рекурсивно)
function reverseListRecursive(head) {
    if (!head || !head.next) return head;
    const newHead = reverseListRecursive(head.next);
    head.next.next = head;
    head.next = null;
    return newHead;
}
```

### Merge lists
```javascript
// Merge Two Sorted Lists
function mergeTwoLists(l1, l2) {
    const dummy = new ListNode(0);
    let curr = dummy;

    while (l1 && l2) {
        if (l1.val < l2.val) {
            curr.next = l1;
            l1 = l1.next;
        } else {
            curr.next = l2;
            l2 = l2.next;
        }
        curr = curr.next;
    }
    curr.next = l1 || l2;
    return dummy.next;
}

// Remove Nth Node From End
function removeNthFromEnd(head, n) {
    const dummy = new ListNode(0, head);
    let fast = dummy, slow = dummy;

    for (let i = 0; i <= n; i++) fast = fast.next;
    while (fast) {
        fast = fast.next;
        slow = slow.next;
    }
    slow.next = slow.next.next;
    return dummy.next;
}
```

---

## 7. Trees

```javascript
// Определение TreeNode
class TreeNode {
    constructor(val = 0, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}
```

### DFS (in/pre/post-order)
```javascript
// Inorder (левый -> корень -> правый)
function inorderTraversal(root) {
    const result = [];
    function dfs(node) {
        if (!node) return;
        dfs(node.left);
        result.push(node.val);
        dfs(node.right);
    }
    dfs(root);
    return result;
}

// Preorder (корень -> левый -> правый)
function preorderTraversal(root) {
    const result = [];
    function dfs(node) {
        if (!node) return;
        result.push(node.val);
        dfs(node.left);
        dfs(node.right);
    }
    dfs(root);
    return result;
}

// Postorder (левый -> правый -> корень)
function postorderTraversal(root) {
    const result = [];
    function dfs(node) {
        if (!node) return;
        dfs(node.left);
        dfs(node.right);
        result.push(node.val);
    }
    dfs(root);
    return result;
}

// Maximum Depth
function maxDepth(root) {
    if (!root) return 0;
    return 1 + Math.max(maxDepth(root.left), maxDepth(root.right));
}

// Diameter of Binary Tree
function diameterOfBinaryTree(root) {
    let diameter = 0;
    function height(node) {
        if (!node) return 0;
        const left = height(node.left);
        const right = height(node.right);
        diameter = Math.max(diameter, left + right);
        return 1 + Math.max(left, right);
    }
    height(root);
    return diameter;
}
```

### BFS (level-order)
```javascript
// Level Order Traversal
function levelOrder(root) {
    if (!root) return [];
    const result = [];
    const queue = [root];

    while (queue.length) {
        const levelSize = queue.length;
        const level = [];

        for (let i = 0; i < levelSize; i++) {
            const node = queue.shift();
            level.push(node.val);
            if (node.left) queue.push(node.left);
            if (node.right) queue.push(node.right);
        }
        result.push(level);
    }
    return result;
}

// Right Side View
function rightSideView(root) {
    if (!root) return [];
    const result = [];
    const queue = [root];

    while (queue.length) {
        const levelSize = queue.length;
        for (let i = 0; i < levelSize; i++) {
            const node = queue.shift();
            if (i === levelSize - 1) result.push(node.val);
            if (node.left) queue.push(node.left);
            if (node.right) queue.push(node.right);
        }
    }
    return result;
}
```

### Binary Search Tree операции
```javascript
// Validate BST
function isValidBST(root) {
    function validate(node, min, max) {
        if (!node) return true;
        if (node.val <= min || node.val >= max) return false;
        return validate(node.left, min, node.val) &&
               validate(node.right, node.val, max);
    }
    return validate(root, -Infinity, Infinity);
}

// Insert into BST
function insertIntoBST(root, val) {
    if (!root) return new TreeNode(val);
    if (val < root.val) root.left = insertIntoBST(root.left, val);
    else root.right = insertIntoBST(root.right, val);
    return root;
}
```

### Lowest Common Ancestor
```javascript
// LCA of Binary Tree
function lowestCommonAncestor(root, p, q) {
    if (!root || root === p || root === q) return root;

    const left = lowestCommonAncestor(root.left, p, q);
    const right = lowestCommonAncestor(root.right, p, q);

    if (left && right) return root;
    return left || right;
}
```

---

## 8. Graphs

```javascript
// Представление графа
// 1. Adjacency List: const graph = [[1,2], [0,2], [0,1]];
// 2. Adjacency Matrix: const graph = [[0,1,1], [1,0,1], [1,1,0]];
```

### DFS/BFS traversal
```javascript
// DFS (рекурсивный)
function dfsRecursive(graph, start, visited = new Set()) {
    visited.add(start);
    console.log(start);

    for (let neighbor of graph[start]) {
        if (!visited.has(neighbor)) {
            dfsRecursive(graph, neighbor, visited);
        }
    }
}

// DFS (итеративный со стеком)
function dfsIterative(graph, start) {
    const visited = new Set();
    const stack = [start];

    while (stack.length) {
        const node = stack.pop();
        if (visited.has(node)) continue;
        visited.add(node);
        console.log(node);

        for (let neighbor of graph[node]) {
            if (!visited.has(neighbor)) {
                stack.push(neighbor);
            }
        }
    }
}

// BFS (с очередью)
function bfs(graph, start) {
    const visited = new Set([start]);
    const queue = [start];

    while (queue.length) {
        const node = queue.shift();
        console.log(node);

        for (let neighbor of graph[node]) {
            if (!visited.has(neighbor)) {
                visited.add(neighbor);
                queue.push(neighbor);
            }
        }
    }
}

// Number of Islands (DFS)
function numIslands(grid) {
    let count = 0;

    function dfs(i, j) {
        if (i < 0 || i >= grid.length || j < 0 || j >= grid[0].length ||
            grid[i][j] === '0') return;
        grid[i][j] = '0';
        dfs(i + 1, j);
        dfs(i - 1, j);
        dfs(i, j + 1);
        dfs(i, j - 1);
    }

    for (let i = 0; i < grid.length; i++) {
        for (let j = 0; j < grid[0].length; j++) {
            if (grid[i][j] === '1') {
                count++;
                dfs(i, j);
            }
        }
    }
    return count;
}
```

### Dijkstra (кратчайший путь)
```javascript
// Dijkstra's Algorithm
function dijkstra(graph, start) {
    const distances = new Array(graph.length).fill(Infinity);
    distances[start] = 0;
    const visited = new Set();
    const pq = [[0, start]]; // [distance, node]

    while (pq.length) {
        pq.sort((a, b) => a[0] - b[0]);
        const [dist, node] = pq.shift();

        if (visited.has(node)) continue;
        visited.add(node);

        for (let [neighbor, weight] of graph[node]) {
            const newDist = dist + weight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                pq.push([newDist, neighbor]);
            }
        }
    }
    return distances;
}
```

### Topological Sort
```javascript
// Topological Sort (Kahn's Algorithm - BFS)
function topologicalSort(numCourses, prerequisites) {
    const graph = Array.from({ length: numCourses }, () => []);
    const indegree = new Array(numCourses).fill(0);

    for (let [course, prereq] of prerequisites) {
        graph[prereq].push(course);
        indegree[course]++;
    }

    const queue = [];
    for (let i = 0; i < numCourses; i++) {
        if (indegree[i] === 0) queue.push(i);
    }

    const result = [];
    while (queue.length) {
        const course = queue.shift();
        result.push(course);

        for (let next of graph[course]) {
            indegree[next]--;
            if (indegree[next] === 0) queue.push(next);
        }
    }

    return result.length === numCourses ? result : [];
}

// Topological Sort (DFS)
function topologicalSortDFS(numCourses, prerequisites) {
    const graph = Array.from({ length: numCourses }, () => []);
    for (let [course, prereq] of prerequisites) {
        graph[prereq].push(course);
    }

    const visited = new Set();
    const result = [];

    function dfs(node) {
        if (visited.has(node)) return;
        visited.add(node);
        for (let neighbor of graph[node]) {
            dfs(neighbor);
        }
        result.push(node);
    }

    for (let i = 0; i < numCourses; i++) {
        dfs(i);
    }
    return result.reverse();
}
```

### Union Find (Disjoint Set)
```javascript
class UnionFind {
    constructor(n) {
        this.parent = Array.from({ length: n }, (_, i) => i);
        this.rank = new Array(n).fill(1);
    }

    find(x) {
        if (this.parent[x] !== x) {
            this.parent[x] = this.find(this.parent[x]); // path compression
        }
        return this.parent[x];
    }

    union(x, y) {
        const rootX = this.find(x);
        const rootY = this.find(y);

        if (rootX === rootY) return false;

        // Union by rank
        if (this.rank[rootX] > this.rank[rootY]) {
            this.parent[rootY] = rootX;
        } else if (this.rank[rootX] < this.rank[rootY]) {
            this.parent[rootX] = rootY;
        } else {
            this.parent[rootY] = rootX;
            this.rank[rootX]++;
        }
        return true;
    }
}

// Number of Connected Components
function countComponents(n, edges) {
    const uf = new UnionFind(n);
    for (let [u, v] of edges) {
        uf.union(u, v);
    }
    return new Set(Array.from({ length: n }, (_, i) => uf.find(i))).size;
}
```

### Detect cycle
```javascript
// Detect Cycle in Undirected Graph (Union Find)
function hasCycleUndirected(n, edges) {
    const uf = new UnionFind(n);
    for (let [u, v] of edges) {
        if (!uf.union(u, v)) return true; // уже в одном компоненте
    }
    return false;
}

// Detect Cycle in Directed Graph (DFS)
function hasCycleDirected(numCourses, prerequisites) {
    const graph = Array.from({ length: numCourses }, () => []);
    for (let [course, prereq] of prerequisites) {
        graph[prereq].push(course);
    }

    const visiting = new Set();
    const visited = new Set();

    function dfs(node) {
        if (visiting.has(node)) return true; // cycle detected
        if (visited.has(node)) return false;

        visiting.add(node);
        for (let neighbor of graph[node]) {
            if (dfs(neighbor)) return true;
        }
        visiting.delete(node);
        visited.add(node);
        return false;
    }

    for (let i = 0; i < numCourses; i++) {
        if (dfs(i)) return true;
    }
    return false;
}
```

---

## 9. Dynamic Programming

### 1D DP (Fibonacci-like)
```javascript
// Fibonacci
function fib(n) {
    if (n <= 1) return n;
    let prev2 = 0, prev1 = 1;
    for (let i = 2; i <= n; i++) {
        const curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

// Climbing Stairs
function climbStairs(n) {
    if (n <= 2) return n;
    let prev2 = 1, prev1 = 2;
    for (let i = 3; i <= n; i++) {
        const curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

// House Robber
function rob(nums) {
    if (nums.length === 0) return 0;
    if (nums.length === 1) return nums[0];
    let prev2 = 0, prev1 = 0;
    for (let num of nums) {
        const curr = Math.max(prev1, prev2 + num);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
```

### 2D DP (grid problems)
```javascript
// Unique Paths
function uniquePaths(m, n) {
    const dp = Array(m).fill(0).map(() => Array(n).fill(1));

    for (let i = 1; i < m; i++) {
        for (let j = 1; j < n; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }
    return dp[m - 1][n - 1];
}

// Minimum Path Sum
function minPathSum(grid) {
    const m = grid.length, n = grid[0].length;
    const dp = Array(m).fill(0).map(() => Array(n).fill(0));
    dp[0][0] = grid[0][0];

    for (let i = 1; i < m; i++) dp[i][0] = dp[i - 1][0] + grid[i][0];
    for (let j = 1; j < n; j++) dp[0][j] = dp[0][j - 1] + grid[0][j];

    for (let i = 1; i < m; i++) {
        for (let j = 1; j < n; j++) {
            dp[i][j] = Math.min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
        }
    }
    return dp[m - 1][n - 1];
}
```

### Knapsack (0/1, unbounded)
```javascript
// 0/1 Knapsack
function knapsack01(weights, values, capacity) {
    const n = weights.length;
    const dp = Array(n + 1).fill(0).map(() => Array(capacity + 1).fill(0));

    for (let i = 1; i <= n; i++) {
        for (let w = 0; w <= capacity; w++) {
            if (weights[i - 1] <= w) {
                dp[i][w] = Math.max(
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

// Unbounded Knapsack (можно брать элементы несколько раз)
function knapsackUnbounded(weights, values, capacity) {
    const dp = new Array(capacity + 1).fill(0);

    for (let w = 0; w <= capacity; w++) {
        for (let i = 0; i < weights.length; i++) {
            if (weights[i] <= w) {
                dp[w] = Math.max(dp[w], dp[w - weights[i]] + values[i]);
            }
        }
    }
    return dp[capacity];
}

// Coin Change (минимум монет)
function coinChange(coins, amount) {
    const dp = new Array(amount + 1).fill(Infinity);
    dp[0] = 0;

    for (let i = 1; i <= amount; i++) {
        for (let coin of coins) {
            if (coin <= i) {
                dp[i] = Math.min(dp[i], dp[i - coin] + 1);
            }
        }
    }
    return dp[amount] === Infinity ? -1 : dp[amount];
}
```

### LCS/LIS (longest common/increasing subsequence)
```javascript
// Longest Common Subsequence
function longestCommonSubsequence(text1, text2) {
    const m = text1.length, n = text2.length;
    const dp = Array(m + 1).fill(0).map(() => Array(n + 1).fill(0));

    for (let i = 1; i <= m; i++) {
        for (let j = 1; j <= n; j++) {
            if (text1[i - 1] === text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

// Longest Increasing Subsequence (O(n^2))
function lengthOfLIS(nums) {
    const dp = new Array(nums.length).fill(1);

    for (let i = 1; i < nums.length; i++) {
        for (let j = 0; j < i; j++) {
            if (nums[j] < nums[i]) {
                dp[i] = Math.max(dp[i], dp[j] + 1);
            }
        }
    }
    return Math.max(...dp);
}

// LIS (O(n log n) с binary search)
function lengthOfLISOptimized(nums) {
    const tails = [];

    for (let num of nums) {
        let left = 0, right = tails.length;
        while (left < right) {
            const mid = Math.floor((left + right) / 2);
            if (tails[mid] < num) left = mid + 1;
            else right = mid;
        }
        if (left === tails.length) tails.push(num);
        else tails[left] = num;
    }
    return tails.length;
}
```

### Kadane's Algorithm
```javascript
// Maximum Subarray Sum
function maxSubArray(nums) {
    let maxSum = nums[0], currSum = nums[0];

    for (let i = 1; i < nums.length; i++) {
        currSum = Math.max(nums[i], currSum + nums[i]);
        maxSum = Math.max(maxSum, currSum);
    }
    return maxSum;
}

// Maximum Product Subarray
function maxProduct(nums) {
    let maxProd = nums[0], minProd = nums[0], result = nums[0];

    for (let i = 1; i < nums.length; i++) {
        if (nums[i] < 0) {
            [maxProd, minProd] = [minProd, maxProd];
        }
        maxProd = Math.max(nums[i], maxProd * nums[i]);
        minProd = Math.min(nums[i], minProd * nums[i]);
        result = Math.max(result, maxProd);
    }
    return result;
}
```

---

## 10. Backtracking

### Combinations
```javascript
// Combinations (выбрать k элементов из n)
function combine(n, k) {
    const result = [];

    function backtrack(start, path) {
        if (path.length === k) {
            result.push([...path]);
            return;
        }
        for (let i = start; i <= n; i++) {
            path.push(i);
            backtrack(i + 1, path);
            path.pop();
        }
    }

    backtrack(1, []);
    return result;
}

// Combination Sum (с повторениями)
function combinationSum(candidates, target) {
    const result = [];

    function backtrack(start, path, sum) {
        if (sum === target) {
            result.push([...path]);
            return;
        }
        if (sum > target) return;

        for (let i = start; i < candidates.length; i++) {
            path.push(candidates[i]);
            backtrack(i, path, sum + candidates[i]); // i, не i+1 (можно повторять)
            path.pop();
        }
    }

    backtrack(0, [], 0);
    return result;
}
```

### Permutations
```javascript
// Permutations (все перестановки)
function permute(nums) {
    const result = [];

    function backtrack(path, used) {
        if (path.length === nums.length) {
            result.push([...path]);
            return;
        }

        for (let i = 0; i < nums.length; i++) {
            if (used[i]) continue;
            path.push(nums[i]);
            used[i] = true;
            backtrack(path, used);
            path.pop();
            used[i] = false;
        }
    }

    backtrack([], new Array(nums.length).fill(false));
    return result;
}

// Permutations II (с дубликатами)
function permuteUnique(nums) {
    nums.sort((a, b) => a - b);
    const result = [];

    function backtrack(path, used) {
        if (path.length === nums.length) {
            result.push([...path]);
            return;
        }

        for (let i = 0; i < nums.length; i++) {
            if (used[i]) continue;
            if (i > 0 && nums[i] === nums[i - 1] && !used[i - 1]) continue;

            path.push(nums[i]);
            used[i] = true;
            backtrack(path, used);
            path.pop();
            used[i] = false;
        }
    }

    backtrack([], new Array(nums.length).fill(false));
    return result;
}
```

### Subsets
```javascript
// Subsets (все подмножества)
function subsets(nums) {
    const result = [];

    function backtrack(start, path) {
        result.push([...path]);

        for (let i = start; i < nums.length; i++) {
            path.push(nums[i]);
            backtrack(i + 1, path);
            path.pop();
        }
    }

    backtrack(0, []);
    return result;
}

// Subsets II (с дубликатами)
function subsetsWithDup(nums) {
    nums.sort((a, b) => a - b);
    const result = [];

    function backtrack(start, path) {
        result.push([...path]);

        for (let i = start; i < nums.length; i++) {
            if (i > start && nums[i] === nums[i - 1]) continue;
            path.push(nums[i]);
            backtrack(i + 1, path);
            path.pop();
        }
    }

    backtrack(0, []);
    return result;
}
```

### N-Queens pattern
```javascript
// N-Queens
function solveNQueens(n) {
    const result = [];
    const board = Array(n).fill(0).map(() => Array(n).fill('.'));
    const cols = new Set();
    const diag1 = new Set(); // row - col
    const diag2 = new Set(); // row + col

    function backtrack(row) {
        if (row === n) {
            result.push(board.map(r => r.join('')));
            return;
        }

        for (let col = 0; col < n; col++) {
            if (cols.has(col) || diag1.has(row - col) || diag2.has(row + col)) {
                continue;
            }

            board[row][col] = 'Q';
            cols.add(col);
            diag1.add(row - col);
            diag2.add(row + col);

            backtrack(row + 1);

            board[row][col] = '.';
            cols.delete(col);
            diag1.delete(row - col);
            diag2.delete(row + col);
        }
    }

    backtrack(0);
    return result;
}

// Word Search
function wordSearch(board, word) {
    const m = board.length, n = board[0].length;

    function backtrack(i, j, k) {
        if (k === word.length) return true;
        if (i < 0 || i >= m || j < 0 || j >= n || board[i][j] !== word[k]) {
            return false;
        }

        const temp = board[i][j];
        board[i][j] = '#'; // mark visited

        const found = backtrack(i + 1, j, k + 1) ||
                      backtrack(i - 1, j, k + 1) ||
                      backtrack(i, j + 1, k + 1) ||
                      backtrack(i, j - 1, k + 1);

        board[i][j] = temp; // restore
        return found;
    }

    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            if (backtrack(i, j, 0)) return true;
        }
    }
    return false;
}
```

---

## 11. Greedy

### Intervals (merge, schedule)
```javascript
// Merge Intervals
function merge(intervals) {
    intervals.sort((a, b) => a[0] - b[0]);
    const result = [intervals[0]];

    for (let i = 1; i < intervals.length; i++) {
        const last = result[result.length - 1];
        if (intervals[i][0] <= last[1]) {
            last[1] = Math.max(last[1], intervals[i][1]);
        } else {
            result.push(intervals[i]);
        }
    }
    return result;
}

// Insert Interval
function insert(intervals, newInterval) {
    const result = [];
    let i = 0;

    // Add all intervals before newInterval
    while (i < intervals.length && intervals[i][1] < newInterval[0]) {
        result.push(intervals[i]);
        i++;
    }

    // Merge overlapping intervals
    while (i < intervals.length && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = Math.min(newInterval[0], intervals[i][0]);
        newInterval[1] = Math.max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.push(newInterval);

    // Add remaining intervals
    while (i < intervals.length) {
        result.push(intervals[i]);
        i++;
    }

    return result;
}

// Non-overlapping Intervals (минимум удалений)
function eraseOverlapIntervals(intervals) {
    intervals.sort((a, b) => a[1] - b[1]); // sort by end time
    let end = intervals[0][1];
    let count = 0;

    for (let i = 1; i < intervals.length; i++) {
        if (intervals[i][0] < end) {
            count++; // overlap, remove this interval
        } else {
            end = intervals[i][1];
        }
    }
    return count;
}

// Meeting Rooms II (минимум комнат)
function minMeetingRooms(intervals) {
    const starts = intervals.map(x => x[0]).sort((a, b) => a - b);
    const ends = intervals.map(x => x[1]).sort((a, b) => a - b);
    let rooms = 0, endPtr = 0;

    for (let i = 0; i < starts.length; i++) {
        if (starts[i] < ends[endPtr]) {
            rooms++;
        } else {
            endPtr++;
        }
    }
    return rooms;
}
```

### Jump Game pattern
```javascript
// Jump Game
function canJump(nums) {
    let maxReach = 0;
    for (let i = 0; i < nums.length; i++) {
        if (i > maxReach) return false;
        maxReach = Math.max(maxReach, i + nums[i]);
    }
    return true;
}

// Jump Game II (минимум прыжков)
function jump(nums) {
    let jumps = 0, currEnd = 0, maxReach = 0;

    for (let i = 0; i < nums.length - 1; i++) {
        maxReach = Math.max(maxReach, i + nums[i]);
        if (i === currEnd) {
            jumps++;
            currEnd = maxReach;
        }
    }
    return jumps;
}
```

### Two pointer greedy
```javascript
// Assign Cookies
function findContentChildren(g, s) {
    g.sort((a, b) => a - b);
    s.sort((a, b) => a - b);
    let i = 0, j = 0;

    while (i < g.length && j < s.length) {
        if (s[j] >= g[i]) i++;
        j++;
    }
    return i;
}
```

---

## 12. Heap (Priority Queue)

```javascript
// MinHeap implementation
class MinHeap {
    constructor() {
        this.heap = [];
    }

    push(val) {
        this.heap.push(val);
        this.bubbleUp(this.heap.length - 1);
    }

    pop() {
        if (this.heap.length === 0) return null;
        if (this.heap.length === 1) return this.heap.pop();

        const min = this.heap[0];
        this.heap[0] = this.heap.pop();
        this.bubbleDown(0);
        return min;
    }

    peek() {
        return this.heap[0];
    }

    size() {
        return this.heap.length;
    }

    bubbleUp(idx) {
        while (idx > 0) {
            const parentIdx = Math.floor((idx - 1) / 2);
            if (this.heap[idx] >= this.heap[parentIdx]) break;
            [this.heap[idx], this.heap[parentIdx]] = [this.heap[parentIdx], this.heap[idx]];
            idx = parentIdx;
        }
    }

    bubbleDown(idx) {
        while (true) {
            let smallest = idx;
            const left = 2 * idx + 1;
            const right = 2 * idx + 2;

            if (left < this.heap.length && this.heap[left] < this.heap[smallest]) {
                smallest = left;
            }
            if (right < this.heap.length && this.heap[right] < this.heap[smallest]) {
                smallest = right;
            }
            if (smallest === idx) break;

            [this.heap[idx], this.heap[smallest]] = [this.heap[smallest], this.heap[idx]];
            idx = smallest;
        }
    }
}
```

### Top K elements
```javascript
// Kth Largest Element
function findKthLargest(nums, k) {
    const minHeap = new MinHeap();

    for (let num of nums) {
        minHeap.push(num);
        if (minHeap.size() > k) {
            minHeap.pop();
        }
    }
    return minHeap.peek();
}

// Top K Frequent Elements
function topKFrequent(nums, k) {
    const freq = new Map();
    for (let num of nums) {
        freq.set(num, (freq.get(num) || 0) + 1);
    }

    // Bucket sort approach
    const buckets = Array(nums.length + 1).fill(0).map(() => []);
    for (let [num, count] of freq) {
        buckets[count].push(num);
    }

    const result = [];
    for (let i = buckets.length - 1; i >= 0 && result.length < k; i--) {
        result.push(...buckets[i]);
    }
    return result.slice(0, k);
}
```

### Merge K sorted
```javascript
// Merge K Sorted Lists
function mergeKLists(lists) {
    const minHeap = new MinHeap();
    minHeap.heap = lists.filter(l => l).sort((a, b) => a.val - b.val);

    const dummy = new ListNode(0);
    let curr = dummy;

    while (minHeap.size() > 0) {
        const node = minHeap.pop();
        curr.next = node;
        curr = curr.next;

        if (node.next) {
            minHeap.push(node.next);
        }
    }
    return dummy.next;
}
```

### Meeting Rooms
```javascript
// Meeting Rooms II (с heap)
function minMeetingRoomsHeap(intervals) {
    intervals.sort((a, b) => a[0] - b[0]);
    const minHeap = new MinHeap();

    for (let interval of intervals) {
        if (minHeap.size() > 0 && minHeap.peek() <= interval[0]) {
            minHeap.pop();
        }
        minHeap.push(interval[1]);
    }
    return minHeap.size();
}
```

---

## 13. Bit Manipulation

### XOR tricks
```javascript
// Single Number (один элемент встречается 1 раз, остальные 2)
function singleNumber(nums) {
    let result = 0;
    for (let num of nums) {
        result ^= num; // XOR: a ^ a = 0, a ^ 0 = a
    }
    return result;
}

// Missing Number
function missingNumber(nums) {
    let xor = 0;
    for (let i = 0; i <= nums.length; i++) {
        xor ^= i;
    }
    for (let num of nums) {
        xor ^= num;
    }
    return xor;
}
```

### Count bits
```javascript
// Number of 1 Bits (Hamming Weight)
function hammingWeight(n) {
    let count = 0;
    while (n !== 0) {
        count++;
        n &= (n - 1); // удаляет самый правый 1 бит
    }
    return count;
}

// Counting Bits (от 0 до n)
function countBits(n) {
    const result = new Array(n + 1).fill(0);
    for (let i = 1; i <= n; i++) {
        result[i] = result[i >> 1] + (i & 1);
    }
    return result;
}
```

### Bit operations
```javascript
// Полезные операции
function bitOperations() {
    // Проверить, установлен ли i-й бит
    const isSet = (num, i) => (num & (1 << i)) !== 0;

    // Установить i-й бит
    const setBit = (num, i) => num | (1 << i);

    // Очистить i-й бит
    const clearBit = (num, i) => num & ~(1 << i);

    // Переключить i-й бит
    const toggleBit = (num, i) => num ^ (1 << i);

    // Проверить, степень ли 2
    const isPowerOfTwo = (n) => n > 0 && (n & (n - 1)) === 0;

    // Получить самый правый установленный бит
    const rightmostSetBit = (n) => n & -n;

    return { isSet, setBit, clearBit, toggleBit, isPowerOfTwo, rightmostSetBit };
}

// Reverse Bits
function reverseBits(n) {
    let result = 0;
    for (let i = 0; i < 32; i++) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    return result >>> 0; // unsigned right shift
}
```

---

## 14. Trie

### Prefix tree operations
```javascript
class TrieNode {
    constructor() {
        this.children = {};
        this.isEndOfWord = false;
    }
}

class Trie {
    constructor() {
        this.root = new TrieNode();
    }

    insert(word) {
        let node = this.root;
        for (let char of word) {
            if (!node.children[char]) {
                node.children[char] = new TrieNode();
            }
            node = node.children[char];
        }
        node.isEndOfWord = true;
    }

    search(word) {
        let node = this.root;
        for (let char of word) {
            if (!node.children[char]) return false;
            node = node.children[char];
        }
        return node.isEndOfWord;
    }

    startsWith(prefix) {
        let node = this.root;
        for (let char of prefix) {
            if (!node.children[char]) return false;
            node = node.children[char];
        }
        return true;
    }
}
```

### Word search
```javascript
// Word Search II (найти все слова из словаря на доске)
function findWords(board, words) {
    const trie = new Trie();
    for (let word of words) {
        trie.insert(word);
    }

    const result = new Set();
    const m = board.length, n = board[0].length;

    function backtrack(i, j, node, path) {
        if (i < 0 || i >= m || j < 0 || j >= n || board[i][j] === '#') return;

        const char = board[i][j];
        if (!node.children[char]) return;

        node = node.children[char];
        path += char;

        if (node.isEndOfWord) {
            result.add(path);
        }

        const temp = board[i][j];
        board[i][j] = '#';

        backtrack(i + 1, j, node, path);
        backtrack(i - 1, j, node, path);
        backtrack(i, j + 1, node, path);
        backtrack(i, j - 1, node, path);

        board[i][j] = temp;
    }

    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            backtrack(i, j, trie.root, '');
        }
    }
    return [...result];
}
```

---

## 15. Advanced

### Segment Tree
```javascript
// Segment Tree для Range Sum Query
class SegmentTree {
    constructor(nums) {
        this.n = nums.length;
        this.tree = new Array(4 * this.n).fill(0);
        if (this.n > 0) this.build(nums, 0, 0, this.n - 1);
    }

    build(nums, node, start, end) {
        if (start === end) {
            this.tree[node] = nums[start];
            return;
        }

        const mid = Math.floor((start + end) / 2);
        const leftNode = 2 * node + 1;
        const rightNode = 2 * node + 2;

        this.build(nums, leftNode, start, mid);
        this.build(nums, rightNode, mid + 1, end);
        this.tree[node] = this.tree[leftNode] + this.tree[rightNode];
    }

    update(idx, val, node = 0, start = 0, end = this.n - 1) {
        if (start === end) {
            this.tree[node] = val;
            return;
        }

        const mid = Math.floor((start + end) / 2);
        const leftNode = 2 * node + 1;
        const rightNode = 2 * node + 2;

        if (idx <= mid) {
            this.update(idx, val, leftNode, start, mid);
        } else {
            this.update(idx, val, rightNode, mid + 1, end);
        }
        this.tree[node] = this.tree[leftNode] + this.tree[rightNode];
    }

    query(left, right, node = 0, start = 0, end = this.n - 1) {
        if (right < start || left > end) return 0;
        if (left <= start && end <= right) return this.tree[node];

        const mid = Math.floor((start + end) / 2);
        const leftNode = 2 * node + 1;
        const rightNode = 2 * node + 2;

        const leftSum = this.query(left, right, leftNode, start, mid);
        const rightSum = this.query(left, right, rightNode, mid + 1, end);
        return leftSum + rightSum;
    }
}
```

### Fenwick Tree (Binary Indexed Tree)
```javascript
// Fenwick Tree для Range Sum Query
class FenwickTree {
    constructor(n) {
        this.n = n;
        this.tree = new Array(n + 1).fill(0);
    }

    update(idx, delta) {
        idx++; // 1-indexed
        while (idx <= this.n) {
            this.tree[idx] += delta;
            idx += idx & -idx; // добавить последний установленный бит
        }
    }

    query(idx) {
        idx++; // 1-indexed
        let sum = 0;
        while (idx > 0) {
            sum += this.tree[idx];
            idx -= idx & -idx; // удалить последний установленный бит
        }
        return sum;
    }

    rangeQuery(left, right) {
        return this.query(right) - (left > 0 ? this.query(left - 1) : 0);
    }
}

// Range Sum Query - Mutable
class NumArray {
    constructor(nums) {
        this.nums = nums;
        this.fenwick = new FenwickTree(nums.length);
        for (let i = 0; i < nums.length; i++) {
            this.fenwick.update(i, nums[i]);
        }
    }

    update(index, val) {
        const delta = val - this.nums[index];
        this.nums[index] = val;
        this.fenwick.update(index, delta);
    }

    sumRange(left, right) {
        return this.fenwick.rangeQuery(left, right);
    }
}
```

---

## Сложность алгоритмов (Quick Reference)

### Time Complexity
```
O(1)         - Константная
O(log n)     - Логарифмическая (binary search, balanced tree)
O(n)         - Линейная (один проход по массиву)
O(n log n)   - Linearithmic (merge sort, heap sort)
O(n²)        - Квадратичная (вложенные циклы)
O(2ⁿ)        - Экспоненциальная (рекурсивный fibonacci)
O(n!)        - Факториальная (все перестановки)
```

### Space Complexity
```
O(1)         - Константная (in-place алгоритмы)
O(log n)     - Рекурсивный стек (binary search)
O(n)         - Дополнительный массив/структура данных
O(n²)        - 2D массив
```

---

## Полезные шаблоны кода

### Инициализация массивов
```javascript
// 1D массив
const arr = new Array(n).fill(0);

// 2D массив
const matrix = Array(m).fill(0).map(() => Array(n).fill(0));

// Array.from
const arr = Array.from({ length: n }, (_, i) => i); // [0, 1, 2, ..., n-1]
```

### Работа с Map/Set
```javascript
// Map
const map = new Map();
map.set(key, value);
map.get(key);
map.has(key);
map.delete(key);
map.size;

// Set
const set = new Set();
set.add(value);
set.has(value);
set.delete(value);
set.size;
```

### Сортировка
```javascript
// По возрастанию
arr.sort((a, b) => a - b);

// По убыванию
arr.sort((a, b) => b - a);

// Custom comparator
arr.sort((a, b) => {
    if (a[0] !== b[0]) return a[0] - b[0];
    return a[1] - b[1];
});
```

### Строки
```javascript
// Split и join
const words = str.split(' ');
const str = words.join(' ');

// Char array
const chars = str.split('');
chars.reverse();
const reversed = chars.join('');

// Char code
const code = str.charCodeAt(0); // 'a' -> 97
const char = String.fromCharCode(97); // 97 -> 'a'
```

---

**Совет**: Практикуйте каждый паттерн на LeetCode, HackerRank или Codeforces для закрепления!
