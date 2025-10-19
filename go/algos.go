package cheatsheets

import (
	"container/heap"
	"container/list"
	"fmt"
	"sort"
)

// ---------------------------------------------------
// 📌 Стек на срезе
// ---------------------------------------------------
type Stack[T any] []T

func (s *Stack[T]) Push(v T) {
	*s = append(*s, v)
}

func (s *Stack[T]) Pop() (T, bool) {
	if len(*s) == 0 {
		var zero T
		return zero, false
	}
	idx := len(*s) - 1
	v := (*s)[idx]
	*s = (*s)[:idx]
	return v, true
}

// ---------------------------------------------------
// 📌 Очередь и двусторонняя очередь
// ---------------------------------------------------
type Queue[T any] struct {
	data []T
}

func (q *Queue[T]) Push(v T) {
	q.data = append(q.data, v)
}

func (q *Queue[T]) Pop() (T, bool) {
	if len(q.data) == 0 {
		var zero T
		return zero, false
	}
	v := q.data[0]
	q.data = q.data[1:]
	return v, true
}

type Deque[T any] struct {
	data []T
}

func (d *Deque[T]) PushFront(v T) {
	d.data = append([]T{v}, d.data...)
}

func (d *Deque[T]) PushBack(v T) {
	d.data = append(d.data, v)
}

func (d *Deque[T]) PopFront() (T, bool) {
	if len(d.data) == 0 {
		var zero T
		return zero, false
	}
	v := d.data[0]
	d.data = d.data[1:]
	return v, true
}

func (d *Deque[T]) PopBack() (T, bool) {
	if len(d.data) == 0 {
		var zero T
		return zero, false
	}
	idx := len(d.data) - 1
	v := d.data[idx]
	d.data = d.data[:idx]
	return v, true
}

func queueExample() {
	q := Queue[int]{}
	q.Push(1)
	q.Push(2)
	fmt.Println(q.Pop())
	fmt.Println(q.Pop())

	d := Deque[int]{}
	d.PushBack(1)
	d.PushFront(0)
	fmt.Println(d.PopFront())
	fmt.Println(d.PopBack())
}

// ---------------------------------------------------
// 📌 Куча / приоритетная очередь
// ---------------------------------------------------
type Item struct {
	Value    string
	Priority int
}

type PriorityQueue []Item

func (pq PriorityQueue) Len() int           { return len(pq) }
func (pq PriorityQueue) Less(i, j int) bool { return pq[i].Priority < pq[j].Priority }
func (pq PriorityQueue) Swap(i, j int)      { pq[i], pq[j] = pq[j], pq[i] }

func (pq *PriorityQueue) Push(x any) {
	*pq = append(*pq, x.(Item))
}

func (pq *PriorityQueue) Pop() any {
	old := *pq
	n := len(old)
	item := old[n-1]
	*pq = old[:n-1]
	return item
}

func priorityQueueExample() Item {
	pq := &PriorityQueue{}
	heap.Init(pq)
	heap.Push(pq, Item{Value: "task-1", Priority: 2})
	heap.Push(pq, Item{Value: "task-2", Priority: 1})
	next := heap.Pop(pq).(Item)
	fmt.Println(next)
	return next
}

// ---------------------------------------------------
// 📌 Бинарный поиск
// ---------------------------------------------------
func binarySearch(arr []int, target int) int {
	idx := sort.Search(len(arr), func(i int) bool { return arr[i] >= target })
	if idx < len(arr) && arr[idx] == target {
		fmt.Println("found", idx)
		return idx
	}
	fmt.Println("not found")
	return -1
}

// ---------------------------------------------------
// 📌 DFS / BFS по графу
// ---------------------------------------------------
func DFSGraph(start int, adj map[int][]int, visit func(int)) {
	visited := make(map[int]bool)
	var dfs func(int)
	dfs = func(v int) {
		if visited[v] {
			return
		}
		visited[v] = true
		visit(v)
		for _, to := range adj[v] {
			dfs(to)
		}
	}
	dfs(start)
}

func BFSGraph(start int, adj map[int][]int, visit func(int)) {
	visited := make(map[int]bool)
	queue := []int{start}
	visited[start] = true
	for len(queue) > 0 {
		v := queue[0]
		queue = queue[1:]
		visit(v)
		for _, to := range adj[v] {
			if !visited[to] {
				visited[to] = true
				queue = append(queue, to)
			}
		}
	}
}

// ---------------------------------------------------
// 📌 Связный список container/list
// ---------------------------------------------------
func listExample() {
	var lst list.List
	lst.PushBack(1)
	lst.PushFront(0)
	for e := lst.Front(); e != nil; e = e.Next() {
		fmt.Println(e.Value)
	}
}

// ---------------------------------------------------
// 📌 Кольцевой буфер
// ---------------------------------------------------
type RingBuffer[T any] struct {
	data       []T
	head, tail int
	full       bool
}

func NewRingBuffer[T any](size int) *RingBuffer[T] {
	return &RingBuffer[T]{data: make([]T, size)}
}

func (r *RingBuffer[T]) Push(v T) {
	r.data[r.tail] = v
	if r.full {
		r.head = (r.head + 1) % len(r.data)
	}
	r.tail = (r.tail + 1) % len(r.data)
	r.full = r.tail == r.head
}

func (r *RingBuffer[T]) Pop() (T, bool) {
	if !r.full && r.head == r.tail {
		var zero T
		return zero, false
	}
	v := r.data[r.head]
	r.head = (r.head + 1) % len(r.data)
	r.full = false
	return v, true
}

// ---------------------------------------------------
// 📌 Топологическая сортировка
// ---------------------------------------------------
func TopoSort(adj map[int][]int) []int {
	visited := make(map[int]bool)
	result := []int{}

	var dfs func(int)
	dfs = func(v int) {
		if visited[v] {
			return
		}
		visited[v] = true
		for _, to := range adj[v] {
			dfs(to)
		}
		result = append([]int{v}, result...) // prepend
	}

	for v := range adj {
		dfs(v)
	}
	return result
}

// ---------------------------------------------------
// 📌 Проверка на цикл в графе
// ---------------------------------------------------
func HasCycle(adj map[int][]int) bool {
	color := make(map[int]int) // 0=white, 1=gray, 2=black

	var dfs func(int) bool
	dfs = func(v int) bool {
		color[v] = 1
		for _, to := range adj[v] {
			if color[to] == 1 {
				return true // back edge
			}
			if color[to] == 0 && dfs(to) {
				return true
			}
		}
		color[v] = 2
		return false
	}

	for v := range adj {
		if color[v] == 0 && dfs(v) {
			return true
		}
	}
	return false
}
