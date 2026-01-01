/*
 * ============================================
 * АНТИПАТТЕРНЫ C++ (Чего НЕ делать)
 * ============================================
 * 
 * Распространенные ошибки и плохие практики в C++
 * с объяснениями почему это плохо и как делать правильно.
 * 
 * Требования: C++20 или выше
 * Компиляция: g++ -std=c++20 -Wall -Wextra anti_patterns.cpp
 */

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include <thread>
#include <optional>
#include <algorithm>
#include <fstream>

// ============================================
// 📌 MEMORY MANAGEMENT ANTI-PATTERNS
// ============================================

namespace memory_antipatterns {

// ❌ ПЛОХО: Ручное управление памятью с new/delete
class BadResourceManager {
private:
    int* data_;
    
public:
    BadResourceManager(int size) {
        data_ = new int[size];  // Что если выброситься исключение?
    }
    
    ~BadResourceManager() {
        delete[] data_;  // Что если забудем?
    }
    
    // Проблемы:
    // 1. Нет copy constructor/assignment
    // 2. Утечка памяти при исключении в конструкторе
    // 3. Double delete при копировании
    // 4. Нужно помнить про rule of five
};

// ✅ ХОРОШО: Используйте RAII и стандартные контейнеры
class GoodResourceManager {
private:
    std::vector<int> data_;  // Автоматическое управление памятью
    
public:
    GoodResourceManager(int size) : data_(size) {
        // Все безопасно, автоматически
    }
    
    // Компилятор генерирует:
    // - Copy constructor
    // - Move constructor
    // - Copy assignment
    // - Move assignment
    // - Destructor
};

// ❌ ПЛОХО: Смешивание new[] и delete
void bad_delete_mismatch() {
    int* arr = new int[10];
    delete arr;  // УБ! Должно быть delete[]
    
    int* single = new int(42);
    delete[] single;  // УБ! Должно быть delete
}

// ❌ ПЛОХО: Возврат raw pointer из new
int* bad_create_array(int size) {
    return new int[size];  // Кто отвечает за удаление?
}

// ✅ ХОРОШО: Возвращайте умные указатели или контейнеры
std::unique_ptr<int[]> good_create_array(int size) {
    return std::make_unique<int[]>(size);
}

std::vector<int> even_better_create_array(int size) {
    return std::vector<int>(size);  // Самый лучший вариант
}

// ❌ ПЛОХО: Хранение ссылок в контейнерах
void bad_storing_references() {
    int x = 10, y = 20, z = 30;
    // std::vector<int&> refs{x, y, z};  // НЕ КОМПИЛИРУЕТСЯ!
    
    // Но это компилируется и опасно:
    std::vector<int*> ptrs{&x, &y, &z};
    // Что если x, y, z выходят из scope?
}

// ✅ ХОРОШО: Используйте std::reference_wrapper
void good_storing_references() {
    int x = 10, y = 20, z = 30;
    std::vector<std::reference_wrapper<int>> refs{x, y, z};
    
    for (auto& ref : refs) {
        ref.get() += 10;
    }
    
    std::cout << "x = " << x << '\n';  // 20
}

// ❌ ПЛОХО: Забытый delete в исключительных ситуациях
void bad_exception_safety() {
    int* data = new int[100];
    
    // Если здесь бросится исключение, data утечет
    process_data(data);  
    
    delete[] data;  // Может не выполниться!
}

// ✅ ХОРОШО: RAII гарантирует очистку
void good_exception_safety() {
    auto data = std::make_unique<int[]>(100);
    
    // Даже если исключение, unique_ptr автоматически очистит память
    process_data(data.get());
    
    // delete не нужен
}

} // namespace memory_antipatterns

// ============================================
// 📌 EXCEPTION SAFETY ANTI-PATTERNS
// ============================================

namespace exception_antipatterns {

// ❌ ПЛОХО: Бросание исключений в деструкторе
class BadDestructor {
    ~BadDestructor() {
        // Если деструктор вызван во время раскрутки стека из-за исключения,
        // и здесь бросится еще одно - std::terminate()!
        throw std::runtime_error("Oops");  // НИКОГДА ТАК НЕ ДЕЛАЙТЕ!
    }
};

// ✅ ХОРОШО: Деструкторы должны быть noexcept
class GoodDestructor {
    ~GoodDestructor() noexcept {
        try {
            // Опасная операция
            cleanup();
        } catch (...) {
            // Логируем ошибку, но не бросаем дальше
            std::cerr << "Cleanup failed\n";
        }
    }
    
    void cleanup();
};

// ❌ ПЛОХО: catch(...) без rethrow может скрыть проблемы
void bad_catch_all() {
    try {
        dangerous_operation();
    } catch (...) {
        std::cout << "Something went wrong\n";
        // Исключение проглочено - вызывающий код не узнает об ошибке!
    }
}

// ✅ ХОРОШО: Обрабатывайте то, что можете, остальное пробрасывайте
void good_exception_handling() {
    try {
        dangerous_operation();
    } catch (const std::runtime_error& e) {
        // Обрабатываем то, что знаем как обработать
        std::cerr << "Runtime error: " << e.what() << '\n';
        // Можем восстановиться
    } catch (...) {
        // Логируем неизвестную ошибку
        std::cerr << "Unknown error\n";
        throw;  // И пробрасываем дальше!
    }
}

// ❌ ПЛОХО: Игнорирование exception safety guarantees
class BadVector {
    int* data_;
    size_t size_;
    
    void push_back(int value) {
        // Проблема: если new бросит исключение после delete,
        // мы потеряем старые данные!
        int* new_data = new int[size_ + 1];  // Может бросить
        delete[] data_;  // Старые данные потеряны!
        data_ = new_data;  // Если здесь исключение - утечка
        data_[size_] = value;
        ++size_;
    }
};

// ✅ ХОРОШО: Strong exception guarantee
class GoodVector {
    std::vector<int> data_;
    
    void push_back(int value) {
        // std::vector гарантирует strong exception safety:
        // либо операция успешна, либо состояние не изменено
        data_.push_back(value);
    }
};

// ❌ ПЛОХО: Исключения в noexcept функциях
void bad_noexcept() noexcept {
    throw std::runtime_error("Oops");  // std::terminate() будет вызван!
}

// ✅ ХОРОШО: Не помечайте noexcept, если функция может бросить
void good_function() {
    // Может бросить исключение - это нормально
    throw std::runtime_error("This is fine");
}

// Или обрабатывайте все внутри:
void good_noexcept() noexcept {
    try {
        dangerous_operation();
    } catch (...) {
        // Обработано внутри, наружу не выходит
    }
}

} // namespace exception_antipatterns

// ============================================
// 📌 CONCURRENCY ANTI-PATTERNS
// ============================================

namespace concurrency_antipatterns {

// ❌ ПЛОХО: Data race
class BadCounter {
    int count_ = 0;
    
public:
    void increment() {
        ++count_;  // НЕ АТОМАРНО! Race condition!
    }
    
    int get() const {
        return count_;  // Тоже может вернуть "разорванное" значение
    }
};

// ✅ ХОРОШО: Используйте атомарные операции или мьютексы
class GoodCounter {
    std::atomic<int> count_{0};
    
public:
    void increment() {
        ++count_;  // Атомарная операция
    }
    
    int get() const {
        return count_.load();
    }
};

class GoodCounterWithMutex {
    mutable std::mutex mutex_;
    int count_ = 0;
    
public:
    void increment() {
        std::lock_guard lock(mutex_);
        ++count_;
    }
    
    int get() const {
        std::lock_guard lock(mutex_);
        return count_;
    }
};

// ❌ ПЛОХО: Deadlock из-за неправильного порядка блокировки
class BadBankAccount {
    mutable std::mutex mutex_;
    double balance_;
    
public:
    void transfer(BadBankAccount& to, double amount) {
        std::lock_guard lock1(mutex_);        // Блокируем this
        std::lock_guard lock2(to.mutex_);     // Блокируем to
        // DEADLOCK если два потока вызывают:
        // A.transfer(B, 100) и B.transfer(A, 50)
        balance_ -= amount;
        to.balance_ += amount;
    }
};

// ✅ ХОРОШО: Используйте std::scoped_lock для множественных мьютексов
class GoodBankAccount {
    mutable std::mutex mutex_;
    double balance_;
    
public:
    void transfer(GoodBankAccount& to, double amount) {
        // std::scoped_lock блокирует оба мьютекса атомарно
        // в правильном порядке, избегая deadlock
        std::scoped_lock lock(mutex_, to.mutex_);
        balance_ -= amount;
        to.balance_ += amount;
    }
};

// ❌ ПЛОХО: Забытый unlock
void bad_manual_lock(std::mutex& m, int& shared_data) {
    m.lock();
    
    if (shared_data < 0) {
        return;  // УТЕЧКА БЛОКИРОВКИ! Забыли unlock!
    }
    
    ++shared_data;
    m.unlock();
}

// ✅ ХОРОШО: Всегда используйте RAII для блокировок
void good_raii_lock(std::mutex& m, int& shared_data) {
    std::lock_guard lock(m);
    
    if (shared_data < 0) {
        return;  // OK, lock автоматически разблокируется
    }
    
    ++shared_data;
    // Автоматический unlock при выходе из scope
}

// ❌ ПЛОХО: Livelock - потоки постоянно реагируют друг на друга
void bad_livelock_example() {
    /*
    std::atomic<bool> flag1{false};
    std::atomic<bool> flag2{false};
    
    // Поток 1:
    while (!flag2) {
        flag1 = true;
        // Если поток 2 тоже в такой петле с flag2 и flag1,
        // они могут бесконечно уступать друг другу
    }
    */
}

// ✅ ХОРОШО: Используйте правильные примитивы синхронизации
void good_synchronization() {
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    
    // Поток 1 - ждущий
    std::thread t1([&] {
        std::unique_lock lock(m);
        cv.wait(lock, [&] { return ready; });
        // Продолжаем когда ready == true
    });
    
    // Поток 2 - уведомляющий
    std::thread t2([&] {
        {
            std::lock_guard lock(m);
            ready = true;
        }
        cv.notify_one();
    });
    
    t1.join();
    t2.join();
}

} // namespace concurrency_antipatterns

// ============================================
// 📌 TEMPLATE ANTI-PATTERNS
// ============================================

namespace template_antipatterns {

// ❌ ПЛОХО: Template bloat - генерация кода для каждого типа
template<typename T>
class BadContainer {
    std::vector<T> data_;
    
    // Много кода, который не зависит от T
    void log_size() {
        std::cout << "Size: " << data_.size() << '\n';
    }
    
    void log_capacity() {
        std::cout << "Capacity: " << data_.capacity() << '\n';
    }
    
    // ... еще 100 функций, не зависящих от T
    // Весь этот код будет дублироваться для каждого T!
};

// ✅ ХОРОШО: Вынесите не зависящий от шаблона код в базовый класс
class ContainerBase {
protected:
    void log_size(size_t size) {
        std::cout << "Size: " << size << '\n';
    }
    
    void log_capacity(size_t capacity) {
        std::cout << "Capacity: " << capacity << '\n';
    }
    
    // Код компилируется только один раз!
};

template<typename T>
class GoodContainer : private ContainerBase {
    std::vector<T> data_;
    
public:
    void log_info() {
        log_size(data_.size());
        log_capacity(data_.capacity());
    }
};

// ❌ ПЛОХО: Непонятные ошибки компиляции без concepts
template<typename T>
T bad_add(T a, T b) {
    return a + b;  // Что если T не поддерживает operator+?
}

// Использование: bad_add(std::vector<int>{}, std::vector<int>{});
// Ошибка: страница непонятного текста про std::vector и operator+

// ✅ ХОРОШО: Используйте concepts для понятных ошибок (C++20)
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template<Addable T>
T good_add(T a, T b) {
    return a + b;
}

// Использование: good_add(std::vector<int>{}, std::vector<int>{});
// Ошибка: "std::vector<int> не удовлетворяет концепту Addable"

// ❌ ПЛОХО: Чрезмерное использование SFINAE
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
bad_increment(T value) {
    return value + 1;
}

template<typename T>
typename std::enable_if<!std::is_integral<T>::value, T>::type
bad_increment(T value) {
    return value;  // Для не-интегральных типов
}

// ✅ ХОРОШО: Используйте if constexpr (C++17) или concepts
template<typename T>
T good_increment(T value) {
    if constexpr (std::is_integral_v<T>) {
        return value + 1;
    } else {
        return value;
    }
}

// Или с concepts:
template<typename T>
T modern_increment(T value) requires std::integral<T> {
    return value + 1;
}

template<typename T>
T modern_increment(T value) requires (!std::integral<T>) {
    return value;
}

} // namespace template_antipatterns

// ============================================
// 📌 PERFORMANCE ANTI-PATTERNS
// ============================================

namespace performance_antipatterns {

// ❌ ПЛОХО: Ненужные копирования
void bad_unnecessary_copies() {
    std::vector<std::string> names{"Alice", "Bob", "Charlie"};
    
    // Копирование каждой строки!
    for (std::string name : names) {
        std::cout << name << '\n';
    }
}

// ✅ ХОРОШО: Используйте const ref
void good_const_ref() {
    std::vector<std::string> names{"Alice", "Bob", "Charlie"};
    
    for (const auto& name : names) {
        std::cout << name << '\n';
    }
}

// ❌ ПЛОХО: std::endl вместо '\n'
void bad_endl() {
    for (int i = 0; i < 1000; ++i) {
        std::cout << i << std::endl;  // Сброс буфера на КАЖДОЙ итерации!
    }
}

// ✅ ХОРОШО: Используйте '\n'
void good_newline() {
    for (int i = 0; i < 1000; ++i) {
        std::cout << i << '\n';  // Буфер сбрасывается когда нужно
    }
    // Явный flush при необходимости:
    std::cout << std::flush;
}

// ❌ ПЛОХО: Передача больших объектов по значению
struct BigData {
    std::array<double, 1000> values;
};

void bad_pass_by_value(BigData data) {  // Копирование 8KB!
    process(data);
}

// ✅ ХОРОШО: Передавайте по const ref
void good_pass_by_ref(const BigData& data) {  // Без копирования
    process(data);
}

// Или по значению, если нужна копия И move дешев
void good_pass_by_value_when_needed(BigData data) {
    store(std::move(data));  // Избегаем лишнего move
}

// ❌ ПЛОХО: vector<bool> - специализация с проблемами
void bad_vector_bool() {
    std::vector<bool> flags{true, false, true};
    
    // bool& ref = flags[0];  // НЕ КОМПИЛИРУЕТСЯ!
    // vector<bool> хранит биты, не bool'ы
    
    auto& element = flags[0];  // Это НЕ bool&, а прокси-объект!
}

// ✅ ХОРОШО: Используйте vector<char> или bitset
void good_bool_alternatives() {
    // Если нужны bool'ы:
    std::vector<char> flags{1, 0, 1};
    bool& ref = reinterpret_cast<bool&>(flags[0]);  // OK
    
    // Если нужен битовый массив фиксированного размера:
    std::bitset<8> bits;
    bits[0] = true;
    
    // Если нужен динамический битовый массив:
    // Используйте сторонние библиотеки вроде boost::dynamic_bitset
}

// ❌ ПЛОХО: Преждевременная оптимизация
void bad_premature_optimization() {
    // "Я использую int вместо size_t для скорости"
    for (int i = 0; i < vec.size(); ++i) {  // size() возвращает size_t!
        // Предупреждение компилятора + потенциальные проблемы
    }
    
    // "Я буду использовать битовые трюки для ясности"
    int abs_value = (x ^ (x >> 31)) - (x >> 31);  // WTF?
}

// ✅ ХОРОШО: Сначала ясность, потом оптимизация
void good_clear_code() {
    // Используйте правильные типы
    for (size_t i = 0; i < vec.size(); ++i) {
        // Ясно и корректно
    }
    
    // Или еще лучше:
    for (const auto& item : vec) {
        // Самый ясный вариант
    }
    
    // Ясный код для абсолютного значения
    int abs_value = std::abs(x);
    
    // Оптимизируйте ТОЛЬКО если профилирование показало узкое место!
}

} // namespace performance_antipatterns

// ============================================
// 📌 MODERN C++ MISUSE
// ============================================

namespace modern_cpp_misuse {

// ❌ ПЛОХО: Не использовать auto где нужно
void bad_without_auto() {
    std::vector<int> vec{1, 2, 3};
    
    std::vector<int>::iterator it = vec.begin();  // Многословно
    
    std::pair<std::string, std::vector<int>> complex_pair;
    std::pair<std::string, std::vector<int>> copy = complex_pair;  // Дубли
}

// ✅ ХОРОШО: Используйте auto для итераторов и сложных типов
void good_with_auto() {
    std::vector<int> vec{1, 2, 3};
    
    auto it = vec.begin();  // Ясно и кратко
    
    std::pair<std::string, std::vector<int>> complex_pair;
    auto copy = complex_pair;  // DRY принцип
}

// ❌ ПЛОХО: Не использовать range-based for
void bad_index_loop() {
    std::vector<std::string> names{"Alice", "Bob", "Charlie"};
    
    for (size_t i = 0; i < names.size(); ++i) {
        std::cout << names[i] << '\n';
    }
}

// ✅ ХОРОШО: Range-based for для итерации
void good_range_for() {
    std::vector<std::string> names{"Alice", "Bob", "Charlie"};
    
    for (const auto& name : names) {
        std::cout << name << '\n';
    }
}

// ❌ ПЛОХО: Не использовать std::optional для nullable значений
std::string* bad_find_user(int id) {
    if (user_exists(id)) {
        return new std::string(get_user_name(id));  // Утечка памяти!
    }
    return nullptr;
}

// ✅ ХОРОШО: Используйте std::optional (C++17)
std::optional<std::string> good_find_user(int id) {
    if (user_exists(id)) {
        return get_user_name(id);
    }
    return std::nullopt;
}

// Использование:
void use_optional() {
    if (auto user = good_find_user(42)) {
        std::cout << "User: " << *user << '\n';
    } else {
        std::cout << "User not found\n";
    }
}

// ❌ ПЛОХО: Сырые циклы вместо алгоритмов
void bad_raw_loops() {
    std::vector<int> numbers{1, 2, 3, 4, 5};
    
    // Подсчет четных чисел
    int count = 0;
    for (int n : numbers) {
        if (n % 2 == 0) {
            ++count;
        }
    }
    
    // Проверка, все ли положительные
    bool all_positive = true;
    for (int n : numbers) {
        if (n <= 0) {
            all_positive = false;
            break;
        }
    }
}

// ✅ ХОРОШО: Используйте алгоритмы STL
void good_algorithms() {
    std::vector<int> numbers{1, 2, 3, 4, 5};
    
    // Подсчет четных чисел
    auto count = std::ranges::count_if(numbers, 
        [](int n) { return n % 2 == 0; });
    
    // Проверка, все ли положительные
    bool all_positive = std::ranges::all_of(numbers,
        [](int n) { return n > 0; });
}

// ❌ ПЛОХО: Игнорирование std::expected (C++23)
std::pair<int, bool> bad_parse_int(const std::string& s) {
    try {
        return {std::stoi(s), true};
    } catch (...) {
        return {0, false};  // Потеря информации об ошибке!
    }
}

// ✅ ХОРОШО: Используйте std::expected (C++23)
/*
std::expected<int, std::string> good_parse_int(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (const std::exception& e) {
        return std::unexpected(e.what());  // Сохраняем информацию
    }
}

// Использование:
void use_expected() {
    auto result = good_parse_int("123");
    if (result) {
        std::cout << "Parsed: " << *result << '\n';
    } else {
        std::cout << "Error: " << result.error() << '\n';
    }
}
*/

} // namespace modern_cpp_misuse

// ============================================
// 📌 API DESIGN ANTI-PATTERNS
// ============================================

namespace api_design_antipatterns {

// ❌ ПЛОХО: Boolean параметры неясны в месте вызова
void bad_set_visibility(bool visible) {
    // Использование: bad_set_visibility(true);
    // Что значит true? Показать или скрыть?
}

// ✅ ХОРОШО: Используйте enum для ясности
enum class Visibility {
    Visible,
    Hidden
};

void good_set_visibility(Visibility vis) {
    // Использование: good_set_visibility(Visibility::Visible);
    // Кристально ясно!
}

// ❌ ПЛОХО: Out-параметры вместо возврата значений
void bad_calculate(int a, int b, int& sum, int& product) {
    sum = a + b;
    product = a * b;
}

// ✅ ХОРОШО: Возвращайте значения
struct CalculationResult {
    int sum;
    int product;
};

CalculationResult good_calculate(int a, int b) {
    return {a + b, a * b};
}

// Или с structured bindings (C++17):
std::pair<int, int> modern_calculate(int a, int b) {
    return {a + b, a * b};
}

void use_modern_calculate() {
    auto [sum, product] = modern_calculate(5, 10);
}

// ❌ ПЛОХО: Смешивание ответственностей в одной функции
void bad_process_and_save_user(const std::string& name, const std::string& email) {
    // Валидация
    if (name.empty() || email.empty()) {
        throw std::invalid_argument("Invalid input");
    }
    
    // Бизнес-логика
    User user = create_user(name, email);
    validate_user(user);
    
    // База данных
    save_to_database(user);
    
    // Отправка email
    send_welcome_email(user);
    
    // Логирование
    log("User created: " + name);
}

// ✅ ХОРОШО: Разделите ответственности
User good_create_user(const std::string& name, const std::string& email) {
    if (name.empty() || email.empty()) {
        throw std::invalid_argument("Invalid input");
    }
    return create_user(name, email);
}

void good_save_user(const User& user) {
    save_to_database(user);
}

void good_notify_user(const User& user) {
    send_welcome_email(user);
}

// Композиция в высокоуровневой функции:
void good_register_user(const std::string& name, const std::string& email) {
    auto user = good_create_user(name, email);
    good_save_user(user);
    good_notify_user(user);
}

// ❌ ПЛОХО: God Object - объект, знающий/делающий слишком много
class BadUserManager {
    // База данных
    void save_to_db(const User& user);
    void load_from_db(int id);
    void delete_from_db(int id);
    
    // Валидация
    bool validate_email(const std::string& email);
    bool validate_name(const std::string& name);
    
    // Email
    void send_email(const User& user);
    void send_password_reset(const User& user);
    
    // Аутентификация
    bool authenticate(const std::string& username, const std::string& password);
    
    // Авторизация
    bool has_permission(const User& user, const std::string& resource);
    
    // Логирование
    void log(const std::string& message);
    
    // И еще 50 методов...
};

// ✅ ХОРОШО: Разделите ответственности на отдельные классы
class UserRepository {
    void save(const User& user);
    User load(int id);
    void remove(int id);
};

class UserValidator {
    bool validate_email(const std::string& email);
    bool validate_name(const std::string& name);
};

class EmailService {
    void send(const User& user, const std::string& message);
};

class AuthenticationService {
    bool authenticate(const std::string& username, const std::string& password);
};

class AuthorizationService {
    bool has_permission(const User& user, const std::string& resource);
};

} // namespace api_design_antipatterns

// ============================================
// 📌 CODE SMELL PATTERNS
// ============================================

namespace code_smells {

// ❌ ПЛОХО: Magic numbers
void bad_magic_numbers() {
    if (age >= 18) {  // Что значит 18?
        grant_access();
    }
    
    double tax = price * 0.15;  // Что за 0.15?
    
    for (int i = 0; i < 7; ++i) {  // Почему 7?
        process_day(i);
    }
}

// ✅ ХОРОШО: Именованные константы
void good_named_constants() {
    constexpr int LEGAL_AGE = 18;
    constexpr double TAX_RATE = 0.15;
    constexpr int DAYS_IN_WEEK = 7;
    
    if (age >= LEGAL_AGE) {
        grant_access();
    }
    
    double tax = price * TAX_RATE;
    
    for (int day = 0; day < DAYS_IN_WEEK; ++day) {
        process_day(day);
    }
}

// ❌ ПЛОХО: Глубокая вложенность
void bad_deep_nesting(const User& user) {
    if (user.is_authenticated()) {
        if (user.has_permission("write")) {
            if (user.has_quota()) {
                if (validate_input(user.input())) {
                    if (check_rate_limit(user)) {
                        // Наконец-то настоящая логика!
                        process(user);
                    } else {
                        log_rate_limit_exceeded();
                    }
                } else {
                    log_invalid_input();
                }
            } else {
                log_quota_exceeded();
            }
        } else {
            log_no_permission();
        }
    } else {
        log_not_authenticated();
    }
}

// ✅ ХОРОШО: Early returns для уменьшения вложенности
void good_early_returns(const User& user) {
    if (!user.is_authenticated()) {
        log_not_authenticated();
        return;
    }
    
    if (!user.has_permission("write")) {
        log_no_permission();
        return;
    }
    
    if (!user.has_quota()) {
        log_quota_exceeded();
        return;
    }
    
    if (!validate_input(user.input())) {
        log_invalid_input();
        return;
    }
    
    if (!check_rate_limit(user)) {
        log_rate_limit_exceeded();
        return;
    }
    
    // Настоящая логика на верхнем уровне
    process(user);
}

// ❌ ПЛОХО: Длинные функции (100+ строк)
void bad_long_function() {
    // Инициализация (20 строк)
    // ...
    
    // Валидация (30 строк)
    // ...
    
    // Обработка (40 строк)
    // ...
    
    // Сохранение (20 строк)
    // ...
    
    // Очистка (10 строк)
    // ...
}

// ✅ ХОРОШО: Разбивка на маленькие функции
void good_decomposed_function() {
    initialize();
    validate();
    process();
    save();
    cleanup();
}

// ❌ ПЛОХО: Дублирование кода
void bad_process_user_data() {
    std::string name = user_input["name"];
    name.erase(0, name.find_first_not_of(" \t"));
    name.erase(name.find_last_not_of(" \t") + 1);
    
    std::string email = user_input["email"];
    email.erase(0, email.find_first_not_of(" \t"));
    email.erase(email.find_last_not_of(" \t") + 1);
    
    std::string phone = user_input["phone"];
    phone.erase(0, phone.find_first_not_of(" \t"));
    phone.erase(phone.find_last_not_of(" \t") + 1);
}

// ✅ ХОРОШО: DRY - Don't Repeat Yourself
std::string trim(std::string s) {
    s.erase(0, s.find_first_not_of(" \t"));
    s.erase(s.find_last_not_of(" \t") + 1);
    return s;
}

void good_process_user_data() {
    std::string name = trim(user_input["name"]);
    std::string email = trim(user_input["email"]);
    std::string phone = trim(user_input["phone"]);
}

// ❌ ПЛОХО: Закомментированный код
void bad_with_commented_code() {
    process_data();
    
    // Old implementation - don't delete, might need later
    // for (int i = 0; i < data.size(); ++i) {
    //     process(data[i]);
    // }
    
    // Legacy code
    // old_process();
    // legacy_cleanup();
    
    save_result();
}

// ✅ ХОРОШО: Удаляйте мертвый код (у вас есть Git!)
void good_clean_code() {
    process_data();
    save_result();
    // Старый код в истории Git, если понадобится
}

} // namespace code_smells

// ============================================
// 📌 UNDEFINED BEHAVIOR TRAPS
// ============================================

namespace undefined_behavior {

// ❌ ПЛОХО: Signed integer overflow
void bad_signed_overflow() {
    int max_int = std::numeric_limits<int>::max();
    int overflow = max_int + 1;  // UNDEFINED BEHAVIOR!
    // Может быть что угодно
}

// ✅ ХОРОШО: Используйте unsigned или проверяйте
void good_overflow_handling() {
    unsigned int max_uint = std::numeric_limits<unsigned int>::max();
    unsigned int overflow = max_uint + 1;  // Определенное поведение: 0
    
    // Или проверяйте перед операцией:
    int a = 1000000, b = 1000000;
    if (a > std::numeric_limits<int>::max() - b) {
        // Переполнение!
        throw std::overflow_error("Integer overflow");
    }
    int sum = a + b;
}

// ❌ ПЛОХО: Использование неинициализированных переменных
void bad_uninitialized() {
    int x;  // Неинициализирован!
    std::cout << x << '\n';  // UNDEFINED BEHAVIOR!
    
    int* ptr;  // Неинициализирован!
    *ptr = 42;  // UNDEFINED BEHAVIOR!
}

// ✅ ХОРОШО: Всегда инициализируйте переменные
void good_initialized() {
    int x = 0;  // Или другое значение по умолчанию
    std::cout << x << '\n';  // OK
    
    int* ptr = nullptr;
    if (ptr) {  // Проверка перед использованием
        *ptr = 42;
    }
}

// ❌ ПЛОХО: Висячие ссылки
const std::string& bad_dangling_reference() {
    std::string temp = "Hello";
    return temp;  // UNDEFINED BEHAVIOR! temp уничтожается
}

void bad_use_dangling() {
    const std::string& ref = bad_dangling_reference();
    std::cout << ref << '\n';  // Обращение к уничтоженному объекту!
}

// ✅ ХОРОШО: Возвращайте по значению (RVO оптимизирует)
std::string good_return_by_value() {
    std::string temp = "Hello";
    return temp;  // OK, RVO избежит копирования
}

// ❌ ПЛОХО: Инвалидация итераторов
void bad_iterator_invalidation() {
    std::vector<int> vec{1, 2, 3, 4, 5};
    
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (*it % 2 == 0) {
            vec.erase(it);  // ИНВАЛИДИРУЕТ it!
            // Следующий ++it - UNDEFINED BEHAVIOR!
        }
    }
}

// ✅ ХОРОШО: Используйте возвращаемый итератор
void good_iterator_handling() {
    std::vector<int> vec{1, 2, 3, 4, 5};
    
    for (auto it = vec.begin(); it != vec.end(); ) {
        if (*it % 2 == 0) {
            it = vec.erase(it);  // erase возвращает следующий валидный итератор
        } else {
            ++it;
        }
    }
    
    // Или используйте erase-remove idiom:
    vec.erase(
        std::remove_if(vec.begin(), vec.end(),
            [](int x) { return x % 2 == 0; }),
        vec.end()
    );
}

// ❌ ПЛОХО: Множественные модификации без sequence point
void bad_sequence_point() {
    int i = 0;
    int x = i++ + i++;  // UNDEFINED BEHAVIOR!
    // Порядок вычисления не определен
    
    int arr[10];
    int idx = 0;
    arr[idx] = idx++;  // UNDEFINED BEHAVIOR!
}

// ✅ ХОРОШО: Разделите операции
void good_sequence_points() {
    int i = 0;
    int temp1 = i++;
    int temp2 = i++;
    int x = temp1 + temp2;  // OK
    
    int arr[10];
    int idx = 0;
    arr[idx] = idx;
    ++idx;  // OK
}

// ❌ ПЛОХО: Обращение к массиву за границами
void bad_out_of_bounds() {
    int arr[5] = {1, 2, 3, 4, 5};
    int value = arr[10];  // UNDEFINED BEHAVIOR!
    
    std::vector<int> vec{1, 2, 3};
    vec[5] = 42;  // UNDEFINED BEHAVIOR!
}

// ✅ ХОРОШО: Используйте at() для проверки или проверяйте границы
void good_bounds_checking() {
    int arr[5] = {1, 2, 3, 4, 5};
    int index = 10;
    if (index < 5) {
        int value = arr[index];
    }
    
    std::vector<int> vec{1, 2, 3};
    try {
        vec.at(5) = 42;  // Бросит std::out_of_range
    } catch (const std::out_of_range&) {
        std::cerr << "Index out of range\n";
    }
}

} // namespace undefined_behavior

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== C++ Anti-Patterns Guide ===\n\n";
    
    std::cout << "Этот файл содержит примеры ПЛОХИХ практик\n";
    std::cout << "и их ПРАВИЛЬНЫХ альтернатив.\n\n";
    
    std::cout << "Основные категории:\n";
    std::cout << "1. Memory Management - RAII, умные указатели\n";
    std::cout << "2. Exception Safety - noexcept, безопасность\n";
    std::cout << "3. Concurrency - потоки, блокировки, data races\n";
    std::cout << "4. Templates - concepts, bloat, SFINAE\n";
    std::cout << "5. Performance - копирования, endl, vector<bool>\n";
    std::cout << "6. Modern C++ - auto, ranges, optional\n";
    std::cout << "7. API Design - ясность, разделение ответственностей\n";
    std::cout << "8. Code Smells - magic numbers, вложенность, дубли\n";
    std::cout << "9. Undefined Behavior - инициализация, итераторы\n";
    
    std::cout << "\n✅ Следуйте ХОРОШИМ примерам\n";
    std::cout << "❌ Избегайте ПЛОХИХ примеров\n";
    
    return 0;
}