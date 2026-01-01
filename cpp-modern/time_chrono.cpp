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

// ============================================
// 📌 std::chrono BASICS
// ============================================

namespace chrono_basics {

void demo_duration() {
    std::cout << "=== Duration ===\n";
    
    // Duration - промежуток времени
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
}

void demo_time_point() {
    std::cout << "\n=== Time Point ===\n";
    
    // Time point - момент времени
    auto now = system_clock::now();
    
    // Арифметика с duration
    auto future = now + hours(24);
    auto past = now - minutes(30);
    
    // Разница между time points
    auto diff = future - now;
    std::cout << "Difference: " << duration_cast<hours>(diff).count() << " hours\n";
}

void demo_clocks() {
    std::cout << "\n=== Clocks ===\n";
    
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
    std::cout << "High-resolution clock available\n";
}

void demo() {
    std::cout << "=== Chrono Basics ===\n";
    demo_duration();
    demo_time_point();
    demo_clocks();
}

} // namespace chrono_basics

// ============================================
// 📌 DURATION TYPES
// ============================================

namespace duration_types {

void demo_standard_durations() {
    std::cout << "\n=== Standard Durations ===\n";
    
    // Стандартные типы
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
}

void demo_literals() {
    std::cout << "\n=== Duration Literals ===\n";
    
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
}

void demo_custom_duration() {
    std::cout << "\n=== Custom Duration ===\n";
    
    // Custom duration - например, сотни миллисекунд
    using centiseconds = duration<long long, std::centi>;
    
    centiseconds cs(500);  // 500 сотых секунды = 5 секунд
    auto sec = duration_cast<seconds>(cs);
    std::cout << "Centiseconds to seconds: " << sec.count() << '\n';
}

void demo_conversions() {
    std::cout << "\n=== Duration Conversions ===\n";
    
    auto h = 2h;
    auto m = duration_cast<minutes>(h);
    auto s = duration_cast<seconds>(h);
    auto ms = duration_cast<milliseconds>(h);
    
    std::cout << h.count() << " hours = \n";
    std::cout << "  " << m.count() << " minutes\n";
    std::cout << "  " << s.count() << " seconds\n";
    std::cout << "  " << ms.count() << " milliseconds\n";
}

void demo() {
    std::cout << "\n=== Duration Types ===\n";
    demo_standard_durations();
    demo_literals();
    demo_custom_duration();
    demo_conversions();
}

} // namespace duration_types

// ============================================
// 📌 CALENDAR (C++20)
// ============================================

namespace calendar_examples {

void demo_basic_dates() {
    std::cout << "\n=== Basic Dates ===\n";
    
    // Создание дат
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
}

void demo_weekdays() {
    std::cout << "\n=== Weekdays ===\n";
    
    // Weekday
    weekday wd{Sunday};
    std::cout << "Weekday: " << wd << '\n';
    
    // Найти день недели для даты
    auto ymd = 2024y / January / 15d;
    weekday date_wd{sys_days{ymd}};
    std::cout << "2024-01-15 is " << date_wd << '\n';
    
    // Следующий понедельник
    auto next_monday = sys_days{ymd} + (Monday - date_wd);
    std::cout << "Next Monday: " << year_month_day{next_monday} << '\n';
}

void demo_date_arithmetic() {
    std::cout << "\n=== Date Arithmetic ===\n";
    
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
}

void demo_date_validation() {
    std::cout << "\n=== Date Validation ===\n";
    
    // Валидная дата
    auto valid = 2024y / January / 31d;
    std::cout << "2024-01-31 ok: " << valid.ok() << '\n';
    
    // Невалидная дата
    auto invalid = 2024y / February / 30d;
    std::cout << "2024-02-30 ok: " << invalid.ok() << '\n';
    
    // Високосный год
    std::cout << "2024 is leap: " << year{2024}.is_leap() << '\n';
    std::cout << "2023 is leap: " << year{2023}.is_leap() << '\n';
}

void demo() {
    std::cout << "\n=== Calendar (C++20) ===\n";
    demo_basic_dates();
    demo_weekdays();
    demo_date_arithmetic();
    demo_date_validation();
}

} // namespace calendar_examples

// ============================================
// 📌 TIME ZONES (C++20)
// ============================================

namespace timezone_examples {

void demo_timezones() {
    std::cout << "\n=== Time Zones ===\n";
    
    #ifdef __cpp_lib_chrono_zone
    // Текущая временная зона
    auto current_tz = current_zone();
    std::cout << "Current zone: " << current_tz->name() << '\n';
    
    // Конкретная зона
    auto ny_tz = locate_zone("America/New_York");
    auto tokyo_tz = locate_zone("Asia/Tokyo");
    
    // zoned_time - время в конкретной зоне
    auto now = system_clock::now();
    zoned_time ny_time{ny_tz, now};
    zoned_time tokyo_time{tokyo_tz, now};
    
    std::cout << "NY time: " << ny_time << '\n';
    std::cout << "Tokyo time: " << tokyo_time << '\n';
    #else
    std::cout << "Time zone support not available\n";
    #endif
}

void demo() {
    std::cout << "\n=== Time Zones ===\n";
    demo_timezones();
}

} // namespace timezone_examples

// ============================================
// 📌 FORMATTING
// ============================================

namespace formatting_examples {

void demo_formatting() {
    std::cout << "\n=== Formatting ===\n";
    
    auto now = system_clock::now();
    auto today = year_month_day{floor<days>(now)};
    
    #ifdef __cpp_lib_format
    // std::format для chrono (C++20)
    std::cout << std::format("Date: {}\n", today);
    std::cout << std::format("Time: {:%H:%M:%S}\n", now);
    std::cout << std::format("ISO: {:%F %T}\n", now);
    #else
    // Fallback на старый способ
    time_t tt = system_clock::to_time_t(now);
    std::cout << "Date: " << std::put_time(std::localtime(&tt), "%Y-%m-%d") << '\n';
    std::cout << "Time: " << std::put_time(std::localtime(&tt), "%H:%M:%S") << '\n';
    #endif
}

void demo() {
    std::cout << "\n=== Formatting ===\n";
    demo_formatting();
}

} // namespace formatting_examples

// ============================================
// 📌 PRACTICAL EXAMPLES
// ============================================

namespace practical_examples {

// Текущая дата и время
void demo_current_datetime() {
    std::cout << "\n=== Current Date/Time ===\n";
    
    auto now = system_clock::now();
    time_t tt = system_clock::to_time_t(now);
    
    std::cout << "Current time: " << std::ctime(&tt);
    
    auto today = year_month_day{floor<days>(now)};
    std::cout << "Today: " << today << '\n';
}

// Измерение времени выполнения
template<typename Func>
auto measure_time(Func f) {
    auto start = steady_clock::now();
    f();
    auto end = steady_clock::now();
    return duration_cast<milliseconds>(end - start);
}

void demo_timing() {
    std::cout << "\n=== Timing Operations ===\n";
    
    auto elapsed = measure_time([]() {
        std::this_thread::sleep_for(100ms);
    });
    
    std::cout << "Operation took: " << elapsed.count() << " ms\n";
}

// Таймер
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

void demo_timer() {
    std::cout << "\n=== Timer ===\n";
    
    Timer timer;
    
    std::this_thread::sleep_for(50ms);
    timer.print_elapsed();
    
    std::this_thread::sleep_for(50ms);
    timer.print_elapsed();
    
    timer.reset();
    std::this_thread::sleep_for(30ms);
    timer.print_elapsed();
}

// Deadline и timeout
void demo_deadline() {
    std::cout << "\n=== Deadline ===\n";
    
    auto deadline = steady_clock::now() + 200ms;
    
    while (steady_clock::now() < deadline) {
        // Работа...
        std::this_thread::sleep_for(50ms);
        std::cout << "Working...\n";
    }
    
    std::cout << "Deadline reached!\n";
}

// Вычисление возраста
int calculate_age(year_month_day birth_date) {
    auto today = year_month_day{floor<days>(system_clock::now())};
    
    int age = static_cast<int>(today.year()) - static_cast<int>(birth_date.year());
    
    // Корректировка если день рождения еще не наступил
    if (today.month() < birth_date.month() ||
        (today.month() == birth_date.month() && today.day() < birth_date.day())) {
        --age;
    }
    
    return age;
}

void demo_age_calculation() {
    std::cout << "\n=== Age Calculation ===\n";
    
    auto birth = 1990y / March / 15d;
    int age = calculate_age(birth);
    
    std::cout << "Birth date: " << birth << '\n';
    std::cout << "Age: " << age << " years\n";
}

// Рабочие дни
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

void demo_business_days() {
    std::cout << "\n=== Business Days ===\n";
    
    auto start = 2024y / January / 1d;
    auto end = 2024y / January / 31d;
    
    int business_days = count_business_days(start, end);
    std::cout << "Business days in Jan 2024: " << business_days << '\n';
}

void demo() {
    std::cout << "\n=== Practical Examples ===\n";
    demo_current_datetime();
    demo_timing();
    demo_timer();
    demo_deadline();
    demo_age_calculation();
    demo_business_days();
}

} // namespace practical_examples

// ============================================
// 📌 BEST PRACTICES
// ============================================

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

int main() {
    std::cout << "=== C++ Time & Chrono ===\n";
    
    chrono_basics::demo();
    duration_types::demo();
    calendar_examples::demo();
    timezone_examples::demo();
    formatting_examples::demo();
    practical_examples::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ Duration - промежутки времени с литералами\n";
    std::cout << "✓ steady_clock для измерений, system_clock для времени\n";
    std::cout << "✓ Calendar (C++20): year_month_day, weekday\n";
    std::cout << "✓ Time zones (C++20): zoned_time\n";
    std::cout << "✓ std::format для типобезопасного форматирования\n";
    std::cout << "✓ Практические примеры: таймеры, deadline, возраст\n";
    
    return 0;
}
