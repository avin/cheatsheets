/*
 * ============================================
 * СТРОКИ И ТЕКСТ В C++
 * ============================================
 * 
 * Полное руководство по работе со строками:
 * - std::string и string_view
 * - std::format (C++20)
 * - Regular expressions
 * - Parsing и конвертация
 * - Unicode support
 * 
 * Компиляция: g++ -std=c++20 strings_text.cpp
 */

#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <algorithm>
#include <regex>
#include <charconv>
#include <vector>

#ifdef __cpp_lib_format
#include <format>
#endif

// ====================================================================================================
// 📌 std::string BASICS
// ====================================================================================================

// ──────────────────────────────────────────
// String Construction - создание строк
// ──────────────────────────────────────────

std::string s1;                          // Пустая
std::string s2 = "Hello";                // Из литерала
std::string s3(5, 'x');                  // "xxxxx"
std::string s4(s2, 0, 3);                // "Hel"
std::string s5 = s2 + " World";          // Конкатенация

std::cout << "s2: " << s2 << '\n';
std::cout << "s3: " << s3 << '\n';
std::cout << "s4: " << s4 << '\n';
std::cout << "s5: " << s5 << '\n';

// ──────────────────────────────────────────
// Capacity Management - управление памятью
// ──────────────────────────────────────────

std::string str = "Hello";

std::cout << "Size: " << str.size() << '\n';
std::cout << "Capacity: " << str.capacity() << '\n';

// reserve - резервирует память
str.reserve(100);
std::cout << "After reserve(100): " << str.capacity() << '\n';

// shrink_to_fit - освобождает лишнюю память
str.shrink_to_fit();
std::cout << "After shrink_to_fit: " << str.capacity() << '\n';

// SSO (Small String Optimization)
std::string small = "Hi";
std::string large = "This is a long string that probably won't fit in SSO";
std::cout << "\nSSO demo:\n";
std::cout << "Small capacity: " << small.capacity() << '\n';
std::cout << "Large capacity: " << large.capacity() << '\n';

// ──────────────────────────────────────────
// Search Operations - поиск в строках
// ──────────────────────────────────────────

std::string text = "Hello World, Hello C++!";

// find - первое вхождение
size_t pos = text.find("Hello");
    std::cout << "First 'Hello' at: " << pos << '\n';
    
    // find с позиции
    pos = text.find("Hello", pos + 1);
    std::cout << "Second 'Hello' at: " << pos << '\n';
    
    // rfind - последнее вхождение
    pos = text.rfind("Hello");
    std::cout << "Last 'Hello' at: " << pos << '\n';
    
std::cout << "First 'Hello' at: " << pos << '\n';

// find - поиск с позиции
pos = text.find("Hello", pos + 1);
std::cout << "Second 'Hello' at: " << pos << '\n';

// find_first_of - любой из символов
pos = text.find_first_of("aeiou");
std::cout << "First vowel at: " << pos << " ('" << text[pos] << "')\n";

// find_last_of
pos = text.find_last_of("!");
std::cout << "Last '!' at: " << pos << '\n';

// ──────────────────────────────────────────
// C++20 Features - starts_with, ends_with
// ──────────────────────────────────────────

std::string filename = "hello_world.cpp";

// starts_with (C++20)
if (filename.starts_with("hello")) {
    std::cout << "Starts with 'hello'\n";
}

// ends_with (C++20)
if (filename.ends_with(".cpp")) {
    std::cout << "Is a C++ file\n";
}

#ifdef __cpp_lib_string_contains
// contains (C++23)
if (filename.contains("world")) {
    std::cout << "Contains 'world'\n";
}
#endif

// ====================================================================================================
// 📌 std::string_view (C++17) - НЕ КОПИРУЕТ СТРОКУ
// ====================================================================================================

// ❌ Плохо - копирование
void process_bad(const std::string& str) {
    std::cout << "Length: " << str.size() << '\n';
}

// ✅ Хорошо - никаких копий
void process_good(std::string_view str) {
    std::cout << "Length: " << str.size() << '\n';
}

// ──────────────────────────────────────────
// Performance - никаких аллокаций
// ──────────────────────────────────────────

const char* cstr = "Hello World";

// string_view - без аллокации!
std::string_view sv = cstr;

// substr тоже дешево - НЕ КОПИРУЕТ!
auto substr_view = sv.substr(0, 5);
std::cout << "Substring: " << substr_view << '\n';

// ──────────────────────────────────────────
// Lifetime Issues - ОПАСНОСТЬ
// ──────────────────────────────────────────

std::string_view dangerous;

{
    std::string temp = "Temporary";
    dangerous = temp;  // ❌ Опасно!
    std::cout << "Inside scope: " << dangerous << '\n';
}  // temp уничтожен!

// std::cout << dangerous << '\n';  // ❌ Undefined behavior!

// ✅ Безопасно - литералы живут всю программу
std::string_view safe_literal = "String literal";
std::cout << "Safe: " << safe_literal << '\n';

// ──────────────────────────────────────────
// Парсинг без копирования
// ──────────────────────────────────────────

auto parse_key_value = [](std::string_view line) {
    auto pos = line.find('=');
    if (pos != std::string_view::npos) {
        auto key = line.substr(0, pos);
        auto value = line.substr(pos + 1);
        std::cout << "Key: " << key << ", Value: " << value << '\n';
    }
};

parse_key_value("name=John");
parse_key_value("age=30");

// Токенизация без копирования
std::string_view tokenize_text = "one two three";
size_t token_start = 0;
size_t token_end;

std::cout << "Tokens: ";
while ((token_end = tokenize_text.find(' ', token_start)) != std::string_view::npos) {
    std::cout << tokenize_text.substr(token_start, token_end - token_start) << ' ';
    token_start = token_end + 1;
}
std::cout << tokenize_text.substr(token_start) << '\n';

// ====================================================================================================
// 📌 std::format (C++20) - ФОРМАТИРОВАНИЕ СТРОК
// ====================================================================================================

#ifdef __cpp_lib_format

// Базовое форматирование
auto format_basic = std::format("Hello, {}!", "World");
std::cout << format_basic << '\n';

// Позиционные аргументы
auto format_pos = std::format("{0} {1} {0}", "Hello", "World");
std::cout << format_pos << '\n';

// Числовые типы
int age = 30;
auto format_int = std::format("Age: {}", age);
std::cout << format_int << '\n';

// ──────────────────────────────────────────
// Выравнивание текста
// ──────────────────────────────────────────

std::cout << std::format("|{:<10}|", "left") << '\n';    // Влево
std::cout << std::format("|{:>10}|", "right") << '\n';   // Вправо
std::cout << std::format("|{:^10}|", "center") << '\n';  // По центру

// ──────────────────────────────────────────
// Форматирование чисел
// ──────────────────────────────────────────

std::cout << std::format("Dec: {}, Hex: {:x}, Oct: {:o}", 42, 42, 42) << '\n';
std::cout << std::format("Float: {:.2f}", 3.14159) << '\n';

// Заполнение символами
std::cout << std::format("{:*>10}", "test") << '\n';  // ******test

// ──────────────────────────────────────────
// Custom Types - форматирование своих типов
// ──────────────────────────────────────────

struct Point {
    int x, y;
};

// Простой способ без своего formatter
Point p{10, 20};
auto point_str = std::format("Point({}, {})", p.x, p.y);
std::cout << point_str << '\n';

#else

// Fallback на старый способ
std::ostringstream oss;
oss << "Hello, " << "World" << "!";
std::cout << "std::format not available, using ostringstream: " << oss.str() << '\n';

#endif

// ====================================================================================================
// 📌 STRING MANIPULATION - РАБОТА СО СТРОКАМИ
// ====================================================================================================

// ──────────────────────────────────────────
// Helper Functions - вспомогательные функции
// ──────────────────────────────────────────

// Case conversion - изменение регистра
std::string to_upper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return str;
}

std::string to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return str;
}

// Trimming - обрезка пробелов
std::string trim_left(std::string str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(),
              [](unsigned char ch) { return !std::isspace(ch); }));
    return str;
}

std::string trim_right(std::string str) {
    str.erase(std::find_if(str.rbegin(), str.rend(),
              [](unsigned char ch) { return !std::isspace(ch); }).base(),
              str.end());
    return str;
}

std::string trim(std::string str) {
    return trim_left(trim_right(std::move(str)));
}

// Splitting - разбиение строки
std::vector<std::string> split(std::string_view str, char delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end;
    
    while ((end = str.find(delimiter, start)) != std::string_view::npos) {
        result.emplace_back(str.substr(start, end - start));
        start = end + 1;
    }
    result.emplace_back(str.substr(start));
    
    return result;
}

// Joining - объединение строк
std::string join(const std::vector<std::string>& parts, std::string_view separator) {
    if (parts.empty()) return "";
    
    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += separator;
        result += parts[i];
    }
    return result;
}

// Replace all - замена всех вхождений
std::string replace_all(std::string str, std::string_view from, std::string_view to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
    return str;
}

// ──────────────────────────────────────────
// Примеры использования
// ──────────────────────────────────────────

// Case conversion
std::cout << "Upper: " << to_upper("hello") << '\n';
std::cout << "Lower: " << to_lower("WORLD") << '\n';

// Trimming
std::cout << "Trimmed: '" << trim("  hello  ") << "'\n";

// Splitting
auto parts = split("one,two,three", ',');
std::cout << "Split: ";
for (const auto& part : parts) {
    std::cout << part << ' ';
}
std::cout << '\n';

// Joining
std::cout << "Joined: " << join(parts, " | ") << '\n';

// Replace
std::cout << "Replace: " << replace_all("hello world", "o", "0") << '\n';

// ====================================================================================================
// 📌 REGULAR EXPRESSIONS - РЕГУЛЯРНЫЕ ВЫРАЖЕНИЯ
// ====================================================================================================

// ──────────────────────────────────────────
// Regex Matching - поиск по шаблону
// ──────────────────────────────────────────

std::string email_text = "hello@example.com";

// regex_match - полное совпадение
std::regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

if (std::regex_match(email_text, email_pattern)) {
    std::cout << "Valid email: " << email_text << '\n';
}

// regex_search - поиск в тексте
std::string document = "Contact us at support@example.com or sales@example.com";
std::smatch search_match;

if (std::regex_search(document, search_match, email_pattern)) {
    std::cout << "Found email: " << search_match.str() << '\n';
}

// ──────────────────────────────────────────
// Capture Groups - группы захвата
// ──────────────────────────────────────────

std::string date_text = "2024-01-15";
std::regex date_pattern(R"((\d{4})-(\d{2})-(\d{2}))");
std::smatch date_match;

if (std::regex_match(date_text, date_match, date_pattern)) {
    std::cout << "Full match: " << date_match[0] << '\n';
    std::cout << "Year: " << date_match[1] << '\n';
    std::cout << "Month: " << date_match[2] << '\n';
    std::cout << "Day: " << date_match[3] << '\n';
}

// ──────────────────────────────────────────
// Regex Replace - замена по шаблону
// ──────────────────────────────────────────

std::string phone_text = "Phone: 123-456-7890";
std::regex phone_pattern(R"(\d{3}-\d{3}-\d{4})");

// Замена на фиксированный текст
std::string redacted = std::regex_replace(phone_text, phone_pattern, "XXX-XXX-XXXX");
std::cout << "Redacted: " << redacted << '\n';

// Использование групп захвата в замене ($& - полное совпадение)
std::string word_text = "hello world";
std::regex word_pattern(R"(\w+)");
std::string bracketed = std::regex_replace(word_text, word_pattern, "[$&]");
std::cout << "Brackets: " << bracketed << '\n';

// ──────────────────────────────────────────
// Common Patterns - популярные шаблоны
// ──────────────────────────────────────────

// Email
std::regex re_email(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

// URL
std::regex re_url(R"(https?://[^\s]+)");

// Phone (US format)
std::regex re_phone(R"(\d{3}-\d{3}-\d{4})");

// IP address
std::regex re_ip(R"((\d{1,3}\.){3}\d{1,3})");

// Тестирование шаблонов
std::vector<std::pair<std::string, std::regex*>> pattern_tests = {
    {"test@example.com", &re_email},
    {"https://example.com", &re_url},
    {"123-456-7890", &re_phone},
    {"192.168.1.1", &re_ip}
};

for (const auto& [test_text, pattern_ptr] : pattern_tests) {
    std::cout << test_text << ": " 
              << (std::regex_match(test_text, *pattern_ptr) ? "✓" : "✗") << '\n';
}

// ====================================================================================================
// 📌 STRING PARSING - ПАРСИНГ СТРОК
// ====================================================================================================

// ──────────────────────────────────────────
// Old School - std::stoi, stod и т.д.
// ──────────────────────────────────────────

try {
    int parse_i = std::stoi("42");
    double parse_d = std::stod("3.14");
    long parse_l = std::stol("1000000");
    
    std::cout << "int: " << parse_i << '\n';
    std::cout << "double: " << parse_d << '\n';
    std::cout << "long: " << parse_l << '\n';
} catch (const std::exception& e) {
    std::cerr << "Parse error: " << e.what() << '\n';
}

// ──────────────────────────────────────────
// Modern Parsing - std::from_chars (C++17)
// ──────────────────────────────────────────

// from_chars - БЫСТРЕЕ и БЕЗ ИСКЛЮЧЕНИЙ!
const char* parse_str = "12345";
int parsed_value;

auto [ptr, ec] = std::from_chars(parse_str, parse_str + std::strlen(parse_str), parsed_value);

if (ec == std::errc{}) {
    std::cout << "Parsed: " << parsed_value << '\n';
} else {
    std::cout << "Parse error\n";
}

// to_chars - быстрая конвертация в строку
char to_chars_buffer[32];
auto [buffer_end, ec2] = std::to_chars(to_chars_buffer, to_chars_buffer + sizeof(to_chars_buffer), 42);

if (ec2 == std::errc{}) {
    std::cout << "Formatted: " << std::string_view(to_chars_buffer, buffer_end - to_chars_buffer) << '\n';
}

// ──────────────────────────────────────────
// CSV Parsing - парсинг CSV
// ──────────────────────────────────────────

auto parse_csv_line = [](std::string_view line) {
    std::vector<std::string> fields;
    size_t start = 0;
    size_t end;
    
    while ((end = line.find(',', start)) != std::string_view::npos) {
        fields.emplace_back(line.substr(start, end - start));
        start = end + 1;
    }
    fields.emplace_back(line.substr(start));
    
    return fields;
};

std::string csv_data = "John,Doe,30,Engineer";
auto csv_fields = parse_csv_line(csv_data);

std::cout << "CSV fields:\n";
for (size_t i = 0; i < csv_fields.size(); ++i) {
    std::cout << i << ": " << csv_fields[i] << '\n';
}

// ====================================================================================================
// 📌 STRING LITERALS - СТРОКОВЫЕ ЛИТЕРАЛЫ
// ====================================================================================================

// ──────────────────────────────────────────
// Raw String Literals - сырые литералы
// ──────────────────────────────────────────

// Обычная строка - нужны escape
std::string path1 = "C:\\Users\\John\\Documents";

// Raw literal - НЕ нужны escape!
std::string path2 = R"(C:\Users\John\Documents)";

std::cout << "Path: " << path2 << '\n';

// Многострочные raw literals
std::string json = R"({
    "name": "John",
    "age": 30
})";

std::cout << "JSON:\n" << json << '\n';

// Custom delimiter - если внутри есть )"
std::string complex = R"delimiter(Raw string with )" inside)delimiter";
std::cout << "Complex: " << complex << '\n';

// ──────────────────────────────────────────
// User-Defined Literals - пользовательские литералы
// ──────────────────────────────────────────

using namespace std::string_literals;

// ""s - создает std::string
auto lit_s1 = "Hello"s;  // std::string
auto lit_s2 = "World";   // const char*

std::cout << "lit_s1 is std::string\n";
std::cout << "lit_s2 is const char*\n";

// ""sv - создает std::string_view
using namespace std::string_view_literals;
auto lit_sv = "Hello"sv;  // std::string_view

// ====================================================================================================
// 📌 PERFORMANCE OPTIMIZATION - ОПТИМИЗАЦИЯ ПРОИЗВОДИТЕЛЬНОСТИ
// ====================================================================================================

// ──────────────────────────────────────────
// Избегайте копий - Avoid Copies
// ──────────────────────────────────────────

// ❌ Создает копии
std::string process_bad_copy(std::string s) {
    return s + " processed";
}

// ✅ Использует move
std::string process_good_move(std::string s) {
    s += " processed";
    return s;  // RVO (Return Value Optimization)
}

// ✅ string_view для readonly параметров
auto analyze_readonly = [](std::string_view s) {
    std::cout << "Analyzing: " << s << '\n';
};

std::string analyze_data = "test";
analyze_readonly(analyze_data);  // Никаких копий!

// ──────────────────────────────────────────
// Reserve - резервирование памяти
// ──────────────────────────────────────────

// ❌ Множественные реаллокации
std::string perf_s1;
for (int i = 0; i < 1000; ++i) {
    perf_s1 += 'x';
}

// ✅ Одна аллокация
std::string perf_s2;
perf_s2.reserve(1000);
for (int i = 0; i < 1000; ++i) {
    perf_s2 += 'x';
}

std::cout << "Reserve prevents reallocations\n";

// ──────────────────────────────────────────
// Move Semantics - семантика перемещения
// ──────────────────────────────────────────

std::string move_s1 = "Large string with lots of data...";

// ❌ Копирование
// std::string s2 = s1;  // Копия!

// ✅ Перемещение
std::string move_s2 = std::move(move_s1);

std::cout << "move_s1 moved to move_s2\n";
std::cout << "move_s1 is now: '" << move_s1 << "' (empty)\n";
std::cout << "move_s2 is: '" << move_s2 << "'\n";

// ============================================
// 📌 BEST PRACTICES
// ============================================

/*
 * BEST PRACTICES ДЛЯ СТРОК:
 * 
 * 1. ВЫБОР ТИПА
 *    ✅ std::string_view для параметров (readonly)
 *    ✅ std::string для владения
 *    ❌ const char* избегай
 * 
 * 2. ФОРМАТИРОВАНИЕ
 *    ✅ std::format (C++20) - безопасно и быстро
 *    ❌ sprintf - небезопасно
 *    ⚠️ stringstream - медленно
 * 
 * 3. ПРОИЗВОДИТЕЛЬНОСТЬ
 *    ✅ reserve() перед построением
 *    ✅ += вместо + в циклах
 *    ✅ string_view вместо substr()
 *    ✅ move semantics
 * 
 * 4. REGEX
 *    ⚠️ Дорогая компиляция - кешируй std::regex
 *    ✅ Используй raw literals R"(...)"
 *    ⚠️ Проверяй производительность
 * 
 * 5. ПАРСИНГ
 *    ✅ from_chars/to_chars (C++17) - fastest
 *    ⚠️ stoi/stod - бросают исключения
 *    ❌ atoi - нет обработки ошибок
 * 
 * 6. UNICODE
 *    ✅ UTF-8 для большинства случаев
 *    ⚠️ std::wstring редко нужен
 *    ✅ Используй библиотеки (ICU, Boost.Locale)
 */

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================


// ====================================================================================================
// 📌 ИТОГО: C++ Strings & Text Processing
// ====================================================================================================
// • string_view - избегает копий
// • std::format (C++20) - типобезопасное форматирование
// • from_chars/to_chars - быстрый парсинг
// • Regex - сложные паттерны
// • reserve() - оптимизация выделения памяти
// • Raw literals R"(...)" - читаемость
