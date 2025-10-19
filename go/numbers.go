package cheatsheets

import (
	"fmt"
	"math"
	"math/big"
	"math/rand"
	"strconv"
	"time"
)

// ---------------------------------------------------
// 📌 Базовые операции
// ---------------------------------------------------
func numbersBasic(a, b int) {
	quotient := a / b
	remainder := a % b
	floatQuotient := float64(a) / float64(b)
	pow := math.Pow(2, 10)
	fmt.Println(quotient, remainder, floatQuotient, pow)
}

// ---------------------------------------------------
// 📌 Округление и модуль
// ---------------------------------------------------
func numbersRound(x float64) {
	round := math.Round(x)
	floor := math.Floor(x)
	ceil := math.Ceil(x)
	abs := math.Abs(-x)
	min := math.Min(3.14, 2.71)
	max := math.Max(3.14, 2.71)
	fmt.Println(round, floor, ceil, abs, min, max)
}

// ---------------------------------------------------
// 📌 Ограничение значений
// ---------------------------------------------------
func numbersClamp(v, lo, hi int) int {
	switch {
	case v < lo:
		return lo
	case v > hi:
		return hi
	default:
		return v
	}
}

// ---------------------------------------------------
// 📌 Случайные числа
// ---------------------------------------------------
func numbersRandom(seed int64) {
	rand.Seed(seed)
	randInt := rand.Intn(10)
	randFloat := rand.Float64()
	dist := rand.New(rand.NewSource(time.Now().UnixNano()))
	value := dist.Int63n(1_000_000)
	fmt.Println(randInt, randFloat, value)
}

// ---------------------------------------------------
// 📌 Диапазоны чисел
// ---------------------------------------------------
func numbersRange(lo, hi int) []int {
	out := make([]int, 0, hi-lo+1)
	for i := lo; i <= hi; i++ {
		out = append(out, i)
	}
	fmt.Println(out)
	return out
}

// ---------------------------------------------------
// 📌 Работа с math/big
// ---------------------------------------------------
func numbersBig() {
	bigA := big.NewInt(0).Exp(big.NewInt(2), big.NewInt(100), nil)
	bigSum := big.NewInt(0).Add(bigA, big.NewInt(1))
	ratio := big.NewRat(22, 7)
	ratioFloat, _ := ratio.Float64()
	fmt.Println(bigA.BitLen(), bigSum.BitLen(), ratioFloat)
}

// ---------------------------------------------------
// 📌 Конвертация строк и чисел
// ---------------------------------------------------
func numbersConvert() {
	n, err := strconv.Atoi("42")
	if err != nil {
		panic(err)
	}
	value, err := strconv.ParseFloat("3.1415", 64)
	if err != nil {
		panic(err)
	}
	hex := strconv.FormatInt(int64(n), 16)
	formatted := strconv.FormatFloat(value, 'f', 2, 64)
	fmt.Println(n, value, hex, formatted)
}
