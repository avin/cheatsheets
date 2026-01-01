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

// ============================================
// 📌 std::string BASICS
// ============================================

namespace string_basics {

void construction() {
    std::cout << "=== String Construction ===\n";
    
    std::string s1;                          // Пустая
    std::string s2 = "Hello";                // Из литерала
    std::string s3(5, 'x');                  // "xxxxx"
    std::string s4(s2, 0, 3);                // "Hel"
    std::string s5 = s2 + " World";          // Конкатенация
    
    std::cout << "s2: " << s2 << '\n';
    std::cout << "s3: " << s3 << '\n';
    std::cout << "s4: " << s4 << '\n';
    std::cout << "s5: " << s5 << '\n';
}

void capacity_management() {
    std::cout << "\n=== Capacity Management ===\n";
    
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
}

void search_operations() {
    std::cout << "\n=== Search Operations ===\n";
    
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
    
    // find_first_of - любой из символов
    pos = text.find_first_of("aeiou");
    std::cout << "First vowel at: " << pos << " ('" << text[pos] << "')\n";
    
    // find_last_of
    pos = text.find_last_of("!");
    std::cout << "Last '!' at: " << pos << '\n';
}

void cpp20_features() {
    std::cout << "\n=== C++20 Features ===\n";
    
    std::string text = "hello_world.cpp";
    
    // starts_with (C++20)
    if (text.starts_with("hello")) {
        std::cout << "Starts with 'hello'\n";
    }
    
    // ends_with (C++20)
    if (text.ends_with(".cpp")) {
        std::cout << "Is a C++ file\n";
    }
    
    #ifdef __cpp_lib_string_contains
    // contains (C++23)
    if (text.contains("world")) {
        std::cout << "Contains 'world'\n";
    }
    #endif
}

void demo() {
    std::cout << "=== std::string Basics ===\n";
    construction();
    capacity_management();
    search_operations();
    cpp20_features();
}

} // namespace string_basics

// ============================================
// 📌 std::string_view (C++17)
// ============================================

namespace string_view_examples {

// ❌ Плохо - копирование
void process_bad(const std::string& str) {
    std::cout << "Length: " << str.size() << '\n';
}

// ✅ Хорошо - никаких копий
void process_good(std::string_view str) {
    std::cout << "Length: " << str.size() << '\n';
}

void performance_demo() {
    std::cout << "\n=== string_view Performance ===\n";
    
    const char* cstr = "Hello World";
    
    // string_view - без аллокации!
    std::string_view sv = cstr;
    
    // substr тоже дешево
    auto sub = sv.substr(0, 5);  // Не копирует!
    std::cout << "Substring: " << sub << '\n';
}

void lifetime_issues() {
    std::cout << "\n=== Lifetime Issues ===\n";
    
    std::string_view dangerous;
    
    {
        std::string temp = "Temporary";
        dangerous = temp;  // ❌ Опасно!
        std::cout << "Inside scope: " << dangerous << '\n';
    }  // temp уничтожен!
    
    // std::cout << dangerous << '\n';  // ❌ Undefined behavior!
    
    // ✅ Безопасно - литералы живут всю программу
    std::string_view safe = "String literal";
    std::cout << "Safe: " << safe << '\n';
}

void use_cases() {
    std::cout << "\n=== Use Cases ===\n";
    
    // Парсинг без копирования
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
    
    // Токенизация
    std::string_view text = "one two three";
    size_t start = 0;
    size_t end;
    
    std::cout << "Tokens: ";
    while ((end = text.find(' ', start)) != std::string_view::npos) {
        std::cout << text.substr(start, end - start) << ' ';
        start = end + 1;
    }
    std::cout << text.substr(start) << '\n';
}

void demo() {
    std::cout << "\n=== string_view Examples ===\n";
    performance_demo();
    lifetime_issues();
    use_cases();
}

} // namespace string_view_examples

// ============================================
// 📌 STRING FORMATTING (C++20)
// ============================================

namespace string_formatting {

void format_basics() {
    std::cout << "\n=== std::format Basics ===\n";
    
    #ifdef __cpp_lib_format
    // Базовое форматирование
    auto s1 = std::format("Hello, {}!", "World");
    std::cout << s1 << '\n';
    
    // Позиционные аргументы
    auto s2 = std::format("{0} {1} {0}", "Hello", "World");
    std::cout << s2 << '\n';
    
    // Именованные аргументы (через структуры в C++23)
    int age = 30;
    auto s3 = std::format("Age: {}", age);
    std::cout << s3 << '\n';
    
    // Выравнивание
    std::cout << std::format("|{:<10}|", "left") << '\n';    // Влево
    std::cout << std::format("|{:>10}|", "right") << '\n';   // Вправо
    std::cout << std::format("|{:^10}|", "center") << '\n';  // По центру
    
    // Числа
    std::cout << std::format("Dec: {}, Hex: {:x}, Oct: {:o}", 42, 42, 42) << '\n';
    std::cout << std::format("Float: {:.2f}", 3.14159) << '\n';
    
    // Заполнение
    std::cout << std::format("{:*>10}", "test") << '\n';  // ******test
    #else
    std::cout << "std::format not available (C++20)\n";
    
    // Fallback на старый способ
    std::ostringstream oss;
    oss << "Hello, " << "World" << "!";
    std::cout << oss.str() << '\n';
    #endif
}

void custom_formatter() {
    std::cout << "\n=== Custom Formatter ===\n";
    
    #ifdef __cpp_lib_format
    struct Point {
        int x, y;
    };
    
    // Можно реализовать std::formatter<Point>, но это сложно
    // Простой вариант - использовать существующее форматирование
    Point p{10, 20};
    auto s = std::format("Point({}, {})", p.x, p.y);
    std::cout << s << '\n';
    #endif
}

void demo() {
    std::cout << "\n=== String Formatting ===\n";
    format_basics();
    custom_formatter();
}

} // namespace string_formatting

// ============================================
// 📌 STRING MANIPULATION
// ============================================

namespace string_manipulation {

// Case conversion
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

// Trimming
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

// Splitting
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

// Joining
std::string join(const std::vector<std::string>& parts, std::string_view separator) {
    if (parts.empty()) return "";
    
    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += separator;
        result += parts[i];
    }
    return result;
}

// Replace all
std::string replace_all(std::string str, std::string_view from, std::string_view to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
    return str;
}

void demo() {
    std::cout << "\n=== String Manipulation ===\n";
    
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
}

} // namespace string_manipulation

// ============================================
// 📌 REGULAR EXPRESSIONS
// ============================================

namespace regex_examples {

void basic_matching() {
    std::cout << "\n=== Regex Matching ===\n";
    
    std::string text = "hello@example.com";
    
    // regex_match - полное совпадение
    std::regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    
    if (std::regex_match(text, email_pattern)) {
        std::cout << "Valid email: " << text << '\n';
    }
    
    // regex_search - поиск в тексте
    std::string doc = "Contact us at support@example.com or sales@example.com";
    std::smatch match;
    
    if (std::regex_search(doc, match, email_pattern)) {
        std::cout << "Found email: " << match.str() << '\n';
    }
}

void capture_groups() {
    std::cout << "\n=== Capture Groups ===\n";
    
    std::string text = "2024-01-15";
    std::regex date_pattern(R"((\d{4})-(\d{2})-(\d{2}))");
    std::smatch match;
    
    if (std::regex_match(text, match, date_pattern)) {
        std::cout << "Full match: " << match[0] << '\n';
        std::cout << "Year: " << match[1] << '\n';
        std::cout << "Month: " << match[2] << '\n';
        std::cout << "Day: " << match[3] << '\n';
    }
}

void regex_replace_demo() {
    std::cout << "\n=== Regex Replace ===\n";
    
    std::string text = "Phone: 123-456-7890";
    std::regex phone_pattern(R"(\d{3}-\d{3}-\d{4})");
    
    // Замена
    std::string redacted = std::regex_replace(text, phone_pattern, "XXX-XXX-XXXX");
    std::cout << "Redacted: " << redacted << '\n';
    
    // Использование групп захвата
    text = "hello world";
    std::regex word_pattern(R"(\w+)");
    std::string capitalized = std::regex_replace(text, word_pattern, "[$&]");
    std::cout << "Brackets: " << capitalized << '\n';
}

void common_patterns() {
    std::cout << "\n=== Common Patterns ===\n";
    
    // Email
    std::regex email(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    
    // URL
    std::regex url(R"(https?://[^\s]+)");
    
    // Phone (US format)
    std::regex phone(R"(\d{3}-\d{3}-\d{4})");
    
    // IP address
    std::regex ip(R"((\d{1,3}\.){3}\d{1,3})");
    
    // Тестирование
    std::vector<std::pair<std::string, std::regex*>> tests = {
        {"test@example.com", &email},
        {"https://example.com", &url},
        {"123-456-7890", &phone},
        {"192.168.1.1", &ip}
    };
    
    for (const auto& [text, pattern] : tests) {
        std::cout << text << ": " 
                  << (std::regex_match(text, *pattern) ? "✓" : "✗") << '\n';
    }
}

void demo() {
    std::cout << "\n=== Regular Expressions ===\n";
    basic_matching();
    capture_groups();
    regex_replace_demo();
    common_patterns();
}

} // namespace regex_examples

// ============================================
// 📌 STRING PARSING
// ============================================

namespace string_parsing {

void old_school_parsing() {
    std::cout << "\n=== Old School Parsing ===\n";
    
    // std::stoi, stof, etc.
    try {
        int i = std::stoi("42");
        double d = std::stod("3.14");
        long l = std::stol("1000000");
        
        std::cout << "int: " << i << '\n';
        std::cout << "double: " << d << '\n';
        std::cout << "long: " << l << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Parse error: " << e.what() << '\n';
    }
}

void modern_parsing() {
    std::cout << "\n=== Modern Parsing (C++17) ===\n";
    
    // from_chars - быстрее и без исключений!
    const char* str = "12345";
    int value;
    
    auto [ptr, ec] = std::from_chars(str, str + std::strlen(str), value);
    
    if (ec == std::errc{}) {
        std::cout << "Parsed: " << value << '\n';
    } else {
        std::cout << "Parse error\n";
    }
    
    // to_chars - быстрая конвертация в строку
    char buffer[32];
    auto [end, ec2] = std::to_chars(buffer, buffer + sizeof(buffer), 42);
    
    if (ec2 == std::errc{}) {
        std::cout << "Formatted: " << std::string_view(buffer, end - buffer) << '\n';
    }
}

void csv_parsing() {
    std::cout << "\n=== CSV Parsing ===\n";
    
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
    
    std::string csv = "John,Doe,30,Engineer";
    auto fields = parse_csv_line(csv);
    
    std::cout << "CSV fields:\n";
    for (size_t i = 0; i < fields.size(); ++i) {
        std::cout << i << ": " << fields[i] << '\n';
    }
}

void demo() {
    std::cout << "\n=== String Parsing ===\n";
    old_school_parsing();
    modern_parsing();
    csv_parsing();
}

} // namespace string_parsing

// ============================================
// 📌 STRING LITERALS
// ============================================

namespace string_literals {

void raw_literals() {
    std::cout << "\n=== Raw String Literals ===\n";
    
    // Обычная строка - нужны escape
    std::string path1 = "C:\\Users\\John\\Documents";
    
    // Raw literal - не нужны escape!
    std::string path2 = R"(C:\Users\John\Documents)";
    
    std::cout << "Path: " << path2 << '\n';
    
    // Многострочные
    std::string json = R"({
    "name": "John",
    "age": 30
})";
    
    std::cout << "JSON:\n" << json << '\n';
    
    // Custom delimiter
    std::string complex = R"delimiter(Raw string with )" inside)delimiter";
    std::cout << "Complex: " << complex << '\n';
}

void user_defined_literals() {
    std::cout << "\n=== User-Defined Literals ===\n";
    
    using namespace std::string_literals;
    
    // ""s - создает std::string
    auto s1 = "Hello"s;  // std::string
    auto s2 = "World";   // const char*
    
    std::cout << "Type of s1: std::string\n";
    std::cout << "Type of s2: const char*\n";
    
    // ""sv - создает std::string_view
    using namespace std::string_view_literals;
    auto sv = "Hello"sv;  // std::string_view
}

void demo() {
    std::cout << "\n=== String Literals ===\n";
    raw_literals();
    user_defined_literals();
}

} // namespace string_literals

// ============================================
// 📌 PERFORMANCE OPTIMIZATION
// ============================================

namespace performance_optimization {

void avoid_copies() {
    std::cout << "\n=== Avoid Copies ===\n";
    
    // ❌ Создает копии
    std::string process_bad(std::string s) {
        return s + " processed";
    }
    
    // ✅ Использует move
    std::string process_good(std::string s) {
        s += " processed";
        return s;  // RVO
    }
    
    // ✅ string_view для readonly
    void analyze(std::string_view s) {
        std::cout << "Analyzing: " << s << '\n';
    }
    
    std::string data = "test";
    analyze(data);  // Никаких копий!
}

void reserve_optimization() {
    std::cout << "\n=== Reserve Optimization ===\n";
    
    // ❌ Множественные реаллокации
    std::string s1;
    for (int i = 0; i < 1000; ++i) {
        s1 += 'x';
    }
    
    // ✅ Одна аллокация
    std::string s2;
    s2.reserve(1000);
    for (int i = 0; i < 1000; ++i) {
        s2 += 'x';
    }
    
    std::cout << "Reserve prevents reallocations\n";
}

void move_semantics() {
    std::cout << "\n=== Move Semantics ===\n";
    
    std::string s1 = "Large string with lots of data...";
    
    // ❌ Копирование
    // std::string s2 = s1;
    
    // ✅ Перемещение
    std::string s2 = std::move(s1);
    
    std::cout << "s1 moved to s2\n";
    std::cout << "s1 is now: '" << s1 << "' (empty)\n";
    std::cout << "s2 is: '" << s2 << "'\n";
}

void demo() {
    std::cout << "\n=== Performance Optimization ===\n";
    avoid_copies();
    reserve_optimization();
    move_semantics();
}

} // namespace performance_optimization

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

int main() {
    std::cout << "=== C++ Strings & Text Processing ===\n";
    
    string_basics::demo();
    string_view_examples::demo();
    string_formatting::demo();
    string_manipulation::demo();
    regex_examples::demo();
    string_parsing::demo();
    string_literals::demo();
    performance_optimization::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ string_view для параметров (избегает копий)\n";
    std::cout << "✓ std::format (C++20) для форматирования\n";
    std::cout << "✓ from_chars/to_chars для парсинга\n";
    std::cout << "✓ Regex для сложных паттернов\n";
    std::cout << "✓ reserve() для оптимизации\n";
    std::cout << "✓ Move semantics для больших строк\n";
    std::cout << "✓ Raw literals R\"(...)\" для читаемости\n";
    
    return 0;
}
// • In-place modifications
// • Benchmarks