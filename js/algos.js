// ---------------------------------------------------
// 📌 Очередь (FIFO)
// ---------------------------------------------------
const queue = [];
queue.push(x);       // добавление
queue.shift();       // удаление


// ---------------------------------------------------
// 📌 Стек (LIFO)
// ---------------------------------------------------
const stack = [];
stack.push(x);       // добавление
stack.pop();         // удаление


// ---------------------------------------------------
// 📌 Двусторонняя очередь (Deque)
// ---------------------------------------------------
const deque = [];
deque.push(x);       // в конец
deque.unshift(y);    // в начало
deque.pop();         // с конца
deque.shift();       // с начала


// ---------------------------------------------------
// 📌 Связный список (ручная реализация)
// ---------------------------------------------------
class Node {
  constructor(val) {
    this.val = val;
    this.next = null;
  }
}

const head = new Node(1);
head.next = new Node(2);


// ---------------------------------------------------
// 📌 Множество (Set)
// ---------------------------------------------------
const set = new Set([1, 2]);
set.add(3);
set.delete(2);
set.has(1);


// ---------------------------------------------------
// 📌 Хэш-таблица / словарь (Map)
// ---------------------------------------------------
const map = new Map();
map.set('key', 123);
map.get('key');
map.has('key');
map.delete('key');


// ---------------------------------------------------
// 📌 Куча (MinHeap)
// ---------------------------------------------------
class MinHeap {
  constructor() {
    this.heap = [];
  }

  insert(val) { /* push и bubbleUp */
  }

  extractMin() { /* pop и sinkDown */
  }
}


// ---------------------------------------------------
// 📌 Бинарный поиск (на отсортированном массиве)
// ---------------------------------------------------
function binarySearch(arr, target) {
  let l = 0, r = arr.length - 1;
  while (l <= r) {
    const mid = Math.floor((l + r) / 2);
    if (arr[mid] === target) return mid;
    arr[mid] < target ? l = mid + 1 : r = mid - 1;
  }
  return -1;
}


// ---------------------------------------------------
// 📌 Обход графа — DFS (стек) / BFS (очередь)
// ---------------------------------------------------
function dfs(root) {
  const stack = [root], visited = new Set();
  while (stack.length) {
    const node = stack.pop();
    if (visited.has(node)) continue;
    visited.add(node);
    stack.push(...node.neighbors);
  }
}

function bfs(root) {
  const queue = [root], visited = new Set();
  while (queue.length) {
    const node = queue.shift();
    if (visited.has(node)) continue;
    visited.add(node);
    queue.push(...node.neighbors);
  }
}


// ---------------------------------------------------
// 📌 Топологическая сортировка (DFS)
// ---------------------------------------------------
function topoSort(adj) {
  const visited = new Set();
  const result = [];

  function dfs(v) {
    if (visited.has(v)) return;
    visited.add(v);
    for (const u of (adj[v] || [])) {
      dfs(u);
    }
    result.unshift(v);
  }

  for (const v in adj) {
    dfs(v);
  }
  return result;
}


// ---------------------------------------------------
// 📌 Проверка на цикл в графе (DFS)
// ---------------------------------------------------
function hasCycle(adj) {
  const color = {}; // 0=white, 1=gray, 2=black

  function dfs(v) {
    color[v] = 1;
    for (const u of (adj[v] || [])) {
      if (color[u] === 1) return true; // back edge
      if (color[u] === 0 && dfs(u)) return true;
    }
    color[v] = 2;
    return false;
  }

  for (const v in adj) {
    color[v] = color[v] || 0;
  }

  for (const v in adj) {
    if (color[v] === 0 && dfs(v)) return true;
  }
  return false;
}


// ---------------------------------------------------
// 📌 Union-Find (система непересекающихся множеств)
// ---------------------------------------------------
class UnionFind {
  constructor(size) {
    this.parent = Array.from({length: size}, (_, i) => i);
    this.rank = Array(size).fill(0);
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

    if (this.rank[rootX] < this.rank[rootY]) {
      this.parent[rootX] = rootY;
    } else if (this.rank[rootX] > this.rank[rootY]) {
      this.parent[rootY] = rootX;
    } else {
      this.parent[rootY] = rootX;
      this.rank[rootX]++;
    }
    return true;
  }
}