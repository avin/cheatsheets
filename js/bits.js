// ---------------------------------------------------
// 📌 Основные битовые операции
// ---------------------------------------------------
const a = 5;  // 0101
const b = 3;  // 0011

const andResult = a & b;   // 0001 = 1
const orResult = a | b;    // 0111 = 7
const xorResult = a ^ b;   // 0110 = 6
const notResult = ~a;      // инверсия всех бит
const leftShift = a << 1;  // 1010 = 10
const rightShift = a >> 1; // 0010 = 2
const zeroFillRightShift = a >>> 1; // беззнаковый сдвиг вправо


// ---------------------------------------------------
// 📌 Проверка, установка, снятие бита
// ---------------------------------------------------
function getBit(num, pos) {
  return (num & (1 << pos)) !== 0;
}

function setBit(num, pos) {
  return num | (1 << pos);
}

function clearBit(num, pos) {
  return num & ~(1 << pos);
}

function toggleBit(num, pos) {
  return num ^ (1 << pos);
}


// ---------------------------------------------------
// 📌 Подсчёт количества единичных бит (popcount)
// ---------------------------------------------------
function countBits(num) {
  let count = 0;
  while (num) {
    count += num & 1;
    num >>>= 1;
  }
  return count;
}

// Более быстрый алгоритм (Brian Kernighan)
function countBitsFast(num) {
  let count = 0;
  while (num) {
    num &= num - 1; // снимаем младший установленный бит
    count++;
  }
  return count;
}


// ---------------------------------------------------
// 📌 Проверка на степень двойки
// ---------------------------------------------------
function isPowerOfTwo(num) {
  return num > 0 && (num & (num - 1)) === 0;
}


// ---------------------------------------------------
// 📌 Получение младшего установленного бита
// ---------------------------------------------------
function lowestSetBit(num) {
  return num & -num;
}


// ---------------------------------------------------
// 📌 Снятие младшего установленного бита
// ---------------------------------------------------
function clearLowestBit(num) {
  return num & (num - 1);
}


// ---------------------------------------------------
// 📌 Битовые маски и флаги
// ---------------------------------------------------
const FLAG_READ = 1 << 0;    // 0001
const FLAG_WRITE = 1 << 1;   // 0010
const FLAG_EXEC = 1 << 2;    // 0100
const FLAG_ADMIN = 1 << 3;   // 1000

let permissions = 0;

// Установка флагов
permissions |= FLAG_READ;
permissions |= FLAG_WRITE;

// Проверка флага
const canRead = (permissions & FLAG_READ) !== 0;

// Снятие флага
permissions &= ~FLAG_WRITE;

// Переключение флага
permissions ^= FLAG_EXEC;


// ---------------------------------------------------
// 📌 Генерация всех подмножеств (битовые маски)
// ---------------------------------------------------
function generateSubsets(n) {
  const totalSubsets = 1 << n;
  const result = [];

  for (let mask = 0; mask < totalSubsets; mask++) {
    const subset = [];
    for (let i = 0; i < n; i++) {
      if (mask & (1 << i)) {
        subset.push(i);
      }
    }
    result.push(subset);
  }

  return result;
}


// ---------------------------------------------------
// 📌 Итерация по всем установленным битам
// ---------------------------------------------------
function iterateSetBits(num) {
  const positions = [];
  let pos = 0;

  while (num) {
    if (num & 1) {
      positions.push(pos);
    }
    num >>>= 1;
    pos++;
  }

  return positions;
}


// ---------------------------------------------------
// 📌 Swap двух чисел без временной переменной (XOR trick)
// ---------------------------------------------------
function swapXOR(a, b) {
  a ^= b;
  b ^= a;
  a ^= b;
  return [a, b];
}


// ---------------------------------------------------
// 📌 Поиск единственного числа (все остальные встречаются дважды)
// ---------------------------------------------------
function findSingle(nums) {
  let result = 0;
  for (const num of nums) {
    result ^= num;
  }
  return result;
}


// ---------------------------------------------------
// 📌 Получение знакового бита
// ---------------------------------------------------
function isNegative(num) {
  return (num >> 31) & 1;
}


// ---------------------------------------------------
// 📌 Абсолютное значение без ветвлений
// ---------------------------------------------------
function absNoBranch(num) {
  const mask = num >> 31;
  return (num + mask) ^ mask;
}


// ---------------------------------------------------
// 📌 Минимум/максимум без ветвлений
// ---------------------------------------------------
function minNoBranch(a, b) {
  return b ^ ((a ^ b) & -(a < b));
}

function maxNoBranch(a, b) {
  return a ^ ((a ^ b) & -(a < b));
}


// ---------------------------------------------------
// 📌 Разворот бит в числе (32-bit)
// ---------------------------------------------------
function reverseBits(num) {
  let result = 0;
  for (let i = 0; i < 32; i++) {
    result = (result << 1) | (num & 1);
    num >>>= 1;
  }
  return result >>> 0; // беззнаковое 32-битное число
}


// ---------------------------------------------------
// 📌 Подсчёт младших нулей (trailing zeros)
// ---------------------------------------------------
function countTrailingZeros(num) {
  if (num === 0) return 32;
  let count = 0;
  while ((num & 1) === 0) {
    num >>>= 1;
    count++;
  }
  return count;
}

// Более быстрый вариант
function countTrailingZerosFast(num) {
  return Math.clz32((num & -num) - 1) - Math.clz32(num);
}


// ---------------------------------------------------
// 📌 Подсчёт старших нулей (leading zeros)
// ---------------------------------------------------
function countLeadingZeros(num) {
  return Math.clz32(num);
}


// ---------------------------------------------------
// 📌 Получение следующей степени двойки
// ---------------------------------------------------
function nextPowerOfTwo(num) {
  if (num === 0) return 1;
  num--;
  num |= num >>> 1;
  num |= num >>> 2;
  num |= num >>> 4;
  num |= num >>> 8;
  num |= num >>> 16;
  return num + 1;
}


// ---------------------------------------------------
// 📌 Длина числа в битах (floor(log2(n)) + 1)
// ---------------------------------------------------
function bitLength(num) {
  if (num === 0) return 0;
  return 32 - Math.clz32(num);
}


// ---------------------------------------------------
// 📌 Маскирование младших N бит
// ---------------------------------------------------
function maskLowerNBits(n) {
  return (1 << n) - 1;
}


// ---------------------------------------------------
// 📌 Извлечение диапазона бит
// ---------------------------------------------------
function extractBits(num, start, length) {
  const mask = (1 << length) - 1;
  return (num >>> start) & mask;
}


// ---------------------------------------------------
// 📌 Проверка чётности (чётное число единиц)
// ---------------------------------------------------
function hasEvenParity(num) {
  let parity = 0;
  while (num) {
    parity ^= 1;
    num &= num - 1;
  }
  return parity === 0;
}


// ---------------------------------------------------
// 📌 Работа с BigInt (для больших чисел)
// ---------------------------------------------------
const bigA = 5n;
const bigB = 3n;

const bigAnd = bigA & bigB;
const bigOr = bigA | bigB;
const bigXor = bigA ^ bigB;
const bigNot = ~bigA;
const bigLeftShift = bigA << 1n;
const bigRightShift = bigA >> 1n;


// ---------------------------------------------------
// 📌 Быстрое умножение/деление на степень двойки
// ---------------------------------------------------
function multiplyByPowerOf2(num, power) {
  return num << power;
}

function divideByPowerOf2(num, power) {
  return num >> power;
}


// ---------------------------------------------------
// 📌 Проверка, установлен ли N-й бит
// ---------------------------------------------------
function isBitSet(num, n) {
  return (num & (1 << n)) !== 0;
}


// ---------------------------------------------------
// 📌 Установка диапазона бит
// ---------------------------------------------------
function setBitRange(num, start, end) {
  const mask = ((1 << (end - start + 1)) - 1) << start;
  return num | mask;
}


// ---------------------------------------------------
// 📌 Сброс диапазона бит
// ---------------------------------------------------
function clearBitRange(num, start, end) {
  const mask = ((1 << (end - start + 1)) - 1) << start;
  return num & ~mask;
}
