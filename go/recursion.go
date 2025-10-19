package cheatsheets

// ---------------------------------------------------
// 📌 Базовая рекурсия
// ---------------------------------------------------
func factorial(n int) int {
	if n <= 1 {
		return 1
	}
	return n * factorial(n-1)
}

// ---------------------------------------------------
// 📌 Дерево рекурсии
// ---------------------------------------------------
func fib(n int) int {
	if n < 2 {
		return n
	}
	return fib(n-1) + fib(n-2)
}

// ---------------------------------------------------
// 📌 Хвостовая рекурсия → цикл
// ---------------------------------------------------
func gcdIter(a, b int) int {
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

// ---------------------------------------------------
// 📌 Рекурсия с аккумулятором
// ---------------------------------------------------
func sumPrefix(arr []int, idx, acc int) int {
	if idx == len(arr) {
		return acc
	}
	return sumPrefix(arr, idx+1, acc+arr[idx])
}

// ---------------------------------------------------
// 📌 Имитация рекурсии через стек
// ---------------------------------------------------
type TreeNode struct {
	Value    int
	Children []*TreeNode
}

type dfsFrame struct {
	Node *TreeNode
}

func dfsIter(root *TreeNode, visit func(*TreeNode)) {
	if root == nil {
		return
	}
	stack := []dfsFrame{{Node: root}}
	for len(stack) > 0 {
		top := stack[len(stack)-1]
		stack = stack[:len(stack)-1]
		visit(top.Node)
		for i := len(top.Node.Children) - 1; i >= 0; i-- {
			stack = append(stack, dfsFrame{Node: top.Node.Children[i]})
		}
	}
}

// ---------------------------------------------------
// 📌 Мемоизация
// ---------------------------------------------------
func fibMemo(n int, memo map[int]int) int {
	if n < 2 {
		return n
	}
	if val, ok := memo[n]; ok {
		return val
	}
	memo[n] = fibMemo(n-1, memo) + fibMemo(n-2, memo)
	return memo[n]
}
