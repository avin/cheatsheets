package cheatsheets

import (
	"math"
	"math/big"
)

// ---------------------------------------------------
// 📌 НОД и НОК
// ---------------------------------------------------
func gcd(a, b int) int {
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

func lcm(a, b int) int {
	return a / gcd(a, b) * b
}

// ---------------------------------------------------
// 📌 Расширенный алгоритм Евклида
// ---------------------------------------------------
func egcd(a, b int) (g, x, y int) {
	if b == 0 {
		return a, 1, 0
	}
	g, x1, y1 := egcd(b, a%b)
	x = y1
	y = x1 - (a/b)*y1
	return
}

// ---------------------------------------------------
// 📌 Проверка простоты
// ---------------------------------------------------
func isPrime(n int) bool {
	if n < 2 {
		return false
	}
	if n%2 == 0 {
		return n == 2
	}
	limit := int(math.Sqrt(float64(n)))
	for i := 3; i <= limit; i += 2 {
		if n%i == 0 {
			return false
		}
	}
	return true
}

// ---------------------------------------------------
// 📌 Решето Эратосфена
// ---------------------------------------------------
func sieve(limit int) []int {
	marked := make([]bool, limit+1)
	primes := []int{}
	for i := 2; i <= limit; i++ {
		if marked[i] {
			continue
		}
		primes = append(primes, i)
		for j := i * i; j <= limit; j += i {
			marked[j] = true
		}
	}
	return primes
}

// ---------------------------------------------------
// 📌 Быстрое возведение в степень
// ---------------------------------------------------
func modPow(base, exp, mod int) int {
	result := 1 % mod
	base %= mod
	for exp > 0 {
		if exp&1 == 1 {
			result = result * base % mod
		}
		base = base * base % mod
		exp >>= 1
	}
	return result
}

// ---------------------------------------------------
// 📌 Факториал и биномиальные коэффициенты
// ---------------------------------------------------
func factorialBig(n int) *big.Int {
	result := big.NewInt(1)
	for i := 2; i <= n; i++ {
		result.Mul(result, big.NewInt(int64(i)))
	}
	return result
}

func binomial(n, k int) *big.Int {
	if k < 0 || k > n {
		return big.NewInt(0)
	}
	if k > n-k {
		k = n - k
	}
	result := big.NewInt(1)
	for i := 1; i <= k; i++ {
		result.Mul(result, big.NewInt(int64(n-k+i)))
		result.Div(result, big.NewInt(int64(i)))
	}
	return result
}

// ---------------------------------------------------
// 📌 Префиксные суммы
// ---------------------------------------------------
func prefixSums(arr []int) []int {
	sums := make([]int, len(arr)+1)
	for i, v := range arr {
		sums[i+1] = sums[i] + v
	}
	return sums
}

// ---------------------------------------------------
// 📌 Модульная арифметика: сложение, вычитание, умножение
// ---------------------------------------------------
func modAdd(a, b, mod int) int {
	return ((a % mod) + (b % mod)) % mod
}

func modSub(a, b, mod int) int {
	return ((a % mod) - (b % mod) + mod) % mod
}

func modMul(a, b, mod int) int {
	return ((a % mod) * (b % mod)) % mod
}

// ---------------------------------------------------
// 📌 Модульное обратное (inverse modulo)
// ---------------------------------------------------
func modInverse(a, mod int) int {
	g, x, _ := egcd(a, mod)
	if g != 1 {
		return -1 // обратного не существует
	}
	return (x%mod + mod) % mod
}
