#include <bitset>
#include <iostream>
#include <cstdint>

// ---------------------------------------------------
// 📌 Основные битовые операции
// ---------------------------------------------------
void example_basic_bitwise() {
    int a = 5;  // 0101
    int b = 3;  // 0011

    int and_result = a & b;   // 0001 = 1
    int or_result = a | b;    // 0111 = 7
    int xor_result = a ^ b;   // 0110 = 6
    int not_result = ~a;      // инверсия всех бит
    int left_shift = a << 1;  // 1010 = 10
    int right_shift = a >> 1; // 0010 = 2
}

// ---------------------------------------------------
// 📌 Проверка, установка, снятие бита
// ---------------------------------------------------
bool getBit(int num, int pos) {
    return (num & (1 << pos)) != 0;
}

int setBit(int num, int pos) {
    return num | (1 << pos);
}

int clearBit(int num, int pos) {
    return num & ~(1 << pos);
}

int toggleBit(int num, int pos) {
    return num ^ (1 << pos);
}

// ---------------------------------------------------
// 📌 Подсчёт количества единичных бит (popcount)
// ---------------------------------------------------
int countBits(int num) {
    int count = 0;
    while (num) {
        count += num & 1;
        num >>= 1;
    }
    return count;
}

int countBitsBuiltin(int num) {
    return __builtin_popcount(num);
}

int countBitsBuiltin64(long long num) {
    return __builtin_popcountll(num);
}

// ---------------------------------------------------
// 📌 Проверка на степень двойки
// ---------------------------------------------------
bool isPowerOfTwo(int num) {
    return num > 0 && (num & (num - 1)) == 0;
}

// ---------------------------------------------------
// 📌 Получение младшего установленного бита
// ---------------------------------------------------
int lowestSetBit(int num) {
    return num & -num;
}

// ---------------------------------------------------
// 📌 Снятие младшего установленного бита
// ---------------------------------------------------
int clearLowestBit(int num) {
    return num & (num - 1);
}

// ---------------------------------------------------
// 📌 Битовые маски и флаги
// ---------------------------------------------------
void example_flags() {
    const uint32_t FLAG_READ = 1 << 0;    // 0001
    const uint32_t FLAG_WRITE = 1 << 1;   // 0010
    const uint32_t FLAG_EXEC = 1 << 2;    // 0100
    const uint32_t FLAG_ADMIN = 1 << 3;   // 1000

    uint32_t permissions = 0;

    // Установка флагов
    permissions |= FLAG_READ;
    permissions |= FLAG_WRITE;

    // Проверка флага
    bool canRead = (permissions & FLAG_READ) != 0;

    // Снятие флага
    permissions &= ~FLAG_WRITE;

    // Переключение флага
    permissions ^= FLAG_EXEC;
}

// ---------------------------------------------------
// 📌 Генерация всех подмножеств (битовые маски)
// ---------------------------------------------------
void generateSubsets(int n) {
    int totalSubsets = 1 << n;
    for (int mask = 0; mask < totalSubsets; ++mask) {
        std::cout << "Подмножество: ";
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                std::cout << i << " ";
            }
        }
        std::cout << std::endl;
    }
}

// ---------------------------------------------------
// 📌 Итерация по всем установленным битам
// ---------------------------------------------------
void iterateSetBits(int num) {
    while (num) {
        int lowestBit = num & -num;
        int bitPosition = __builtin_ctz(lowestBit); // count trailing zeros
        std::cout << "Бит на позиции: " << bitPosition << std::endl;
        num &= num - 1; // снимаем младший бит
    }
}

// ---------------------------------------------------
// 📌 Работа с std::bitset
// ---------------------------------------------------
void example_bitset() {
    std::bitset<8> bits("10101010");

    bits.set(0);        // установить бит 0
    bits.reset(1);      // сбросить бит 1
    bits.flip(2);       // инвертировать бит 2

    bool bit3 = bits[3];
    size_t count = bits.count();  // количество единиц
    bool all = bits.all();        // все биты = 1?
    bool any = bits.any();        // хотя бы один = 1?
    bool none = bits.none();      // все биты = 0?

    std::string str = bits.to_string();
    unsigned long num = bits.to_ulong();
}

// ---------------------------------------------------
// 📌 Swap двух чисел без временной переменной (XOR trick)
// ---------------------------------------------------
void swapXOR(int& a, int& b) {
    if (&a != &b) {  // защита от swap(x, x)
        a ^= b;
        b ^= a;
        a ^= b;
    }
}

// ---------------------------------------------------
// 📌 Поиск единственного числа (все остальные встречаются дважды)
// ---------------------------------------------------
int findSingle(const std::vector<int>& nums) {
    int result = 0;
    for (int num : nums) {
        result ^= num;
    }
    return result;
}

// ---------------------------------------------------
// 📌 Получение знакового бита
// ---------------------------------------------------
bool isNegative(int num) {
    return (num >> 31) & 1;
}

// ---------------------------------------------------
// 📌 Абсолютное значение без ветвлений
// ---------------------------------------------------
int absNoBranch(int num) {
    int mask = num >> 31;
    return (num + mask) ^ mask;
}

// ---------------------------------------------------
// 📌 Минимум/максимум без ветвлений
// ---------------------------------------------------
int minNoBranch(int a, int b) {
    return b ^ ((a ^ b) & -(a < b));
}

int maxNoBranch(int a, int b) {
    return a ^ ((a ^ b) & -(a < b));
}

// ---------------------------------------------------
// 📌 Разворот бит в числе
// ---------------------------------------------------
uint32_t reverseBits(uint32_t num) {
    uint32_t result = 0;
    for (int i = 0; i < 32; ++i) {
        result = (result << 1) | (num & 1);
        num >>= 1;
    }
    return result;
}

// ---------------------------------------------------
// 📌 Подсчёт старших нулей (leading zeros)
// ---------------------------------------------------
int countLeadingZeros(unsigned int num) {
    return __builtin_clz(num);
}

// ---------------------------------------------------
// 📌 Подсчёт младших нулей (trailing zeros)
// ---------------------------------------------------
int countTrailingZeros(unsigned int num) {
    return __builtin_ctz(num);
}

// ---------------------------------------------------
// 📌 Получение следующей степени двойки
// ---------------------------------------------------
uint32_t nextPowerOfTwo(uint32_t num) {
    if (num == 0) return 1;
    num--;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    return num + 1;
}
