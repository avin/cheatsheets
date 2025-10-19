package cheatsheets

import (
	"fmt"
	"sort"
)

// ---------------------------------------------------
// 📌 Создание и доступ
// ---------------------------------------------------
func mapCreate() {
	scores := map[string]int{"alice": 10, "bob": 7}
	empty := make(map[int]string)
	value := scores["alice"]
	missing, ok := scores["carol"]
	fmt.Println(scores, empty, value, missing, ok)
}

// ---------------------------------------------------
// 📌 Обновление и удаление
// ---------------------------------------------------
func mapUpdate() {
	scores := map[string]int{}
	scores["bob"] = 8
	scores["dave"] = 5
	delete(scores, "dave")
	fmt.Println(scores)
}

// ---------------------------------------------------
// 📌 Итерация по map
// ---------------------------------------------------
func mapIterate(scores map[string]int) []string {
	for name, score := range scores {
		fmt.Printf("%s => %d\n", name, score)
	}

	keys := make([]string, 0, len(scores))
	for name := range scores {
		keys = append(keys, name)
	}
	fmt.Println(keys)
	return keys
}

// ---------------------------------------------------
// 📌 Сортировка по ключам и значениям
// ---------------------------------------------------
func mapSort(scores map[string]int) {
	keys := make([]string, 0, len(scores))
	for name := range scores {
		keys = append(keys, name)
	}
	sort.Strings(keys)
	for _, name := range keys {
		fmt.Println(name, scores[name])
	}

	type pair struct {
		Name  string
		Score int
	}
	pairs := make([]pair, 0, len(scores))
	for name, score := range scores {
		pairs = append(pairs, pair{Name: name, Score: score})
	}
	sort.Slice(pairs, func(i, j int) bool {
		return pairs[i].Score > pairs[j].Score
	})
	fmt.Println(pairs)
}

// ---------------------------------------------------
// 📌 Счётчик частот
// ---------------------------------------------------
func mapFreq(text string) map[rune]int {
	freq := make(map[rune]int)
	for _, r := range text {
		freq[r]++
	}
	fmt.Println(freq)
	return freq
}

// ---------------------------------------------------
// 📌 Множество через map[T]struct{}
// ---------------------------------------------------
func setUsingMap(values []int) map[int]struct{} {
	set := make(map[int]struct{})
	for _, v := range values {
		set[v] = struct{}{}
	}
	fmt.Println(set)
	return set
}

// ---------------------------------------------------
// 📌 Пересечение и разность
// ---------------------------------------------------
func setIntersectDiff(a, b map[int]struct{}) (map[int]struct{}, map[int]struct{}) {
	intersect := make(map[int]struct{})
	for v := range a {
		if _, ok := b[v]; ok {
			intersect[v] = struct{}{}
		}
	}

	diff := make(map[int]struct{})
	for v := range a {
		if _, ok := b[v]; !ok {
			diff[v] = struct{}{}
		}
	}
	fmt.Println(intersect, diff)
	return intersect, diff
}
