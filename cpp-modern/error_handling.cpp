/*
 * ============================================
 * ОБРАБОТКА ОШИБОК В MODERN C++
 * ============================================
 * 
 * Комплексное руководство по обработке ошибок:
 * - Исключения (exceptions)
 * - std::expected<T, E> (C++23)
 * - std::optional<T> (C++17)
 * - Error codes
 * - RAII паттерны
 * 
 * Требования: C++23 для std::expected
 * Компиляция: g++ -std=c++23 error_handling.cpp
 */

#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <optional>
#include <system_error>
#include <fstream>
#include <vector>
#include <functional>

// C++23 expected (если доступно)
#if __has_include(<expected>)
#include <expected>
#define HAS_EXPECTED 1
#else
#define HAS_EXPECTED 0
// Упрощенная реализация для демонстрации
namespace std {
    template<typename T, typename E>
    class expected {
        union {
            T value_;
            E error_;
        };
        bool has_value_;
        
    public:
        expected(T value) : value_(std::move(value)), has_value_(true) {}
        expected(E error) : error_(std::move(error)), has_value_(false) {}
        
        ~expected() {
            if (has_value_) {
                value_.~T();
            } else {
                error_.~E();
            }
        }
        
        bool has_value() const { return has_value_; }
        
        T& value() {
            if (!has_value_) throw std::bad_expected_access<E>(error_);
            return value_;
        }
        
        const T& value() const {
            if (!has_value_) throw std::bad_expected_access<E>(error_);
            return value_;
        }
        
        E& error() { return error_; }
        const E& error() const { return error_; }
        
        T value_or(T default_value) const {
            return has_value_ ? value_ : default_value;
        }
        
        explicit operator bool() const { return has_value_; }
    };
}
#endif

// ============================================
// 📌 EXCEPTION BASICS
// ============================================

namespace exception_basics {

// Стандартная иерархия исключений:
// std::exception
//   ├─ std::logic_error
//   │    ├─ std::invalid_argument
//   │    ├─ std::domain_error
//   │    ├─ std::length_error
//   │    ├─ std::out_of_range
//   │    └─ std::future_error
//   └─ std::runtime_error
//        ├─ std::range_error
//        ├─ std::overflow_error
//        ├─ std::underflow_error
//        └─ std::system_error

// Кастомное исключение
class DatabaseError : public std::runtime_error {
    int error_code_;
    
public:
    DatabaseError(const std::string& message, int code)
        : std::runtime_error(message), error_code_(code) {}
    
    int code() const { return error_code_; }
};

void basic_throw_catch() {
    std::cout << "=== Basic Try-Catch ===\n";
    
    try {
        // Бросаем исключение
        throw std::runtime_error("Something went wrong");
    } catch (const std::exception& e) {  // ✅ Ловим по const reference
        std::cout << "Caught: " << e.what() << '\n';
    }
}

void multiple_catch_blocks() {
    std::cout << "\n=== Multiple Catch Blocks ===\n";
    
    auto test_exception = [](int type) {
        try {
            if (type == 1) {
                throw std::invalid_argument("Invalid argument");
            } else if (type == 2) {
                throw std::runtime_error("Runtime error");
            } else if (type == 3) {
                throw DatabaseError("DB connection failed", 1001);
            } else {
                throw 42;  // Не рекомендуется бросать не-exception типы
            }
        } catch (const DatabaseError& e) {
            // Специфичное исключение ловится первым
            std::cout << "DB Error [" << e.code() << "]: " << e.what() << '\n';
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid arg: " << e.what() << '\n';
        } catch (const std::exception& e) {
            // Базовый класс ловится последним
            std::cout << "Exception: " << e.what() << '\n';
        } catch (...) {
            // Ловим всё остальное
            std::cout << "Unknown exception\n";
        }
    };
    
    test_exception(1);
    test_exception(2);
    test_exception(3);
    test_exception(4);
}

void rethrowing_exceptions() {
    std::cout << "\n=== Rethrowing ===\n";
    
    auto inner = []() {
        throw std::runtime_error("Inner error");
    };
    
    auto outer = [&]() {
        try {
            inner();
        } catch (const std::exception& e) {
            std::cout << "Caught in outer: " << e.what() << '\n';
            
            // Перебрасываем то же исключение
            throw;  // ✅ Правильно: сохраняет тип
            
            // throw e;  // ❌ Неправильно: может срезать тип (slicing)
        }
    };
    
    try {
        outer();
    } catch (const std::exception& e) {
        std::cout << "Finally caught: " << e.what() << '\n';
    }
}

void demo() {
    basic_throw_catch();
    multiple_catch_blocks();
    rethrowing_exceptions();
}

} // namespace exception_basics

// ============================================
// 📌 EXCEPTION SAFETY GUARANTEES
// ============================================

namespace exception_safety {

/*
 * ЧЕТЫРЕ УРОВНЯ EXCEPTION SAFETY:
 * 
 * 1. NO-THROW GUARANTEE (noexcept)
 *    - Операция никогда не бросает исключения
 *    - Пример: деструкторы, move операции, swap
 * 
 * 2. STRONG EXCEPTION SAFETY
 *    - Либо операция успешна, либо состояние не изменено
 *    - Транзакционная семантика
 *    - Пример: copy-and-swap idiom
 * 
 * 3. BASIC EXCEPTION SAFETY
 *    - Инварианты сохранены, ресурсы не утекают
 *    - Но состояние может измениться
 * 
 * 4. NO EXCEPTION SAFETY
 *    - Никаких гарантий (плохо!)
 */

class Widget {
    std::string name_;
    std::vector<int> data_;
    
public:
    // ✅ Strong exception safety через copy-and-swap
    Widget& operator=(const Widget& other) {
        Widget temp(other);  // Может бросить, но this не тронут
        swap(temp);          // noexcept
        return *this;
    }
    
    // ✅ No-throw guarantee для move
    Widget& operator=(Widget&& other) noexcept {
        name_ = std::move(other.name_);
        data_ = std::move(other.data_);
        return *this;
    }
    
    void swap(Widget& other) noexcept {
        name_.swap(other.name_);
        data_.swap(other.data_);
    }
    
    // ✅ Basic exception safety
    void add_element(int value) {
        data_.push_back(value);  // Может бросить
        // Но data_ остается в валидном состоянии
    }
    
    // ✅ noexcept для деструктора (по умолчанию)
    ~Widget() noexcept {
        // Деструкторы НИКОГДА не должны бросать исключения
    }
};

// Conditional noexcept
template<typename T>
class Container {
    T value_;
    
public:
    // noexcept если T::T(T&&) noexcept
    Container(Container&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        : value_(std::move(other.value_)) {}
    
    // Проверка noexcept-свойств
    void swap(Container& other) noexcept(noexcept(std::swap(value_, other.value_))) {
        std::swap(value_, other.value_);
    }
};

void demo_noexcept() {
    std::cout << "\n=== noexcept Examples ===\n";
    
    // noexcept operator - compile-time проверка
    std::cout << "Widget move is noexcept: " 
              << std::boolalpha 
              << noexcept(Widget(std::declval<Widget&&>())) << '\n';
    
    // Использование noexcept для оптимизаций
    std::vector<Widget> widgets;
    widgets.reserve(10);
    
    // Если move noexcept, std::vector будет использовать move при реаллокации
    // Иначе будет использовать copy для exception safety
}

void demo() {
    demo_noexcept();
}

} // namespace exception_safety

// ============================================
// 📌 RAII PATTERN
// ============================================

namespace raii_pattern {

// RAII = Resource Acquisition Is Initialization
// Ресурс захватывается в конструкторе, освобождается в деструкторе

// ❌ БЕЗ RAII
void bad_resource_management() {
    int* data = new int[100];
    
    // Что если здесь исключение?
    // Утечка памяти!
    
    delete[] data;
}

// ✅ С RAII
void good_resource_management() {
    auto data = std::make_unique<int[]>(100);
    
    // Исключение? Нет проблем - деструктор unique_ptr освободит память
}

// Кастомный RAII wrapper для FILE*
class FileHandle {
    FILE* file_;
    
public:
    explicit FileHandle(const char* filename, const char* mode)
        : file_(fopen(filename, mode)) {
        if (!file_) {
            throw std::runtime_error("Failed to open file");
        }
    }
    
    ~FileHandle() {
        if (file_) {
            fclose(file_);
        }
    }
    
    // Запрещаем копирование
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    
    // Разрешаем перемещение
    FileHandle(FileHandle&& other) noexcept
        : file_(std::exchange(other.file_, nullptr)) {}
    
    FILE* get() const { return file_; }
};

// Scope Guard - выполнить действие при выходе из области видимости
template<typename F>
class ScopeGuard {
    F cleanup_;
    bool active_;
    
public:
    explicit ScopeGuard(F cleanup)
        : cleanup_(std::move(cleanup)), active_(true) {}
    
    ~ScopeGuard() {
        if (active_) {
            cleanup_();
        }
    }
    
    void dismiss() { active_ = false; }
    
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

// Helper для создания scope guard
template<typename F>
ScopeGuard<F> make_scope_guard(F cleanup) {
    return ScopeGuard<F>(std::move(cleanup));
}

void demo_scope_guard() {
    std::cout << "\n=== Scope Guard ===\n";
    
    bool cleanup_called = false;
    
    {
        auto guard = make_scope_guard([&cleanup_called]() {
            std::cout << "Cleanup executed\n";
            cleanup_called = true;
        });
        
        std::cout << "Doing work...\n";
        
        // Деструктор guard вызовется здесь
    }
    
    std::cout << "Cleanup called: " << std::boolalpha << cleanup_called << '\n';
}

// RAII для транзакций
class Transaction {
    bool committed_ = false;
    
    void rollback() {
        std::cout << "Rolling back transaction\n";
    }
    
public:
    void commit() {
        std::cout << "Committing transaction\n";
        committed_ = true;
    }
    
    ~Transaction() {
        if (!committed_) {
            rollback();
        }
    }
};

void demo_transaction() {
    std::cout << "\n=== Transaction RAII ===\n";
    
    try {
        Transaction txn;
        
        std::cout << "Doing database work...\n";
        
        // Симуляция ошибки
        throw std::runtime_error("DB error");
        
        txn.commit();  // Не будет вызван
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
        // Деструктор Transaction вызовет rollback
    }
}

void demo() {
    demo_scope_guard();
    demo_transaction();
}

} // namespace raii_pattern

// ============================================
// 📌 std::optional<T> (C++17)
// ============================================

namespace optional_handling {

// optional представляет значение, которое может отсутствовать

std::optional<int> parse_int(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (...) {
        return std::nullopt;  // Нет значения
    }
}

std::optional<std::string> find_user(int id) {
    if (id == 42) {
        return "Alice";
    }
    return std::nullopt;  // Пользователь не найден
}

void basic_optional() {
    std::cout << "\n=== Basic Optional ===\n";
    
    auto result = parse_int("123");
    
    // Проверка наличия значения
    if (result.has_value()) {
        std::cout << "Parsed: " << result.value() << '\n';
    }
    
    // Оператор bool
    if (result) {
        std::cout << "Has value: " << *result << '\n';  // Оператор *
    }
    
    // value_or - значение или дефолт
    auto bad = parse_int("not a number");
    std::cout << "Value or default: " << bad.value_or(-1) << '\n';
}

// C++23 monadic operations (если доступны)
void monadic_optional() {
    std::cout << "\n=== Monadic Optional ===\n";
    
    auto user = find_user(42);
    
    // and_then - применить функцию если есть значение
    #if __cpp_lib_optional >= 202110L
    auto length = user.and_then([](const std::string& name) -> std::optional<size_t> {
        return name.length();
    });
    #endif
    
    // transform - преобразовать значение
    #if __cpp_lib_optional >= 202110L
    auto upper = user.transform([](const std::string& name) {
        std::string result = name;
        for (auto& c : result) c = std::toupper(c);
        return result;
    });
    
    if (upper) {
        std::cout << "Uppercase: " << *upper << '\n';
    }
    #endif
    
    // Цепочка операций
    auto result = find_user(42)
        .and_then([](const std::string& name) -> std::optional<int> {
            return name.length();
        })
        .and_then([](int len) -> std::optional<int> {
            return len > 3 ? std::optional<int>(len * 2) : std::nullopt;
        });
}

void demo() {
    basic_optional();
    monadic_optional();
}

} // namespace optional_handling

// ============================================
// 📌 std::expected<T, E> (C++23)
// ============================================

namespace expected_handling {

enum class ParseError {
    InvalidFormat,
    OutOfRange,
    Empty
};

std::string to_string(ParseError err) {
    switch (err) {
        case ParseError::InvalidFormat: return "Invalid format";
        case ParseError::OutOfRange: return "Out of range";
        case ParseError::Empty: return "Empty string";
    }
    return "Unknown";
}

std::expected<int, ParseError> parse_number(const std::string& str) {
    if (str.empty()) {
        return std::unexpected(ParseError::Empty);
    }
    
    try {
        int value = std::stoi(str);
        if (value < 0 || value > 100) {
            return std::unexpected(ParseError::OutOfRange);
        }
        return value;
    } catch (...) {
        return std::unexpected(ParseError::InvalidFormat);
    }
}

void basic_expected() {
    std::cout << "\n=== Basic Expected ===\n";
    
    auto result = parse_number("42");
    
    if (result) {  // operator bool
        std::cout << "Success: " << *result << '\n';
    } else {
        std::cout << "Error: " << to_string(result.error()) << '\n';
    }
    
    // value_or
    auto bad = parse_number("999");
    std::cout << "Value or default: " << bad.value_or(0) << '\n';
}

// Monadic operations
std::expected<int, ParseError> double_value(int x) {
    return x * 2;
}

std::expected<std::string, ParseError> to_string_safe(int x) {
    return std::to_string(x);
}

void monadic_expected() {
    std::cout << "\n=== Monadic Expected ===\n";
    
    // and_then - цепочка операций, останавливается при первой ошибке
    #if HAS_EXPECTED
    auto result = parse_number("10")
        .and_then(double_value)
        .and_then(to_string_safe);
    
    if (result) {
        std::cout << "Result: " << *result << '\n';
    }
    #endif
    
    // or_else - обработка ошибки
    auto recovery = parse_number("invalid")
        .or_else([](ParseError err) -> std::expected<int, ParseError> {
            std::cout << "Recovering from: " << to_string(err) << '\n';
            return 0;  // Дефолтное значение
        });
}

// Сравнение с исключениями
int parse_with_exception(const std::string& str) {
    // Может бросить исключение
    return std::stoi(str);
}

std::expected<int, std::string> parse_with_expected(const std::string& str) {
    // Возвращает ошибку
    try {
        return std::stoi(str);
    } catch (const std::exception& e) {
        return std::unexpected(std::string(e.what()));
    }
}

void compare_approaches() {
    std::cout << "\n=== Exceptions vs Expected ===\n";
    
    // С исключениями - может быть медленнее при ошибках
    try {
        int val = parse_with_exception("abc");
        std::cout << "Value: " << val << '\n';
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << '\n';
    }
    
    // С expected - предсказуемая производительность
    auto result = parse_with_expected("abc");
    if (!result) {
        std::cout << "Error: " << result.error() << '\n';
    }
}

/*
 * КОГДА ИСПОЛЬЗОВАТЬ ЧТО:
 * 
 * EXCEPTIONS:
 * ✓ Редкие ошибки
 * ✓ Глубокий call stack
 * ✓ Нужно прервать выполнение
 * ✓ Нет контроля над ошибками
 * 
 * EXPECTED:
 * ✓ Частые ошибки (часть нормального flow)
 * ✓ Предсказуемая производительность
 * ✓ Функциональный стиль
 * ✓ Явная обработка ошибок
 * 
 * OPTIONAL:
 * ✓ Отсутствие значения - норма
 * ✓ Нет причины ошибки
 * ✓ Nullable типы
 */

void demo() {
    basic_expected();
    monadic_expected();
    compare_approaches();
}

} // namespace expected_handling

// ============================================
// 📌 ERROR CODES
// ============================================

namespace error_codes {

// Кастомная категория ошибок
enum class MyError {
    Success = 0,
    FileNotFound,
    PermissionDenied,
    NetworkError
};

class MyErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override {
        return "MyError";
    }
    
    std::string message(int ev) const override {
        switch (static_cast<MyError>(ev)) {
            case MyError::Success: return "Success";
            case MyError::FileNotFound: return "File not found";
            case MyError::PermissionDenied: return "Permission denied";
            case MyError::NetworkError: return "Network error";
            default: return "Unknown error";
        }
    }
};

const MyErrorCategory& my_error_category() {
    static MyErrorCategory instance;
    return instance;
}

std::error_code make_error_code(MyError e) {
    return {static_cast<int>(e), my_error_category()};
}

void demo_error_code() {
    std::cout << "\n=== Error Codes ===\n";
    
    std::error_code ec = make_error_code(MyError::FileNotFound);
    
    std::cout << "Error: " << ec.message() << '\n';
    std::cout << "Category: " << ec.category().name() << '\n';
    std::cout << "Value: " << ec.value() << '\n';
    
    // Сравнение
    if (ec == make_error_code(MyError::FileNotFound)) {
        std::cout << "Is FileNotFound\n";
    }
    
    // Проверка на успех
    if (!ec) {
        std::cout << "Success\n";
    }
}

void demo() {
    demo_error_code();
}

} // namespace error_codes

// ============================================
// 📌 BEST PRACTICES
// ============================================

namespace best_practices {

/*
 * РЕКОМЕНДАЦИИ ПО ОБРАБОТКЕ ОШИБОК:
 * 
 * 1. ИСПОЛЬЗУЙТЕ RAII
 *    - Всегда управляйте ресурсами через RAII
 *    - Умные указатели, lock_guard, unique_lock
 *    - Кастомные RAII wrappers для специфичных ресурсов
 * 
 * 2. EXCEPTION SAFETY
 *    - Деструкторы всегда noexcept
 *    - Move операции желательно noexcept
 *    - Используйте copy-and-swap для strong guarantee
 * 
 * 3. ЛОВИТЕ ИСКЛЮЧЕНИЯ ПО CONST REFERENCE
 *    catch (const std::exception& e)  ✅
 *    catch (std::exception e)         ❌ (копирование + slicing)
 * 
 * 4. НЕ БРОСАЙТЕ ИСКЛЮЧЕНИЯ ИЗ:
 *    - Деструкторов
 *    - noexcept функций
 *    - Move конструкторов (по возможности)
 *    - swap функций
 * 
 * 5. ВЫБОР МЕХАНИЗМА:
 *    - Редкие ошибки → exceptions
 *    - Частые ошибки → expected
 *    - Nullable значения → optional
 *    - Системные ошибки → error_code
 * 
 * 6. ДОКУМЕНТИРУЙТЕ
 *    - Какие исключения может бросить функция
 *    - Exception safety guarantee
 *    - Условия для std::expected
 * 
 * 7. НЕ ИСПОЛЬЗУЙТЕ EXCEPTION SPECIFICATIONS (deprecated)
 *    void func() throw(std::runtime_error);  ❌ Deprecated
 *    void func() noexcept;                    ✅ Используйте только noexcept
 */

// Пример хорошей функции с обработкой ошибок
class DataProcessor {
public:
    // ✅ Документирована, может бросить, strong exception safety
    std::vector<int> process(const std::vector<int>& input) {
        std::vector<int> result;
        result.reserve(input.size());  // Strong guarantee через reserve
        
        for (int val : input) {
            if (val < 0) {
                throw std::invalid_argument("Negative values not allowed");
            }
            result.push_back(val * 2);
        }
        
        return result;  // NRVO
    }
    
    // ✅ С expected - без исключений
    std::expected<std::vector<int>, std::string> 
    process_safe(const std::vector<int>& input) {
        std::vector<int> result;
        result.reserve(input.size());
        
        for (int val : input) {
            if (val < 0) {
                return std::unexpected("Negative values not allowed");
            }
            result.push_back(val * 2);
        }
        
        return result;
    }
    
    // ✅ Move noexcept
    DataProcessor(DataProcessor&&) noexcept = default;
    DataProcessor& operator=(DataProcessor&&) noexcept = default;
};

void demo() {
    std::cout << "\n=== Best Practices ===\n";
    
    DataProcessor processor;
    
    // С исключениями
    try {
        auto result = processor.process({1, 2, 3});
        std::cout << "Processed " << result.size() << " elements\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
    
    // С expected
    auto result = processor.process_safe({1, -2, 3});
    if (!result) {
        std::cout << "Error: " << result.error() << '\n';
    }
}

} // namespace best_practices

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== Modern C++ Error Handling ===\n\n";
    
    exception_basics::demo();
    exception_safety::demo();
    raii_pattern::demo();
    optional_handling::demo();
    expected_handling::demo();
    error_codes::demo();
    best_practices::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ Exceptions - для редких ошибок\n";
    std::cout << "✓ std::expected - для частых ошибок (C++23)\n";
    std::cout << "✓ std::optional - для nullable значений\n";
    std::cout << "✓ error_code - для системных ошибок\n";
    std::cout << "✓ RAII - всегда для управления ресурсами\n";
    std::cout << "✓ noexcept - для move операций и деструкторов\n";
    std::cout << "✓ Strong exception safety - через copy-and-swap\n";
    
    return 0;
}
// • Error handling philosophy
// • Fail-fast vs graceful degradation
// • Logging и monitoring
// • Testing error paths
// • Documentation
