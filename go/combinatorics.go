package cheatsheets

// ---------------------------------------------------
// 📌 Подмножества через битовые маски
// ---------------------------------------------------
func subsets(nums []int) [][]int {
	out := make([][]int, 0, 1<<len(nums))
	for mask := 0; mask < (1 << len(nums)); mask++ {
		subset := make([]int, 0, len(nums))
		for i, v := range nums {
			if mask&(1<<i) != 0 {
				subset = append(subset, v)
			}
		}
		out = append(out, subset)
	}
	return out
}

// ---------------------------------------------------
// 📌 Рекурсивные подмножества
// ---------------------------------------------------
func subsetsRec(nums []int) [][]int {
	var out [][]int
	var backtrack func(int, []int)
	backtrack = func(idx int, cur []int) {
		clone := append([]int(nil), cur...)
		out = append(out, clone)
		for i := idx; i < len(nums); i++ {
			cur = append(cur, nums[i])
			backtrack(i+1, cur)
			cur = cur[:len(cur)-1]
		}
	}
	backtrack(0, nil)
	return out
}

// ---------------------------------------------------
// 📌 Перестановки
// ---------------------------------------------------
func permutations(nums []int) [][]int {
	out := [][]int{}
	var permute func(int)
	permute = func(i int) {
		if i == len(nums) {
			out = append(out, append([]int(nil), nums...))
			return
		}
		for j := i; j < len(nums); j++ {
			nums[i], nums[j] = nums[j], nums[i]
			permute(i + 1)
			nums[i], nums[j] = nums[j], nums[i]
		}
	}
	permute(0)
	return out
}

// ---------------------------------------------------
// 📌 Комбинации C(n, k)
// ---------------------------------------------------
func combinations(nums []int, k int) [][]int {
	var out [][]int
	var backtrack func(int, []int)
	backtrack = func(start int, cur []int) {
		if len(cur) == k {
			out = append(out, append([]int(nil), cur...))
			return
		}
		for i := start; i < len(nums); i++ {
			cur = append(cur, nums[i])
			backtrack(i+1, cur)
			cur = cur[:len(cur)-1]
		}
	}
	backtrack(0, nil)
	return out
}

// ---------------------------------------------------
// 📌 Размещения с повторениями
// ---------------------------------------------------
func kTuples(values []int, k int) [][]int {
	var out [][]int
	cur := make([]int, k)
	var dfs func(int)
	dfs = func(pos int) {
		if pos == k {
			out = append(out, append([]int(nil), cur...))
			return
		}
		for _, v := range values {
			cur[pos] = v
			dfs(pos + 1)
		}
	}
	dfs(0)
	return out
}

// ---------------------------------------------------
// 📌 Генерация пар
// ---------------------------------------------------
func allPairs(nums []int) [][2]int {
	out := make([][2]int, 0)
	for i := 0; i < len(nums); i++ {
		for j := i + 1; j < len(nums); j++ {
			out = append(out, [2]int{nums[i], nums[j]})
		}
	}
	return out
}
