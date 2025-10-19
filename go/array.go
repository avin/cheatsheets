package cheatsheets

import (
	"fmt"
	"sort"
)

// ---------------------------------------------------
// 📌 Создание и инициализация
// ---------------------------------------------------
func arrayCreateAndInit() {
	arr := [3]int{1, 2, 3}
	slice := []int{1, 2, 3}
	empty := make([]int, 0, 16)

	matrix := make([][]int, 3)
	for i := range matrix {
		matrix[i] = make([]int, 4)
	}

	fmt.Println(arr)
	fmt.Println(slice, len(empty), cap(empty))
	fmt.Println(matrix)
}

// ---------------------------------------------------
// 📌 Итерация и индексы
// ---------------------------------------------------
func arrayIterate(slice []int, matrix [][]int) {
	for i := 0; i < len(slice); i++ {
		fmt.Println(i, slice[i])
	}
	for idx, val := range slice {
		fmt.Println(idx, val)
	}
	for _, row := range matrix {
		fmt.Println(row)
	}
}

// ---------------------------------------------------
// 📌 Копирование и клонирование
// ---------------------------------------------------
func arrayCopy(slice []int, matrix [][]int) {
	clone := append([]int(nil), slice...)
	copyTarget := make([]int, len(slice))
	copy(copyTarget, slice)

	column := make([]int, len(matrix))
	for i := range matrix {
		column[i] = matrix[i][0]
	}

	fmt.Println(clone)
	fmt.Println(copyTarget)
	fmt.Println(column)
}

// ---------------------------------------------------
// 📌 Добавление и объединение
// ---------------------------------------------------
func arrayAppend(slice []int) []int {
	slice = append(slice, 42)
	more := []int{7, 8, 9}
	slice = append(slice, more...)
	prefix := []int{1, 2}
	slice = append(prefix, slice...)
	fmt.Println(slice)
	return slice
}

// ---------------------------------------------------
// 📌 Подсрезы, длина и ёмкость
// ---------------------------------------------------
func arraySubslice(slice []int) {
	if len(slice) < 4 {
		slice = append(slice, make([]int, 4-len(slice))...)
	}
	firstTwo := slice[:2]
	mid := slice[1:4]
	last := slice[len(slice)-2:]
	grown := append(make([]int, 0, 2), slice...)
	fmt.Println(firstTwo, mid, last, len(grown), cap(grown))
}

// ---------------------------------------------------
// 📌 Фильтрация
// ---------------------------------------------------
func arrayFilter(slice []int) []int {
	filtered := slice[:0]
	for _, v := range slice {
		if v%2 == 0 {
			filtered = append(filtered, v)
		}
	}
	fmt.Println(filtered)
	return filtered
}

// ---------------------------------------------------
// 📌 Map-преобразование
// ---------------------------------------------------
func arrayMap(slice []int) []int {
	squared := make([]int, len(slice))
	for i, v := range slice {
		squared[i] = v * v
	}
	fmt.Println(squared)
	return squared
}

// ---------------------------------------------------
// 📌 Reduce / fold
// ---------------------------------------------------
func arrayReduce(slice []int) (int, int) {
	sum := 0
	product := 1
	for _, v := range slice {
		sum += v
		product *= v
	}
	fmt.Println(sum, product)
	return sum, product
}

// ---------------------------------------------------
// 📌 Сортировка и разворот
// ---------------------------------------------------
func arraySort(slice []int) []int {
	sort.Ints(slice)
	sort.Slice(slice, func(i, j int) bool { return slice[i] > slice[j] })
	for i, j := 0, len(slice)-1; i < j; i, j = i+1, j-1 {
		slice[i], slice[j] = slice[j], slice[i]
	}
	fmt.Println(slice)
	return slice
}

// ---------------------------------------------------
// 📌 Удаление и вставка
// ---------------------------------------------------
func arrayModify(slice []int, idx, value int) []int {
	if idx >= 0 && idx < len(slice) {
		slice = append(slice[:idx], slice[idx+1:]...)
	}
	if idx < 0 {
		idx = 0
	}
	if idx > len(slice) {
		idx = len(slice)
	}
	slice = append(slice, 0)
	copy(slice[idx+1:], slice[idx:])
	slice[idx] = value
	fmt.Println(slice)
	return slice
}

// ---------------------------------------------------
// 📌 Дедупликация
// ---------------------------------------------------
func arrayDedup(slice []int) []int {
	seen := make(map[int]struct{})
	out := slice[:0]
	for _, v := range slice {
		if _, ok := seen[v]; ok {
			continue
		}
		seen[v] = struct{}{}
		out = append(out, v)
	}
	fmt.Println(out)
	return out
}
