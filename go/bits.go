package cheatsheets

import (
	"fmt"
	"math/bits"
)

// ---------------------------------------------------
// 📌 Основные битовые операции
// ---------------------------------------------------
func bitBasicOps() {
	a, b := 5, 3 // 0101, 0011

	andResult := a & b   // 0001 = 1
	orResult := a | b    // 0111 = 7
	xorResult := a ^ b   // 0110 = 6
	notResult := ^a      // инверсия всех бит
	leftShift := a << 1  // 1010 = 10
	rightShift := a >> 1 // 0010 = 2

	fmt.Println(andResult, orResult, xorResult, notResult, leftShift, rightShift)
}

// ---------------------------------------------------
// 📌 Проверка, установка, снятие бита
// ---------------------------------------------------
func getBit(num, pos int) bool {
	return (num & (1 << pos)) != 0
}

func setBit(num, pos int) int {
	return num | (1 << pos)
}

func clearBit(num, pos int) int {
	return num &^ (1 << pos)
}

func toggleBit(num, pos int) int {
	return num ^ (1 << pos)
}

// ---------------------------------------------------
// 📌 Подсчёт количества единичных бит (popcount)
// ---------------------------------------------------
func countBits(num int) int {
	count := 0
	for num > 0 {
		count += num & 1
		num >>= 1
	}
	return count
}

func countBitsBuiltin(num uint) int {
	return bits.OnesCount(num)
}

func countBitsBuiltin64(num uint64) int {
	return bits.OnesCount64(num)
}

// ---------------------------------------------------
// 📌 Проверка на степень двойки
// ---------------------------------------------------
func isPowerOfTwo(num int) bool {
	return num > 0 && (num&(num-1)) == 0
}

// ---------------------------------------------------
// 📌 Получение младшего установленного бита
// ---------------------------------------------------
func lowestSetBit(num int) int {
	return num & -num
}

// ---------------------------------------------------
// 📌 Снятие младшего установленного бита
// ---------------------------------------------------
func clearLowestBit(num int) int {
	return num & (num - 1)
}

// ---------------------------------------------------
// 📌 Битовые маски и флаги
// ---------------------------------------------------
func bitFlags() {
	const (
		FlagRead  = 1 << 0 // 0001
		FlagWrite = 1 << 1 // 0010
		FlagExec  = 1 << 2 // 0100
		FlagAdmin = 1 << 3 // 1000
	)

	var permissions int

	// Установка флагов
	permissions |= FlagRead
	permissions |= FlagWrite

	// Проверка флага
	canRead := (permissions & FlagRead) != 0

	// Снятие флага
	permissions &^= FlagWrite

	// Переключение флага
	permissions ^= FlagExec

	fmt.Println(canRead, permissions)
}

// ---------------------------------------------------
// 📌 Генерация всех подмножеств (битовые маски)
// ---------------------------------------------------
func generateSubsets(n int) {
	totalSubsets := 1 << n
	for mask := 0; mask < totalSubsets; mask++ {
		fmt.Print("Подмножество: ")
		for i := 0; i < n; i++ {
			if mask&(1<<i) != 0 {
				fmt.Print(i, " ")
			}
		}
		fmt.Println()
	}
}

// ---------------------------------------------------
// 📌 Итерация по всем установленным битам
// ---------------------------------------------------
func iterateSetBits(num int) {
	for num > 0 {
		lowestBit := num & -num
		bitPosition := bits.TrailingZeros(uint(lowestBit))
		fmt.Println("Бит на позиции:", bitPosition)
		num &= num - 1 // снимаем младший бит
	}
}

// ---------------------------------------------------
// 📌 Swap двух чисел без временной переменной (XOR trick)
// ---------------------------------------------------
func swapXOR(a, b int) (int, int) {
	a ^= b
	b ^= a
	a ^= b
	return a, b
}

// ---------------------------------------------------
// 📌 Поиск единственного числа (все остальные встречаются дважды)
// ---------------------------------------------------
func findSingle(nums []int) int {
	result := 0
	for _, num := range nums {
		result ^= num
	}
	return result
}

// ---------------------------------------------------
// 📌 Получение знакового бита
// ---------------------------------------------------
func isNegative(num int) bool {
	return num < 0
}

// ---------------------------------------------------
// 📌 Разворот бит в числе (32-bit)
// ---------------------------------------------------
func reverseBits(num uint32) uint32 {
	return bits.Reverse32(num)
}

func reverseBits64(num uint64) uint64 {
	return bits.Reverse64(num)
}

// ---------------------------------------------------
// 📌 Подсчёт старших нулей (leading zeros)
// ---------------------------------------------------
func countLeadingZeros(num uint) int {
	return bits.LeadingZeros(num)
}

func countLeadingZeros64(num uint64) int {
	return bits.LeadingZeros64(num)
}

// ---------------------------------------------------
// 📌 Подсчёт младших нулей (trailing zeros)
// ---------------------------------------------------
func countTrailingZeros(num uint) int {
	return bits.TrailingZeros(num)
}

func countTrailingZeros64(num uint64) int {
	return bits.TrailingZeros64(num)
}

// ---------------------------------------------------
// 📌 Получение следующей степени двойки
// ---------------------------------------------------
func nextPowerOfTwo(num uint32) uint32 {
	if num == 0 {
		return 1
	}
	num--
	num |= num >> 1
	num |= num >> 2
	num |= num >> 4
	num |= num >> 8
	num |= num >> 16
	return num + 1
}

// ---------------------------------------------------
// 📌 Длина числа в битах (floor(log2(n)) + 1)
// ---------------------------------------------------
func bitLength(num uint) int {
	return bits.Len(num)
}

func bitLength64(num uint64) int {
	return bits.Len64(num)
}

// ---------------------------------------------------
// 📌 Rotate left/right
// ---------------------------------------------------
func rotateLeft(num uint, shift int) uint {
	return bits.RotateLeft(num, shift)
}

func rotateLeft64(num uint64, shift int) uint64 {
	return bits.RotateLeft64(num, shift)
}

// ---------------------------------------------------
// 📌 Проверка, что только один бит установлен
// ---------------------------------------------------
func hasOnlyOneBitSet(num int) bool {
	return num > 0 && (num&(num-1)) == 0
}

// ---------------------------------------------------
// 📌 Маскирование младших N бит
// ---------------------------------------------------
func maskLowerNBits(n int) int {
	return (1 << n) - 1
}

// ---------------------------------------------------
// 📌 Извлечение диапазона бит
// ---------------------------------------------------
func extractBits(num, start, length int) int {
	mask := (1 << length) - 1
	return (num >> start) & mask
}
