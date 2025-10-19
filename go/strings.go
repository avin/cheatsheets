package cheatsheets

import (
	"fmt"
	"regexp"
	"sort"
	"strings"
)

// ---------------------------------------------------
// 📌 Базовые операции и руны
// ---------------------------------------------------
func stringBasics() {
	s := "Привет, мир"
	lengthBytes := len(s)
	runes := []rune(s)
	firstRune := runes[0]
	substring := string(runes[2:5])
	fmt.Println(lengthBytes, firstRune, substring)
}

// ---------------------------------------------------
// 📌 Перебор строки
// ---------------------------------------------------
func stringIterate(s string) {
	for idx, r := range s {
		fmt.Printf("pos=%d rune=%c\n", idx, r)
	}
}

// ---------------------------------------------------
// 📌 Поиск и проверки
// ---------------------------------------------------
func stringSearch(s string) {
	fmt.Println(
		strings.Contains(s, "мир"),
		strings.HasPrefix(s, "При"),
		strings.HasSuffix(s, "мир"),
		strings.IndexRune(s, 'и'),
		strings.Count(s, "и"),
	)
}

// ---------------------------------------------------
// 📌 Разбиение и объединение
// ---------------------------------------------------
func stringSplitJoin() {
	parts := strings.Split("a,b,c", ",")
	fields := strings.Fields("  spaced   words  ")
	joined := strings.Join(parts, " | ")
	fmt.Println(parts, fields, joined)
}

// ---------------------------------------------------
// 📌 Построение строки
// ---------------------------------------------------
func stringBuilder() string {
	var b strings.Builder
	b.Grow(64)
	for i := 0; i < 5; i++ {
		fmt.Fprintf(&b, "item-%d ", i)
	}
	result := b.String()
	fmt.Println(result)
	return result
}

// ---------------------------------------------------
// 📌 Замены и нормализация
// ---------------------------------------------------
func stringNormalize() {
	replacer := strings.NewReplacer("-", " ", "_", " ")
	normalized := replacer.Replace("go-is_awesome")
	upper := strings.ToUpper(normalized)
	lower := strings.ToLower(normalized)
	title := strings.ToTitle(normalized)
	fmt.Println(upper, lower, title)
}

// ---------------------------------------------------
// 📌 Регулярные выражения
// ---------------------------------------------------
func stringRegex(input string) {
	re := regexp.MustCompile(`\d{4}-\d{2}-\d{2}`)
	found := re.FindString(input)
	masked := re.ReplaceAllString("id=123-45-6789", "****")
	fmt.Println(found, masked)
}

// ---------------------------------------------------
// 📌 Сортировка строк
// ---------------------------------------------------
func stringSort(words []string) {
	sort.Strings(words)
	sort.Slice(words, func(i, j int) bool {
		return len(words[i]) < len(words[j])
	})
	fmt.Println(words)
}

// ---------------------------------------------------
// 📌 Удаление пробелов (Trim)
// ---------------------------------------------------
func stringTrim(s string) {
	trimmed := strings.TrimSpace("  hello  ")
	leftTrim := strings.TrimLeft(s, " \t")
	rightTrim := strings.TrimRight(s, " \n")
	trimPrefix := strings.TrimPrefix(s, "prefix")
	trimSuffix := strings.TrimSuffix(s, "suffix")
	fmt.Println(trimmed, leftTrim, rightTrim, trimPrefix, trimSuffix)
}

// ---------------------------------------------------
// 📌 Повторение и проверка типов символов
// ---------------------------------------------------
func stringRepeatAndCheck(s string) {
	repeated := strings.Repeat("Go", 3) // "GoGoGo"
	allDigits := true
	for _, r := range s {
		if r < '0' || r > '9' {
			allDigits = false
			break
		}
	}
	fmt.Println(repeated, allDigits)
}
