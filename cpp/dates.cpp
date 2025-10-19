#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace std::chrono;

// ---------------------------------------------------
// 📌 Получение текущего времени
// ---------------------------------------------------
void example_now() {
    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);
    std::cout << std::ctime(&now_time_t);
}

// ---------------------------------------------------
// 📌 Создание даты из компонентов
// ---------------------------------------------------
std::time_t createDate(int year, int month, int day, int hour = 0, int minute = 0, int second = 0) {
    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    return std::mktime(&tm);
}

// ---------------------------------------------------
// 📌 Форматирование даты/времени
// ---------------------------------------------------
std::string formatTime(const std::time_t& time, const char* format) {
    std::tm* tm = std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(tm, format);
    return oss.str();
}

void example_format() {
    auto now = system_clock::now();
    auto time_t_now = system_clock::to_time_t(now);

    // "2024-01-15 10:30:00"
    std::cout << formatTime(time_t_now, "%Y-%m-%d %H:%M:%S") << std::endl;

    // "15.01.2024"
    std::cout << formatTime(time_t_now, "%d.%m.%Y") << std::endl;

    // "Mon Jan 15"
    std::cout << formatTime(time_t_now, "%a %b %d") << std::endl;
}

// ---------------------------------------------------
// 📌 Парсинг строки в дату
// ---------------------------------------------------
std::time_t parseDate(const std::string& dateStr, const char* format) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, format);
    return std::mktime(&tm);
}

// Пример: parseDate("2024-01-15", "%Y-%m-%d")

// ---------------------------------------------------
// 📌 Получение компонентов даты
// ---------------------------------------------------
void getDateComponents(const std::time_t& time) {
    std::tm* tm = std::localtime(&time);

    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;   // 1-12
    int day = tm->tm_mday;         // 1-31
    int hour = tm->tm_hour;        // 0-23
    int minute = tm->tm_min;       // 0-59
    int second = tm->tm_sec;       // 0-59
    int weekday = tm->tm_wday;     // 0 (Sunday) - 6 (Saturday)
    int yearday = tm->tm_yday;     // 0-365
}

// ---------------------------------------------------
// 📌 Разница между датами (duration)
// ---------------------------------------------------
void example_duration() {
    auto start = system_clock::now();
    // ... выполнение кода ...
    auto end = system_clock::now();

    auto duration_ms = duration_cast<milliseconds>(end - start);
    auto duration_s = duration_cast<seconds>(end - start);
    auto duration_m = duration_cast<minutes>(end - start);

    std::cout << "Миллисекунды: " << duration_ms.count() << std::endl;
    std::cout << "Секунды: " << duration_s.count() << std::endl;
}

// ---------------------------------------------------
// 📌 Добавление времени к дате
// ---------------------------------------------------
void example_add_time() {
    auto now = system_clock::now();

    // Добавить 1 день
    auto tomorrow = now + hours(24);
    // или
    auto tomorrow2 = now + days(1);

    // Добавить 2 часа
    auto in_two_hours = now + hours(2);

    // Добавить 30 минут
    auto in_30_min = now + minutes(30);

    // Добавить 5 секунд
    auto in_5_sec = now + seconds(5);
}

// ---------------------------------------------------
// 📌 Сравнение дат
// ---------------------------------------------------
void example_compare() {
    auto date1 = system_clock::now();
    auto date2 = system_clock::now() + days(1);

    bool isBefore = date1 < date2;
    bool isAfter = date1 > date2;
    bool isEqual = date1 == date2;
}

// ---------------------------------------------------
// 📌 Timestamp (секунды/миллисекунды с эпохи)
// ---------------------------------------------------
void example_timestamp() {
    auto now = system_clock::now();

    // Секунды с 1970-01-01
    auto seconds = duration_cast<std::chrono::seconds>(
        now.time_since_epoch()
    ).count();

    // Миллисекунды с 1970-01-01
    auto milliseconds = duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();

    // Создание даты из timestamp
    auto from_timestamp = system_clock::from_time_t(1705276800);
}

// ---------------------------------------------------
// 📌 Начало/конец дня
// ---------------------------------------------------
std::time_t startOfDay(std::time_t time) {
    std::tm* tm = std::localtime(&time);
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    return std::mktime(tm);
}

std::time_t endOfDay(std::time_t time) {
    std::tm* tm = std::localtime(&time);
    tm->tm_hour = 23;
    tm->tm_min = 59;
    tm->tm_sec = 59;
    return std::mktime(tm);
}

// ---------------------------------------------------
// 📌 Високосный год
// ---------------------------------------------------
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// ---------------------------------------------------
// 📌 Количество дней в месяце
// ---------------------------------------------------
int getDaysInMonth(int year, int month) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[month - 1];
}

// ---------------------------------------------------
// 📌 Измерение времени выполнения
// ---------------------------------------------------
template<typename Func>
void measureTime(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start);
    std::cout << "Время выполнения: " << duration.count() << " мкс" << std::endl;
}

// ---------------------------------------------------
// 📌 Сон (задержка выполнения)
// ---------------------------------------------------
void example_sleep() {
    std::this_thread::sleep_for(milliseconds(1000)); // 1 секунда
    std::this_thread::sleep_for(seconds(1));         // 1 секунда
    std::this_thread::sleep_for(minutes(1));         // 1 минута
}

// ---------------------------------------------------
// 📌 Разница между двумя датами в днях
// ---------------------------------------------------
int daysBetween(std::time_t date1, std::time_t date2) {
    double diff = std::difftime(date2, date1);
    return static_cast<int>(diff / (60 * 60 * 24));
}

// ---------------------------------------------------
// 📌 Добавление дней к дате
// ---------------------------------------------------
std::time_t addDays(std::time_t time, int days) {
    return time + (days * 24 * 60 * 60);
}

// ---------------------------------------------------
// 📌 UTC время
// ---------------------------------------------------
void example_utc() {
    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);

    std::tm* utc_tm = std::gmtime(&now_time_t);
    std::cout << std::put_time(utc_tm, "%Y-%m-%d %H:%M:%S UTC") << std::endl;
}

// ---------------------------------------------------
// 📌 Таймер (steady_clock для точных измерений)
// ---------------------------------------------------
class Timer {
    steady_clock::time_point start_time;

public:
    Timer() : start_time(steady_clock::now()) {}

    void reset() {
        start_time = steady_clock::now();
    }

    double elapsed_seconds() const {
        auto now = steady_clock::now();
        return duration_cast<duration<double>>(now - start_time).count();
    }

    long long elapsed_milliseconds() const {
        auto now = steady_clock::now();
        return duration_cast<milliseconds>(now - start_time).count();
    }
};

// ---------------------------------------------------
// 📌 Преобразование между time_point и time_t
// ---------------------------------------------------
void example_conversions() {
    // time_point -> time_t
    auto now = system_clock::now();
    std::time_t time_t_now = system_clock::to_time_t(now);

    // time_t -> time_point
    auto time_point_from_t = system_clock::from_time_t(time_t_now);
}

// ---------------------------------------------------
// 📌 Форматирование ISO 8601
// ---------------------------------------------------
std::string toISO8601(const std::time_t& time) {
    std::tm* tm = std::gmtime(&time);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}
