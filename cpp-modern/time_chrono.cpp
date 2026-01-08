/*
 * ============================================
 * ВРЕМЯ И КАЛЕНДАРИ В C++
 * ============================================
 * 
 * Полное руководство по std::chrono:
 * - Duration и time points
 * - Clocks (system, steady, high_resolution)
 * - Calendar (C++20): даты, месяцы, годы
 * - Time zones (C++20)
 * - Форматирование и парсинг
 * 
 * Компиляция: g++ -std=c++20 time_chrono.cpp
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>

#ifdef __cpp_lib_format
#include <format>
#endif

using namespace std::chrono;
using namespace std::chrono_literals;

// ====================================================================================================
// 📌 std::chrono BASICS
// ====================================================================================================

// ──────────────────────────────────────────
// Duration - промежуток времени
// ──────────────────────────────────────────

seconds sec(60);
milliseconds ms(1500);

std::cout << sec.count() << " seconds\n";
std::cout << ms.count() << " milliseconds\n";

// Арифметика
auto total = sec + seconds(30);  // 90 секунд
std::cout << "Total: " << total.count() << " seconds\n";

// Конвертация
auto minutes = duration_cast<std::chrono::minutes>(total);
std::cout << "In minutes: " << minutes.count() << '\n';

// ──────────────────────────────────────────
// Time Point - момент времени
// ──────────────────────────────────────────

auto now = system_clock::now();

// Арифметика с duration
auto future = now + hours(24);
auto past = now - minutes(30);

// Разница между time points
auto diff = future - now;
std::cout << "Difference: " << duration_cast<hours>(diff).count() << " hours\n";

// ──────────────────────────────────────────
// Clocks - типы часов
// ──────────────────────────────────────────

// system_clock - реальное время (может меняться)
auto sys_now = system_clock::now();
time_t tt = system_clock::to_time_t(sys_now);
std::cout << "System time: " << std::ctime(&tt);

// steady_clock - монотонные часы (для измерений)
auto steady_start = steady_clock::now();
std::this_thread::sleep_for(100ms);
auto steady_end = steady_clock::now();
auto elapsed = steady_end - steady_start;
std::cout << "Elapsed: " << duration_cast<milliseconds>(elapsed).count() << " ms\n";

// high_resolution_clock - наивысшее разрешение
auto hr_now = high_resolution_clock::now();

// ====================================================================================================
// 📌 DURATION TYPES
// ====================================================================================================

// ──────────────────────────────────────────
// Стандартные типы duration
// ──────────────────────────────────────────

hours h(2);
minutes m(30);
seconds s(45);
milliseconds ms(500);
microseconds us(1000);
nanoseconds ns(500);

std::cout << h.count() << " hours\n";
std::cout << m.count() << " minutes\n";
std::cout << s.count() << " seconds\n";
std::cout << ms.count() << " milliseconds\n";

// C++20: days, weeks, months, years
days d(7);
weeks w(2);
months mon(3);
years y(1);

std::cout << d.count() << " days\n";
std::cout << w.count() << " weeks\n";

// ──────────────────────────────────────────
// Duration Literals (C++14)
// ──────────────────────────────────────────

using namespace std::chrono_literals;

auto hour = 1h;
auto minute = 30min;
auto second = 45s;
auto milli = 500ms;
auto micro = 1000us;
auto nano = 500ns;

// Арифметика
auto total_time = 2h + 30min + 15s;
std::cout << "Total: " << duration_cast<minutes>(total_time).count() << " minutes\n";

// ──────────────────────────────────────────
// Custom Duration Types
// ──────────────────────────────────────────

// Custom duration - например, сотни миллисекунд (centiseconds)
using centiseconds = duration<long long, std::centi>;

centiseconds cs(500);  // 500 сотых секунды = 5 секунд
auto sec = duration_cast<seconds>(cs);
std::cout << "Centiseconds to seconds: " << sec.count() << '\n';

// ──────────────────────────────────────────
// Duration Conversions
// ──────────────────────────────────────────

auto h_conv = 2h;
auto m_conv = duration_cast<minutes>(h_conv);
auto s_conv = duration_cast<seconds>(h_conv);
auto ms_conv = duration_cast<milliseconds>(h_conv);

std::cout << h_conv.count() << " hours = \n";
std::cout << "  " << m_conv.count() << " minutes\n";
std::cout << "  " << s_conv.count() << " seconds\n";
std::cout << "  " << ms_conv.count() << " milliseconds\n";

// ====================================================================================================
// 📌 CALENDAR (C++20)
// ====================================================================================================

// ──────────────────────────────────────────
// Создание и работа с датами
// ──────────────────────────────────────────

year y{2024};
month m{January};
day d{15};

// year_month_day
year_month_day ymd{y, m, d};
std::cout << "Date: " << ymd << '\n';

// Альтернативный синтаксис
auto ymd2 = 2024y / January / 15d;
std::cout << "Date: " << ymd2 << '\n';

// Текущая дата
auto today = year_month_day{floor<days>(system_clock::now())};
std::cout << "Today: " << today << '\n';

// ──────────────────────────────────────────
// Weekdays - дни недели
// ──────────────────────────────────────────

weekday wd{Sunday};
std::cout << "Weekday: " << wd << '\n';

// Найти день недели для даты
auto ymd_wd = 2024y / January / 15d;
weekday date_wd{sys_days{ymd_wd}};
std::cout << "2024-01-15 is " << date_wd << '\n';

// Следующий понедельник
auto next_monday = sys_days{ymd_wd} + (Monday - date_wd);
std::cout << "Next Monday: " << year_month_day{next_monday} << '\n';

// ──────────────────────────────────────────
// Date Arithmetic - арифметика дат
// ──────────────────────────────────────────

auto date = 2024y / January / 15d;

// Добавить дни
auto tomorrow = sys_days{date} + days{1};
std::cout << "Tomorrow: " << year_month_day{tomorrow} << '\n';

// Добавить месяцы
auto next_month = date + months{1};
std::cout << "Next month: " << next_month << '\n';

// Добавить годы
auto next_year = date + years{1};
std::cout << "Next year: " << next_year << '\n';

// Последний день месяца
auto last = 2024y / February / last;
std::cout << "Last day of Feb 2024: " << last << '\n';

// ──────────────────────────────────────────
// Date Validation - валидация дат
// ──────────────────────────────────────────

// Валидная дата
auto valid = 2024y / January / 31d;
std::cout << "2024-01-31 ok: " << valid.ok() << '\n';

// Невалидная дата
auto invalid = 2024y / February / 30d;
std::cout << "2024-02-30 ok: " << invalid.ok() << '\n';

// Високосный год
std::cout << "2024 is leap: " << year{2024}.is_leap() << '\n';
std::cout << "2023 is leap: " << year{2023}.is_leap() << '\n';

// ====================================================================================================
// 📌 TIME ZONES (C++20)
// ====================================================================================================

// ──────────────────────────────────────────
// Работа с временными зонами
// ──────────────────────────────────────────

#ifdef __cpp_lib_chrono_zone
// Текущая временная зона
auto current_tz = current_zone();
std::cout << "Current zone: " << current_tz->name() << '\n';

// Конкретная зона
auto ny_tz = locate_zone("America/New_York");
auto tokyo_tz = locate_zone("Asia/Tokyo");

// zoned_time - время в конкретной зоне
auto now_tz = system_clock::now();
zoned_time ny_time{ny_tz, now_tz};
zoned_time tokyo_time{tokyo_tz, now_tz};

std::cout << "NY time: " << ny_time << '\n';
std::cout << "Tokyo time: " << tokyo_time << '\n';
#else
std::cout << "Time zone support not available\n";
#endif

// ====================================================================================================
// 📌 FORMATTING
// ====================================================================================================

// ──────────────────────────────────────────
// Форматирование времени и дат
// ──────────────────────────────────────────

auto now_fmt = system_clock::now();
auto today_fmt = year_month_day{floor<days>(now_fmt)};

#ifdef __cpp_lib_format
// std::format для chrono (C++20)
std::cout << std::format("Date: {}\n", today_fmt);
std::cout << std::format("Time: {:%H:%M:%S}\n", now_fmt);
std::cout << std::format("ISO: {:%F %T}\n", now_fmt);
#else
// Fallback на старый способ
time_t tt_fmt = system_clock::to_time_t(now_fmt);
std::cout << "Date: " << std::put_time(std::localtime(&tt_fmt), "%Y-%m-%d") << '\n';
std::cout << "Time: " << std::put_time(std::localtime(&tt_fmt), "%H:%M:%S") << '\n';
#endif

// ====================================================================================================
// 📌 PRACTICAL EXAMPLES - Практические примеры
// ====================================================================================================

// ──────────────────────────────────────────
// Текущая дата и время
// ──────────────────────────────────────────

auto now_curr = system_clock::now();
time_t tt_curr = system_clock::to_time_t(now_curr);

std::cout << "Current time: " << std::ctime(&tt_curr);

auto today_curr = year_month_day{floor<days>(now_curr)};
std::cout << "Today: " << today_curr << '\n';

// ──────────────────────────────────────────
// Измерение времени выполнения
// ──────────────────────────────────────────

template<typename Func>
auto measure_time(Func f) {
    auto start = steady_clock::now();
    f();
    auto end = steady_clock::now();
    return duration_cast<milliseconds>(end - start);
}

auto elapsed_ex = measure_time([]() {
    std::this_thread::sleep_for(100ms);
});

std::cout << "Operation took: " << elapsed_ex.count() << " ms\n";

// ──────────────────────────────────────────
// Timer - простой класс таймера
// ──────────────────────────────────────────

class Timer {
    time_point<steady_clock> start_;
    
public:
    Timer() : start_(steady_clock::now()) {}
    
    void reset() {
        start_ = steady_clock::now();
    }
    
    auto elapsed() const {
        return duration_cast<milliseconds>(steady_clock::now() - start_);
    }
    
    void print_elapsed() const {
        std::cout << "Elapsed: " << elapsed().count() << " ms\n";
    }
};

// Использование Timer
Timer timer_ex;

std::this_thread::sleep_for(50ms);
timer_ex.print_elapsed();

std::this_thread::sleep_for(50ms);
timer_ex.print_elapsed();

timer_ex.reset();
std::this_thread::sleep_for(30ms);
timer_ex.print_elapsed();

// ──────────────────────────────────────────
// Deadline и timeout
// ──────────────────────────────────────────

auto deadline_ex = steady_clock::now() + 200ms;

while (steady_clock::now() < deadline_ex) {
    // Работа...
    std::this_thread::sleep_for(50ms);
    std::cout << "Working...\n";
}

std::cout << "Deadline reached!\n";

// ──────────────────────────────────────────
// Вычисление возраста
// ──────────────────────────────────────────

int calculate_age(year_month_day birth_date) {
    auto today_age = year_month_day{floor<days>(system_clock::now())};
    
    int age = static_cast<int>(today_age.year()) - static_cast<int>(birth_date.year());
    
    // Корректировка если день рождения еще не наступил
    if (today_age.month() < birth_date.month() ||
        (today_age.month() == birth_date.month() && today_age.day() < birth_date.day())) {
        --age;
    }
    
    return age;
}

auto birth_ex = 1990y / March / 15d;
int age_ex = calculate_age(birth_ex);

std::cout << "Birth date: " << birth_ex << '\n';
std::cout << "Age: " << age_ex << " years\n";

// ──────────────────────────────────────────
// Рабочие дни (без выходных)
// ──────────────────────────────────────────

bool is_weekend(weekday wd) {
    return wd == Saturday || wd == Sunday;
}

int count_business_days(year_month_day start, year_month_day end) {
    int count = 0;
    auto current = sys_days{start};
    auto end_date = sys_days{end};
    
    while (current <= end_date) {
        weekday wd{current};
        if (!is_weekend(wd)) {
            ++count;
        }
        current += days{1};
    }
    
    return count;
}

auto start_bd = 2024y / January / 1d;
auto end_bd = 2024y / January / 31d;

int business_days_ex = count_business_days(start_bd, end_bd);
std::cout << "Business days in Jan 2024: " << business_days_ex << '\n';

// ====================================================================================================
// 📌 BEST PRACTICES
// ====================================================================================================

/*
 * BEST PRACTICES ДЛЯ ВРЕМЕНИ:
 * 
 * 1. CLOCKS
 *    ✅ steady_clock для измерений (монотонные)
 *    ✅ system_clock для реального времени
 *    ❌ Не используй system_clock для измерений
 * 
 * 2. DURATION
 *    ✅ Используй duration literals (1h, 30min, 45s)
 *    ✅ duration_cast для конвертации
 *    ✅ Явные типы (не auto везде)
 * 
 * 3. CALENDAR (C++20)
 *    ✅ year_month_day для дат
 *    ✅ Проверяй ok() для валидации
 *    ✅ Operator / для читаемости (2024y/Jan/15d)
 * 
 * 4. TIME ZONES
 *    ✅ zoned_time для конкретных зон
 *    ⚠️ Учитывай DST (daylight saving)
 *    ✅ current_zone() для локальной зоны
 * 
 * 5. PERFORMANCE
 *    ✅ Кешируй now() если вызывается часто
 *    ✅ steady_clock быстрее system_clock
 *    ❌ Избегай частых to_time_t конвертаций
 * 
 * 6. ФОРМАТИРОВАНИЕ
 *    ✅ std::format (C++20) - типобезопасно
 *    ⚠️ strftime - старый способ
 *    ✅ ISO 8601 для обмена данными
 */

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================


// ====================================================================================================
// 📌 ИТОГО: C++ Time & Chrono (C++20)
// ====================================================================================================
// • Duration - промежутки времени с литералами (1s, 500ms)
// • steady_clock - измерения, system_clock - реальное время
// • Calendar (C++20): year_month_day, weekday, арифметика дат
// • Time zones (C++20): zoned_time, конверсии временных зон
// • std::format - типобезопасное форматирование
// • Практические примеры: Timer, Deadline, возраст, FPS
