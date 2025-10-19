// ---------------------------------------------------
// 📌 Создание даты
// ---------------------------------------------------
const now = new Date();
const specificDate = new Date('2024-01-15');
const fromTimestamp = new Date(1705276800000);
const fromComponents = new Date(2024, 0, 15, 10, 30, 0); // месяцы с 0!


// ---------------------------------------------------
// 📌 Получение компонентов даты
// ---------------------------------------------------
const year = now.getFullYear();
const month = now.getMonth();        // 0-11
const day = now.getDate();           // 1-31
const dayOfWeek = now.getDay();      // 0 (воскресенье) - 6 (суббота)
const hours = now.getHours();        // 0-23
const minutes = now.getMinutes();    // 0-59
const seconds = now.getSeconds();    // 0-59
const milliseconds = now.getMilliseconds(); // 0-999


// ---------------------------------------------------
// 📌 Форматирование даты
// ---------------------------------------------------
const isoString = now.toISOString();           // "2024-01-15T10:30:00.000Z"
const dateString = now.toDateString();         // "Mon Jan 15 2024"
const timeString = now.toTimeString();         // "10:30:00 GMT+0300"
const localeString = now.toLocaleString('ru-RU'); // локализованная строка
const localeDateString = now.toLocaleDateString('ru-RU'); // "15.01.2024"
const localeTimeString = now.toLocaleTimeString('ru-RU'); // "10:30:00"


// ---------------------------------------------------
// 📌 Форматирование с опциями (Intl)
// ---------------------------------------------------
const formatted = now.toLocaleDateString('ru-RU', {
  year: 'numeric',
  month: 'long',
  day: 'numeric',
  weekday: 'long'
});
// "понедельник, 15 января 2024 г."

const timeFormatted = now.toLocaleTimeString('ru-RU', {
  hour: '2-digit',
  minute: '2-digit',
  second: '2-digit',
  hour12: false
});


// ---------------------------------------------------
// 📌 Парсинг строки в дату
// ---------------------------------------------------
const parsed1 = new Date('2024-01-15');
const parsed2 = Date.parse('2024-01-15T10:30:00'); // timestamp в мс
const isValid = !isNaN(new Date('invalid').getTime());


// ---------------------------------------------------
// 📌 Timestamp (миллисекунды с 1970-01-01)
// ---------------------------------------------------
const timestamp = now.getTime();
const timestampNow = Date.now();


// ---------------------------------------------------
// 📌 Операции с датами (добавление/вычитание)
// ---------------------------------------------------
// Добавить дни
const tomorrow = new Date();
tomorrow.setDate(tomorrow.getDate() + 1);

// Добавить месяцы
const nextMonth = new Date();
nextMonth.setMonth(nextMonth.getMonth() + 1);

// Добавить годы
const nextYear = new Date();
nextYear.setFullYear(nextYear.getFullYear() + 1);

// Добавить часы
const inOneHour = new Date();
inOneHour.setHours(inOneHour.getHours() + 1);

// Начало дня
const startOfDay = new Date();
startOfDay.setHours(0, 0, 0, 0);

// Конец дня
const endOfDay = new Date();
endOfDay.setHours(23, 59, 59, 999);


// ---------------------------------------------------
// 📌 Разница между датами
// ---------------------------------------------------
const date1 = new Date('2024-01-01');
const date2 = new Date('2024-01-15');

const diffMs = date2 - date1;                    // миллисекунды
const diffSeconds = diffMs / 1000;               // секунды
const diffMinutes = diffMs / (1000 * 60);        // минуты
const diffHours = diffMs / (1000 * 60 * 60);     // часы
const diffDays = diffMs / (1000 * 60 * 60 * 24); // дни


// ---------------------------------------------------
// 📌 Сравнение дат
// ---------------------------------------------------
const isBefore = date1 < date2;
const isAfter = date1 > date2;
const isEqual = date1.getTime() === date2.getTime();


// ---------------------------------------------------
// 📌 Получение первого/последнего дня месяца
// ---------------------------------------------------
const firstDayOfMonth = new Date(now.getFullYear(), now.getMonth(), 1);
const lastDayOfMonth = new Date(now.getFullYear(), now.getMonth() + 1, 0);


// ---------------------------------------------------
// 📌 Получение количества дней в месяце
// ---------------------------------------------------
function getDaysInMonth(year, month) {
  return new Date(year, month + 1, 0).getDate();
}


// ---------------------------------------------------
// 📌 Проверка високосного года
// ---------------------------------------------------
function isLeapYear(year) {
  return (year % 4 === 0 && year % 100 !== 0) || (year % 400 === 0);
}


// ---------------------------------------------------
// 📌 Получение номера недели в году
// ---------------------------------------------------
function getWeekNumber(date) {
  const firstDayOfYear = new Date(date.getFullYear(), 0, 1);
  const pastDaysOfYear = (date - firstDayOfYear) / 86400000;
  return Math.ceil((pastDaysOfYear + firstDayOfYear.getDay() + 1) / 7);
}


// ---------------------------------------------------
// 📌 Клонирование даты
// ---------------------------------------------------
const cloned = new Date(now.getTime());
// или
const cloned2 = new Date(now);


// ---------------------------------------------------
// 📌 Форматирование в UTC
// ---------------------------------------------------
const utcYear = now.getUTCFullYear();
const utcMonth = now.getUTCMonth();
const utcDay = now.getUTCDate();
const utcHours = now.getUTCHours();


// ---------------------------------------------------
// 📌 Работа с таймзонами
// ---------------------------------------------------
const offset = now.getTimezoneOffset(); // смещение в минутах от UTC
const offsetHours = -offset / 60;

// Интернационализация с таймзоной
const withTimezone = now.toLocaleString('ru-RU', {
  timeZone: 'America/New_York',
  dateStyle: 'full',
  timeStyle: 'long'
});


// ---------------------------------------------------
// 📌 Относительное форматирование (недавно добавлено)
// ---------------------------------------------------
const rtf = new Intl.RelativeTimeFormat('ru', { numeric: 'auto' });
rtf.format(-1, 'day');    // "вчера"
rtf.format(0, 'day');     // "сегодня"
rtf.format(1, 'day');     // "завтра"
rtf.format(2, 'day');     // "через 2 дня"
rtf.format(-2, 'week');   // "2 недели назад"


// ---------------------------------------------------
// 📌 Вычисление возраста
// ---------------------------------------------------
function calculateAge(birthDate) {
  const today = new Date();
  let age = today.getFullYear() - birthDate.getFullYear();
  const monthDiff = today.getMonth() - birthDate.getMonth();

  if (monthDiff < 0 || (monthDiff === 0 && today.getDate() < birthDate.getDate())) {
    age--;
  }

  return age;
}


// ---------------------------------------------------
// 📌 Проверка, что дата валидна
// ---------------------------------------------------
function isValidDate(date) {
  return date instanceof Date && !isNaN(date.getTime());
}


// ---------------------------------------------------
// 📌 Форматирование даты вручную (кастомное)
// ---------------------------------------------------
function formatDate(date, format) {
  const pad = (num) => String(num).padStart(2, '0');

  return format
    .replace('YYYY', date.getFullYear())
    .replace('MM', pad(date.getMonth() + 1))
    .replace('DD', pad(date.getDate()))
    .replace('HH', pad(date.getHours()))
    .replace('mm', pad(date.getMinutes()))
    .replace('ss', pad(date.getSeconds()));
}

// Пример: formatDate(new Date(), 'YYYY-MM-DD HH:mm:ss')


// ---------------------------------------------------
// 📌 Добавление рабочих дней (пропуская выходные)
// ---------------------------------------------------
function addBusinessDays(date, days) {
  const result = new Date(date);
  let addedDays = 0;

  while (addedDays < days) {
    result.setDate(result.getDate() + 1);
    const dayOfWeek = result.getDay();
    if (dayOfWeek !== 0 && dayOfWeek !== 6) { // не воскресенье и не суббота
      addedDays++;
    }
  }

  return result;
}


// ---------------------------------------------------
// 📌 Интервал времени (начало и конец)
// ---------------------------------------------------
function getTimeRange(date, type) {
  const start = new Date(date);
  const end = new Date(date);

  switch (type) {
    case 'day':
      start.setHours(0, 0, 0, 0);
      end.setHours(23, 59, 59, 999);
      break;
    case 'week':
      const day = start.getDay();
      const diff = start.getDate() - day + (day === 0 ? -6 : 1);
      start.setDate(diff);
      start.setHours(0, 0, 0, 0);
      end.setDate(diff + 6);
      end.setHours(23, 59, 59, 999);
      break;
    case 'month':
      start.setDate(1);
      start.setHours(0, 0, 0, 0);
      end.setMonth(end.getMonth() + 1, 0);
      end.setHours(23, 59, 59, 999);
      break;
  }

  return {start, end};
}


// ---------------------------------------------------
// 📌 Получение квартала
// ---------------------------------------------------
function getQuarter(date) {
  return Math.floor(date.getMonth() / 3) + 1;
}
