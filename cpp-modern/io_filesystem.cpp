/*
 * ============================================
 * I/O И FILESYSTEM В MODERN C++
 * ============================================
 * 
 * Полное руководство по файловому вводу-выводу:
 * - Standard streams (cin, cout, cerr)
 * - File I/O (fstream)
 * - std::format (C++20)
 * - std::filesystem (C++17)
 * - Serialization patterns
 * 
 * Требования: C++20 для format, C++17 для filesystem
 * Компиляция: g++ -std=c++20 io_filesystem.cpp
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <cstdint>

// C++20 format
#if __has_include(<format>)
#include <format>
#define HAS_FORMAT 1
#else
#define HAS_FORMAT 0
#endif

namespace fs = std::filesystem;

// ============================================
// 📌 STANDARD STREAMS
// ============================================

namespace standard_streams {

void basic_io() {
    std::cout << "=== Basic I/O ===\n";
    
    // Вывод в stdout
    std::cout << "Hello, World!\n";
    
    // Вывод в stderr (небуферизованный)
    std::cerr << "Error message\n";
    
    // Вывод в clog (буферизованный stderr)
    std::clog << "Log message\n";
    
    // Ввод
    std::string name;
    std::cout << "Enter name: ";
    // std::cin >> name;  // Читает до пробела
    
    // Чтение целой строки
    // std::getline(std::cin, name);
}

void stream_manipulators() {
    std::cout << "\n=== Stream Manipulators ===\n";
    
    int value = 42;
    
    // Числовые системы
    std::cout << "Decimal: " << std::dec << value << '\n';
    std::cout << "Hex: 0x" << std::hex << value << '\n';
    std::cout << "Octal: 0" << std::oct << value << '\n';
    std::cout << std::dec;  // Вернуть decimal
    
    // Форматирование чисел с плавающей точкой
    double pi = 3.14159265358979;
    
    std::cout << std::fixed << std::setprecision(2) << pi << '\n';      // 3.14
    std::cout << std::scientific << pi << '\n';                          // 3.14e+00
    std::cout << std::defaultfloat << pi << '\n';                        // 3.14159
    
    // Выравнивание
    std::cout << std::setw(10) << std::left << "Left" << "|\n";
    std::cout << std::setw(10) << std::right << "Right" << "|\n";
    std::cout << std::setw(10) << std::setfill('*') << 42 << "|\n";
    std::cout << std::setfill(' ');  // Сброс
    
    // Boolean
    bool flag = true;
    std::cout << std::boolalpha << flag << '\n';     // true
    std::cout << std::noboolalpha << flag << '\n';   // 1
}

void stream_states() {
    std::cout << "\n=== Stream States ===\n";
    
    std::istringstream iss("123 abc");
    
    int value;
    iss >> value;  // Прочитает 123
    
    std::cout << "Good: " << iss.good() << '\n';  // true
    std::cout << "EOF: " << iss.eof() << '\n';    // false
    std::cout << "Fail: " << iss.fail() << '\n';  // false
    
    // Попытка прочитать число из "abc"
    iss >> value;
    
    std::cout << "After failed read:\n";
    std::cout << "Good: " << iss.good() << '\n';  // false
    std::cout << "Fail: " << iss.fail() << '\n';  // true
    
    // Очистка состояния
    iss.clear();
    std::cout << "After clear: " << iss.fail() << '\n';  // false
}

// Кастомный манипулятор
std::ostream& bold(std::ostream& os) {
    return os << "\033[1m";
}

std::ostream& reset(std::ostream& os) {
    return os << "\033[0m";
}

void demo() {
    basic_io();
    stream_manipulators();
    stream_states();
    
    // Кастомный манипулятор
    std::cout << "\n" << bold << "Bold text" << reset << " normal\n";
}

} // namespace standard_streams

// ============================================
// 📌 FILE I/O
// ============================================

namespace file_io {

void write_text_file() {
    std::cout << "\n=== Writing Text File ===\n";
    
    // Запись текстового файла
    std::ofstream ofs("example.txt");
    
    if (!ofs) {
        std::cerr << "Failed to open file\n";
        return;
    }
    
    ofs << "Line 1\n";
    ofs << "Line 2\n";
    ofs << "Number: " << 42 << '\n';
    
    // Файл автоматически закроется в деструкторе
}

void read_text_file() {
    std::cout << "\n=== Reading Text File ===\n";
    
    std::ifstream ifs("example.txt");
    
    if (!ifs) {
        std::cerr << "Failed to open file\n";
        return;
    }
    
    // Чтение построчно
    std::string line;
    while (std::getline(ifs, line)) {
        std::cout << "Read: " << line << '\n';
    }
}

void binary_io() {
    std::cout << "\n=== Binary I/O ===\n";
    
    struct Data {
        int id;
        double value;
        char name[32];
    };
    
    // Запись бинарных данных
    {
        Data data{42, 3.14, "Test"};
        
        std::ofstream ofs("data.bin", std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(&data), sizeof(data));
    }
    
    // Чтение бинарных данных
    {
        Data data{};
        
        std::ifstream ifs("data.bin", std::ios::binary);
        ifs.read(reinterpret_cast<char*>(&data), sizeof(data));
        
        std::cout << "ID: " << data.id << '\n';
        std::cout << "Value: " << data.value << '\n';
        std::cout << "Name: " << data.name << '\n';
    }
}

void file_modes() {
    std::cout << "\n=== File Open Modes ===\n";
    
    // std::ios::in     - Открыть для чтения
    // std::ios::out    - Открыть для записи
    // std::ios::app    - Append (добавить в конец)
    // std::ios::ate    - Открыть и переместить в конец
    // std::ios::trunc  - Очистить файл при открытии
    // std::ios::binary - Бинарный режим
    
    // Добавление в конец файла
    std::ofstream ofs("example.txt", std::ios::app);
    ofs << "Appended line\n";
    
    // Чтение и запись
    std::fstream fs("data.txt", std::ios::in | std::ios::out);
    
    // Позиционирование
    fs.seekg(0, std::ios::beg);    // В начало для чтения
    fs.seekp(0, std::ios::end);    // В конец для записи
    
    auto read_pos = fs.tellg();    // Текущая позиция чтения
    auto write_pos = fs.tellp();   // Текущая позиция записи
}

// RAII wrapper для файла
class FileGuard {
    std::ofstream file_;
    
public:
    explicit FileGuard(const std::string& filename)
        : file_(filename) {
        if (!file_) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
    }
    
    std::ofstream& get() { return file_; }
    
    ~FileGuard() {
        if (file_.is_open()) {
            file_.flush();  // Гарантировать запись
            file_.close();
        }
    }
};

void demo() {
    write_text_file();
    read_text_file();
    binary_io();
    file_modes();
}

} // namespace file_io

// ============================================
// 📌 STRING STREAMS
// ============================================

namespace string_streams {

void output_string_stream() {
    std::cout << "\n=== Output String Stream ===\n";
    
    std::ostringstream oss;
    
    oss << "Value: " << 42 << ", ";
    oss << "Pi: " << std::fixed << std::setprecision(2) << 3.14159;
    
    std::string result = oss.str();
    std::cout << result << '\n';
}

void input_string_stream() {
    std::cout << "\n=== Input String Stream ===\n";
    
    std::string data = "42 3.14 hello";
    std::istringstream iss(data);
    
    int i;
    double d;
    std::string s;
    
    iss >> i >> d >> s;
    
    std::cout << "Int: " << i << '\n';
    std::cout << "Double: " << d << '\n';
    std::cout << "String: " << s << '\n';
}

void parsing_csv() {
    std::cout << "\n=== Parsing CSV ===\n";
    
    std::string csv = "Alice,30,Engineer\nBob,25,Designer";
    std::istringstream iss(csv);
    
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream line_stream(line);
        std::string name, role;
        int age;
        
        std::getline(line_stream, name, ',');
        line_stream >> age;
        line_stream.ignore();  // Пропустить запятую
        std::getline(line_stream, role);
        
        std::cout << name << " (" << age << ") - " << role << '\n';
    }
}

void demo() {
    output_string_stream();
    input_string_stream();
    parsing_csv();
}

} // namespace string_streams

// ============================================
// 📌 FORMATTING (C++20)
// ============================================

namespace formatting {

void basic_format() {
    #if HAS_FORMAT
    std::cout << "\n=== std::format (C++20) ===\n";
    
    // Базовое использование
    std::string msg = std::format("Hello, {}!", "World");
    std::cout << msg << '\n';
    
    // Позиционные аргументы
    auto s = std::format("{1} {0}", "World", "Hello");
    std::cout << s << '\n';  // Hello World
    
    // Повторное использование
    s = std::format("{0} {0} {0}", "Hi");
    std::cout << s << '\n';  // Hi Hi Hi
    
    #endif
}

void format_specifications() {
    #if HAS_FORMAT
    std::cout << "\n=== Format Specifications ===\n";
    
    int value = 42;
    
    // Ширина и выравнивание
    std::cout << std::format("|{:<10}|", value) << '\n';  // Влево
    std::cout << std::format("|{:>10}|", value) << '\n';  // Вправо
    std::cout << std::format("|{:^10}|", value) << '\n';  // По центру
    
    // Заполнение
    std::cout << std::format("|{:*<10}|", value) << '\n';  // 42********
    
    // Системы счисления
    std::cout << std::format("Hex: {:x}", value) << '\n';
    std::cout << std::format("Hex: {:#x}", value) << '\n';  // С префиксом 0x
    std::cout << std::format("Binary: {:b}", value) << '\n';
    
    // Числа с плавающей точкой
    double pi = 3.14159265;
    std::cout << std::format("Pi: {:.2f}", pi) << '\n';        // 3.14
    std::cout << std::format("Pi: {:.5f}", pi) << '\n';        // 3.14159
    std::cout << std::format("Pi: {:e}", pi) << '\n';          // 3.141593e+00
    
    // Знак
    std::cout << std::format("{:+}", 42) << '\n';   // +42
    std::cout << std::format("{:+}", -42) << '\n';  // -42
    
    #endif
}

// Кастомное форматирование для пользовательского типа
struct Point {
    int x, y;
};

#if HAS_FORMAT
template<>
struct std::formatter<Point> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }
    
    auto format(const Point& p, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};
#endif

void custom_format() {
    #if HAS_FORMAT
    std::cout << "\n=== Custom Formatting ===\n";
    
    Point p{10, 20};
    std::cout << std::format("Point: {}", p) << '\n';
    #endif
}

void demo() {
    #if HAS_FORMAT
    basic_format();
    format_specifications();
    custom_format();
    #else
    std::cout << "\nstd::format not available (requires C++20)\n";
    #endif
}

} // namespace formatting

// ============================================
// 📌 std::filesystem (C++17)
// ============================================

namespace filesystem_ops {

void path_operations() {
    std::cout << "\n=== Path Operations ===\n";
    
    fs::path p = "/home/user/documents/file.txt";
    
    std::cout << "Full path: " << p << '\n';
    std::cout << "Filename: " << p.filename() << '\n';           // file.txt
    std::cout << "Extension: " << p.extension() << '\n';         // .txt
    std::cout << "Stem: " << p.stem() << '\n';                   // file
    std::cout << "Parent: " << p.parent_path() << '\n';          // /home/user/documents
    std::cout << "Root: " << p.root_path() << '\n';              // /
    
    // Построение путей
    fs::path dir = "/tmp";
    fs::path filename = "test.txt";
    fs::path full = dir / filename;  // /tmp/test.txt
    std::cout << "Combined: " << full << '\n';
    
    // Изменение расширения
    fs::path p2 = "file.txt";
    p2.replace_extension(".md");
    std::cout << "New extension: " << p2 << '\n';  // file.md
}

void directory_operations() {
    std::cout << "\n=== Directory Operations ===\n";
    
    fs::path test_dir = "test_directory";
    
    // Создание директории
    if (fs::create_directory(test_dir)) {
        std::cout << "Directory created\n";
    }
    
    // Создание вложенных директорий
    fs::create_directories(test_dir / "sub1" / "sub2");
    
    // Проверка существования
    if (fs::exists(test_dir)) {
        std::cout << "Directory exists\n";
    }
    
    // Проверка типа
    if (fs::is_directory(test_dir)) {
        std::cout << "Is a directory\n";
    }
    
    // Создание файла для теста
    std::ofstream(test_dir / "file.txt") << "test";
    
    // Итерация по директории
    std::cout << "\nDirectory contents:\n";
    for (const auto& entry : fs::directory_iterator(test_dir)) {
        std::cout << "  " << entry.path().filename();
        if (entry.is_directory()) {
            std::cout << " [DIR]";
        }
        std::cout << '\n';
    }
    
    // Рекурсивная итерация
    std::cout << "\nRecursive contents:\n";
    for (const auto& entry : fs::recursive_directory_iterator(test_dir)) {
        std::cout << "  " << entry.path() << '\n';
    }
    
    // Удаление
    fs::remove_all(test_dir);
    std::cout << "Directory removed\n";
}

void file_operations() {
    std::cout << "\n=== File Operations ===\n";
    
    fs::path source = "source.txt";
    fs::path dest = "destination.txt";
    
    // Создание файла
    std::ofstream(source) << "Test content";
    
    // Копирование
    fs::copy_file(source, dest, fs::copy_options::overwrite_existing);
    std::cout << "File copied\n";
    
    // Размер файла
    auto size = fs::file_size(source);
    std::cout << "File size: " << size << " bytes\n";
    
    // Время последней модификации
    auto ftime = fs::last_write_time(source);
    std::cout << "Last write time: " 
              << ftime.time_since_epoch().count() << '\n';
    
    // Переименование
    fs::rename(dest, "renamed.txt");
    
    // Проверка типа
    if (fs::is_regular_file(source)) {
        std::cout << "Is a regular file\n";
    }
    
    // Очистка
    fs::remove(source);
    fs::remove("renamed.txt");
}

void space_info() {
    std::cout << "\n=== Disk Space Info ===\n";
    
    try {
        fs::space_info si = fs::space(".");
        
        std::cout << "Capacity: " << si.capacity / (1024 * 1024) << " MB\n";
        std::cout << "Free: " << si.free / (1024 * 1024) << " MB\n";
        std::cout << "Available: " << si.available / (1024 * 1024) << " MB\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

void current_path_ops() {
    std::cout << "\n=== Current Path ===\n";
    
    // Получить текущую директорию
    auto cwd = fs::current_path();
    std::cout << "Current directory: " << cwd << '\n';
    
    // Изменить (осторожно!)
    // fs::current_path("/tmp");
}

void demo() {
    path_operations();
    directory_operations();
    file_operations();
    space_info();
    current_path_ops();
}

} // namespace filesystem_ops

// ============================================
// 📌 SERIALIZATION
// ============================================

namespace serialization {

// Простая структура для сериализации
struct Person {
    std::string name;
    int age;
    double salary;
};

// Бинарная сериализация (простая, но не переносимая)
void binary_serialization() {
    std::cout << "\n=== Binary Serialization ===\n";
    
    Person p{"Alice", 30, 50000.0};
    
    // Запись
    {
        std::ofstream ofs("person.bin", std::ios::binary);
        
        // Записываем длину строки и саму строку
        size_t name_len = p.name.size();
        ofs.write(reinterpret_cast<const char*>(&name_len), sizeof(name_len));
        ofs.write(p.name.data(), name_len);
        
        // Записываем POD типы
        ofs.write(reinterpret_cast<const char*>(&p.age), sizeof(p.age));
        ofs.write(reinterpret_cast<const char*>(&p.salary), sizeof(p.salary));
    }
    
    // Чтение
    {
        std::ifstream ifs("person.bin", std::ios::binary);
        
        Person loaded;
        
        size_t name_len;
        ifs.read(reinterpret_cast<char*>(&name_len), sizeof(name_len));
        loaded.name.resize(name_len);
        ifs.read(loaded.name.data(), name_len);
        
        ifs.read(reinterpret_cast<char*>(&loaded.age), sizeof(loaded.age));
        ifs.read(reinterpret_cast<char*>(&loaded.salary), sizeof(loaded.salary));
        
        std::cout << "Loaded: " << loaded.name << ", " 
                  << loaded.age << ", " << loaded.salary << '\n';
    }
    
    fs::remove("person.bin");
}

// Текстовая сериализация (простой CSV-like формат)
void text_serialization() {
    std::cout << "\n=== Text Serialization ===\n";
    
    std::vector<Person> people = {
        {"Alice", 30, 50000.0},
        {"Bob", 25, 45000.0}
    };
    
    // Запись
    {
        std::ofstream ofs("people.txt");
        for (const auto& p : people) {
            ofs << p.name << ',' << p.age << ',' << p.salary << '\n';
        }
    }
    
    // Чтение
    {
        std::ifstream ifs("people.txt");
        std::vector<Person> loaded;
        
        std::string line;
        while (std::getline(ifs, line)) {
            std::istringstream iss(line);
            Person p;
            
            std::getline(iss, p.name, ',');
            iss >> p.age;
            iss.ignore();  // Запятая
            iss >> p.salary;
            
            loaded.push_back(p);
        }
        
        std::cout << "Loaded " << loaded.size() << " people\n";
    }
    
    fs::remove("people.txt");
}

// Простой JSON-like формат (для демонстрации)
std::string to_json(const Person& p) {
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"name\": \"" << p.name << "\",\n";
    oss << "  \"age\": " << p.age << ",\n";
    oss << "  \"salary\": " << p.salary << "\n";
    oss << "}";
    return oss.str();
}

void demo() {
    binary_serialization();
    text_serialization();
    
    Person p{"Charlie", 35, 60000.0};
    std::cout << "\nJSON-like:\n" << to_json(p) << '\n';
}

} // namespace serialization

// ============================================
// 📌 BEST PRACTICES
// ============================================

namespace best_practices {

/*
 * РЕКОМЕНДАЦИИ ПО I/O:
 * 
 * 1. ИСПОЛЬЗУЙТЕ RAII
 *    - std::ifstream/ofstream закроются автоматически
 *    - Не нужно вручную вызывать close()
 * 
 * 2. ПРОВЕРЯЙТЕ ОШИБКИ
 *    if (!file) { /* handle error */ }
 * 
 * 3. std::filesystem ДЛЯ ПУТЕЙ
 *    - Кроссплатформенность
 *    - Безопасность путей
 *    - Богатый API
 * 
 * 4. БИНАРНЫЙ РЕЖИМ ДЛЯ БИНАРНЫХ ДАННЫХ
 *    std::ios::binary для точности
 * 
 * 5. БУФЕРИЗАЦИЯ
 *    - Используйте буферы для производительности
 *    - flush() только когда необходимо
 * 
 * 6. std::format ВМЕСТО iostream ДЛЯ ФОРМАТИРОВАНИЯ
 *    - Более читаемо
 *    - Безопаснее чем printf
 *    - Лучше производительность
 * 
 * 7. ИЗБЕГАЙТЕ endl (используйте \n)
 *    endl = \n + flush (медленнее)
 * 
 * 8. STRING STREAMS ДЛЯ ПАРСИНГА
 *    Удобно для разбора строк
 */

void demo() {
    std::cout << "\n=== I/O Best Practices ===\n";
    std::cout << "✓ Use RAII for file handles\n";
    std::cout << "✓ Check stream states\n";
    std::cout << "✓ Use std::filesystem for paths\n";
    std::cout << "✓ Binary mode for binary data\n";
    std::cout << "✓ std::format over iostream formatting\n";
    std::cout << "✓ Prefer \\n over std::endl\n";
}

} // namespace best_practices

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== Modern C++ I/O and Filesystem ===\n";
    
    standard_streams::demo();
    file_io::demo();
    string_streams::demo();
    formatting::demo();
    filesystem_ops::demo();
    serialization::demo();
    best_practices::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ Standard streams - cin, cout, cerr, manipulators\n";
    std::cout << "✓ File I/O - ifstream, ofstream, binary mode\n";
    std::cout << "✓ String streams - parsing and building strings\n";
    std::cout << "✓ std::format - modern string formatting (C++20)\n";
    std::cout << "✓ std::filesystem - path operations, directories, files\n";
    std::cout << "✓ Serialization - binary and text formats\n";
    std::cout << "✓ RAII - automatic resource management\n";
    
    return 0;
}