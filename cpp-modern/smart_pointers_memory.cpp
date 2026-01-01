/*
 * ============================================
 * УМНЫЕ УКАЗАТЕЛИ И УПРАВЛЕНИЕ ПАМЯТЬЮ
 * ============================================
 * 
 * Безопасная работа с динамической памятью:
 * - unique_ptr (эксклюзивное владение)
 * - shared_ptr (разделяемое владение)
 * - weak_ptr (наблюдение без владения)
 * - RAII паттерны
 * - Move semantics
 * 
 * Компиляция: g++ -std=c++20 smart_pointers_memory.cpp
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <cassert>

// ============================================
// 📌 RAW POINTERS PROBLEMS
// ============================================

namespace raw_pointers_problems {

void memory_leak_example() {
    std::cout << "=== Memory Leak ===\n";
    
    // ❌ Memory leak
    int* ptr = new int(42);
    // Забыли delete!
    
    std::cout << "Value: " << *ptr << '\n';
    delete ptr;  // Должны всегда помнить!
}

void dangling_pointer_example() {
    std::cout << "\n=== Dangling Pointer ===\n";
    
    int* ptr = new int(42);
    delete ptr;
    
    // ❌ Dangling pointer - undefined behavior!
    // std::cout << *ptr << '\n';  // Опасно!
    
    ptr = nullptr;  // Хорошая практика
}

void double_deletion_example() {
    std::cout << "\n=== Double Deletion ===\n";
    
    int* ptr = new int(42);
    delete ptr;
    // delete ptr;  // ❌ Undefined behavior!
    
    ptr = nullptr;
    delete ptr;  // ✅ Безопасно удалять nullptr
}

void ownership_ambiguity() {
    std::cout << "\n=== Ownership Ambiguity ===\n";
    
    int* data = new int(42);
    
    // Кто должен удалить data?
    auto process = [](int* p) {
        std::cout << "Processing: " << *p << '\n';
        // delete p;  // Или здесь?
    };
    
    process(data);
    delete data;  // Или здесь?
    
    // С умными указателями это ясно!
}

void demo() {
    std::cout << "=== Raw Pointers Problems ===\n\n";
    memory_leak_example();
    dangling_pointer_example();
    double_deletion_example();
    ownership_ambiguity();
}

} // namespace raw_pointers_problems

// ============================================
// 📌 std::unique_ptr
// ============================================

namespace unique_ptr_examples {

void basic_usage() {
    std::cout << "\n=== unique_ptr Basics ===\n";
    
    // ✅ Автоматическое удаление
    {
        std::unique_ptr<int> ptr = std::make_unique<int>(42);
        std::cout << "Value: " << *ptr << '\n';
    }  // Автоматически удаляется!
    
    // make_unique - предпочтительный способ (C++14)
    auto ptr2 = std::make_unique<std::string>("Hello");
    std::cout << "String: " << *ptr2 << '\n';
}

void array_usage() {
    std::cout << "\n=== unique_ptr Arrays ===\n";
    
    // Массивы
    auto arr = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }
    
    std::cout << "Array: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';
    
    // ✅ Лучше использовать vector!
    std::vector<int> vec(5);
}

// Custom deleter
struct FileDeleter {
    void operator()(FILE* file) const {
        if (file) {
            std::cout << "Closing file\n";
            fclose(file);
        }
    }
};

void custom_deleter_example() {
    std::cout << "\n=== Custom Deleter ===\n";
    
    // unique_ptr с custom deleter
    std::unique_ptr<FILE, FileDeleter> file(
        fopen("test.txt", "w")
    );
    
    if (file) {
        fprintf(file.get(), "Hello from unique_ptr!\n");
    }
    // Файл автоматически закроется!
}

// Move semantics
std::unique_ptr<int> create_unique() {
    return std::make_unique<int>(100);
}

void move_semantics() {
    std::cout << "\n=== Move Semantics ===\n";
    
    auto ptr1 = std::make_unique<int>(42);
    
    // ❌ Нельзя копировать!
    // auto ptr2 = ptr1;  // Ошибка компиляции
    
    // ✅ Можно перемещать
    auto ptr2 = std::move(ptr1);
    
    assert(ptr1 == nullptr);  // ptr1 теперь пустой
    std::cout << "Moved value: " << *ptr2 << '\n';
    
    // Return value optimization
    auto ptr3 = create_unique();
    std::cout << "Created: " << *ptr3 << '\n';
}

// Pimpl idiom
class Widget {
    struct Impl;  // Forward declaration
    std::unique_ptr<Impl> pImpl;
    
public:
    Widget();
    ~Widget();
    void do_something();
};

// В .cpp файле:
struct Widget::Impl {
    std::string data;
    int value;
    
    void process() {
        std::cout << "Processing: " << data << '\n';
    }
};

Widget::Widget() : pImpl(std::make_unique<Impl>()) {
    pImpl->data = "Widget data";
    pImpl->value = 42;
}

Widget::~Widget() = default;  // Важно! Деструктор должен быть в .cpp

void Widget::do_something() {
    pImpl->process();
}

void pimpl_example() {
    std::cout << "\n=== Pimpl Idiom ===\n";
    Widget w;
    w.do_something();
}

// Factory functions
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

class Circle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing circle\n";
    }
};

class Rectangle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing rectangle\n";
    }
};

std::unique_ptr<Shape> create_shape(const std::string& type) {
    if (type == "circle") {
        return std::make_unique<Circle>();
    } else {
        return std::make_unique<Rectangle>();
    }
}

void factory_example() {
    std::cout << "\n=== Factory Pattern ===\n";
    
    auto shape1 = create_shape("circle");
    auto shape2 = create_shape("rectangle");
    
    shape1->draw();
    shape2->draw();
}

void demo() {
    std::cout << "\n=== unique_ptr Examples ===\n";
    basic_usage();
    array_usage();
    custom_deleter_example();
    move_semantics();
    pimpl_example();
    factory_example();
}

} // namespace unique_ptr_examples

// ============================================
// 📌 std::shared_ptr
// ============================================

namespace shared_ptr_examples {

void basic_usage() {
    std::cout << "\n=== shared_ptr Basics ===\n";
    
    // Разделяемое владение
    auto ptr1 = std::make_shared<int>(42);
    std::cout << "Count: " << ptr1.use_count() << '\n';  // 1
    
    {
        auto ptr2 = ptr1;  // Копирование увеличивает счетчик
        std::cout << "Count: " << ptr1.use_count() << '\n';  // 2
        std::cout << "ptr2: " << *ptr2 << '\n';
    }  // ptr2 уничтожен, счетчик уменьшился
    
    std::cout << "Count: " << ptr1.use_count() << '\n';  // 1
}

void make_shared_vs_new() {
    std::cout << "\n=== make_shared vs new ===\n";
    
    // ❌ Две аллокации (объект + control block)
    std::shared_ptr<int> ptr1(new int(42));
    
    // ✅ Одна аллокация (эффективнее!)
    auto ptr2 = std::make_shared<int>(42);
    
    std::cout << "Prefer make_shared for performance\n";
}

// Custom deleter
void custom_deleter_example() {
    std::cout << "\n=== Custom Deleter ===\n";
    
    auto deleter = [](int* p) {
        std::cout << "Deleting " << *p << '\n';
        delete p;
    };
    
    std::shared_ptr<int> ptr(new int(42), deleter);
    // При уничтожении вызовет deleter
}

// enable_shared_from_this
class Task : public std::enable_shared_from_this<Task> {
    std::string name_;
    
public:
    Task(std::string name) : name_(std::move(name)) {}
    
    void run() {
        std::cout << "Running task: " << name_ << '\n';
        
        // Получаем shared_ptr на self
        auto self = shared_from_this();
        std::cout << "Use count in run: " << self.use_count() << '\n';
    }
};

void shared_from_this_example() {
    std::cout << "\n=== shared_from_this ===\n";
    
    auto task = std::make_shared<Task>("MyTask");
    std::cout << "Initial count: " << task.use_count() << '\n';
    
    task->run();
    
    std::cout << "Final count: " << task.use_count() << '\n';
}

// Cyclic references - ПРОБЛЕМА!
class Node {
public:
    std::shared_ptr<Node> next;  // ❌ Циклическая ссылка!
    std::string data;
    
    Node(std::string d) : data(std::move(d)) {
        std::cout << "Node created: " << data << '\n';
    }
    
    ~Node() {
        std::cout << "Node destroyed: " << data << '\n';
    }
};

void cyclic_reference_problem() {
    std::cout << "\n=== Cyclic Reference Problem ===\n";
    
    auto node1 = std::make_shared<Node>("A");
    auto node2 = std::make_shared<Node>("B");
    
    node1->next = node2;
    node2->next = node1;  // ❌ Цикл! Memory leak!
    
    std::cout << "Exiting scope...\n";
    // Деструкторы НЕ вызовутся!
}

void thread_safety() {
    std::cout << "\n=== Thread Safety ===\n";
    
    auto ptr = std::make_shared<int>(42);
    
    // ✅ Счетчик ссылок - thread-safe
    // Копирование shared_ptr в разных потоках безопасно
    
    // ❌ Сам объект НЕ защищен!
    // *ptr = 100;  // Требует синхронизации!
    
    std::cout << "Reference counting is thread-safe\n";
    std::cout << "Object access is NOT thread-safe\n";
}

void demo() {
    std::cout << "\n=== shared_ptr Examples ===\n";
    basic_usage();
    make_shared_vs_new();
    custom_deleter_example();
    shared_from_this_example();
    cyclic_reference_problem();
    thread_safety();
}

} // namespace shared_ptr_examples

// ============================================
// 📌 std::weak_ptr
// ============================================

namespace weak_ptr_examples {

void breaking_cycles() {
    std::cout << "\n=== Breaking Cycles ===\n";
    
    struct Node {
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;  // ✅ Используем weak_ptr!
        std::string data;
        
        Node(std::string d) : data(std::move(d)) {
            std::cout << "Node created: " << data << '\n';
        }
        
        ~Node() {
            std::cout << "Node destroyed: " << data << '\n';
        }
    };
    
    auto node1 = std::make_shared<Node>("A");
    auto node2 = std::make_shared<Node>("B");
    
    node1->next = node2;
    node2->prev = node1;  // ✅ Цикл разорван!
    
    std::cout << "Exiting scope...\n";
    // Деструкторы ВЫЗОВУТСЯ!
}

void lock_method() {
    std::cout << "\n=== lock() Method ===\n";
    
    std::weak_ptr<int> weak;
    
    {
        auto shared = std::make_shared<int>(42);
        weak = shared;
        
        // lock() создает shared_ptr
        if (auto ptr = weak.lock()) {
            std::cout << "Object alive: " << *ptr << '\n';
        }
    }  // shared уничтожен
    
    // Проверка после уничтожения
    if (auto ptr = weak.lock()) {
        std::cout << "Still alive\n";
    } else {
        std::cout << "Object destroyed\n";
    }
    
    // expired() - альтернативная проверка
    std::cout << "Expired: " << std::boolalpha << weak.expired() << '\n';
}

// Cache example
class ExpensiveObject {
    int id_;
    
public:
    ExpensiveObject(int id) : id_(id) {
        std::cout << "Creating expensive object " << id_ << '\n';
    }
    
    ~ExpensiveObject() {
        std::cout << "Destroying expensive object " << id_ << '\n';
    }
    
    int id() const { return id_; }
};

class Cache {
    std::map<int, std::weak_ptr<ExpensiveObject>> cache_;
    
public:
    std::shared_ptr<ExpensiveObject> get(int id) {
        // Проверяем кэш
        auto it = cache_.find(id);
        if (it != cache_.end()) {
            if (auto ptr = it->second.lock()) {
                std::cout << "Cache hit for " << id << '\n';
                return ptr;
            }
        }
        
        // Создаем новый объект
        std::cout << "Cache miss for " << id << '\n';
        auto obj = std::make_shared<ExpensiveObject>(id);
        cache_[id] = obj;
        return obj;
    }
};

void cache_example() {
    std::cout << "\n=== Cache Example ===\n";
    
    Cache cache;
    
    {
        auto obj1 = cache.get(1);
        auto obj2 = cache.get(1);  // Cache hit!
    }  // obj уничтожен
    
    auto obj3 = cache.get(1);  // Cache miss - объект был удален
}

void demo() {
    std::cout << "\n=== weak_ptr Examples ===\n";
    breaking_cycles();
    lock_method();
    cache_example();
}

} // namespace weak_ptr_examples

// ============================================
// 📌 MEMORY MANAGEMENT
// ============================================

namespace memory_management {

void placement_new() {
    std::cout << "\n=== Placement New ===\n";
    
    alignas(int) char buffer[sizeof(int)];
    
    // Создаем объект в заранее выделенной памяти
    int* ptr = new (buffer) int(42);
    std::cout << "Value: " << *ptr << '\n';
    
    // Явно вызываем деструктор
    ptr->~int();
    // delete не нужен - память не выделялась!
}

void aligned_allocation() {
    std::cout << "\n=== Aligned Allocation ===\n";
    
    struct alignas(64) CacheLineAligned {
        int data[16];
    };
    
    auto ptr = std::make_unique<CacheLineAligned>();
    std::cout << "Alignment: " << alignof(CacheLineAligned) << '\n';
}

void custom_allocator() {
    std::cout << "\n=== Custom Allocator ===\n";
    
    // Простой allocator, который логирует
    template<typename T>
    struct LoggingAllocator {
        using value_type = T;
        
        T* allocate(std::size_t n) {
            std::cout << "Allocating " << n << " objects\n";
            return static_cast<T*>(::operator new(n * sizeof(T)));
        }
        
        void deallocate(T* p, std::size_t n) {
            std::cout << "Deallocating " << n << " objects\n";
            ::operator delete(p);
        }
    };
    
    std::vector<int, LoggingAllocator<int>> vec;
    vec.push_back(1);
    vec.push_back(2);
}

void demo() {
    std::cout << "\n=== Memory Management ===\n";
    placement_new();
    aligned_allocation();
    custom_allocator();
}

} // namespace memory_management

// ============================================
// 📌 RAII PATTERNS
// ============================================

namespace raii_patterns {

// Scope guard
template<typename Func>
class ScopeGuard {
    Func cleanup_;
    bool active_ = true;
    
public:
    explicit ScopeGuard(Func f) : cleanup_(std::move(f)) {}
    
    ~ScopeGuard() {
        if (active_) {
            cleanup_();
        }
    }
    
    void dismiss() { active_ = false; }
    
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

template<typename Func>
auto make_scope_guard(Func f) {
    return ScopeGuard<Func>(std::move(f));
}

void scope_guard_example() {
    std::cout << "\n=== Scope Guard ===\n";
    
    FILE* file = fopen("test.txt", "w");
    auto guard = make_scope_guard([file]() {
        if (file) {
            std::cout << "Closing file via scope guard\n";
            fclose(file);
        }
    });
    
    if (file) {
        fprintf(file, "Hello RAII!\n");
    }
    
    // Файл автоматически закроется при выходе из scope
}

// RAII wrapper для ресурсов
class DatabaseConnection {
public:
    DatabaseConnection() {
        std::cout << "Opening database connection\n";
    }
    
    ~DatabaseConnection() {
        std::cout << "Closing database connection\n";
    }
    
    void execute(const std::string& query) {
        std::cout << "Executing: " << query << '\n';
    }
    
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
};

void raii_wrapper_example() {
    std::cout << "\n=== RAII Wrapper ===\n";
    
    {
        DatabaseConnection db;
        db.execute("SELECT * FROM users");
    }  // Автоматически закрывается!
}

void demo() {
    std::cout << "\n=== RAII Patterns ===\n";
    scope_guard_example();
    raii_wrapper_example();
}

} // namespace raii_patterns

// ============================================
// 📌 MOVE SEMANTICS DEEP DIVE
// ============================================

namespace move_semantics {

class Buffer {
    size_t size_;
    int* data_;
    
public:
    // Constructor
    explicit Buffer(size_t size) : size_(size), data_(new int[size_]) {
        std::cout << "Buffer(" << size_ << "): allocated\n";
    }
    
    // Destructor
    ~Buffer() {
        delete[] data_;
        std::cout << "~Buffer(): deallocated\n";
    }
    
    // Copy constructor
    Buffer(const Buffer& other) : size_(other.size_), data_(new int[size_]) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "Buffer(const Buffer&): copied\n";
    }
    
    // Copy assignment
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
            std::cout << "operator=(const Buffer&): copied\n";
        }
        return *this;
    }
    
    // Move constructor
    Buffer(Buffer&& other) noexcept : size_(other.size_), data_(other.data_) {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "Buffer(Buffer&&): moved\n";
    }
    
    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.data_ = nullptr;
            other.size_ = 0;
            std::cout << "operator=(Buffer&&): moved\n";
        }
        return *this;
    }
    
    size_t size() const { return size_; }
};

Buffer create_buffer(size_t size) {
    return Buffer(size);  // RVO
}

void demo() {
    std::cout << "\n=== Move Semantics ===\n";
    
    std::cout << "\n--- Copy ---\n";
    Buffer b1(100);
    Buffer b2 = b1;  // Copy
    
    std::cout << "\n--- Move ---\n";
    Buffer b3 = std::move(b1);  // Move
    
    std::cout << "\n--- RVO ---\n";
    Buffer b4 = create_buffer(200);  // No copy/move!
    
    std::cout << "\n--- End ---\n";
}

} // namespace move_semantics

// ============================================
// 📌 BEST PRACTICES
// ============================================

/*
 * BEST PRACTICES ДЛЯ УМНЫХ УКАЗАТЕЛЕЙ:
 * 
 * 1. ВЫБОР УМНОГО УКАЗАТЕЛЯ
 *    ✅ unique_ptr по умолчанию (эксклюзивное владение)
 *    ✅ shared_ptr когда нужно разделяемое владение
 *    ✅ weak_ptr для разрыва циклов
 *    ❌ Избегай raw pointers для владения
 * 
 * 2. СОЗДАНИЕ
 *    ✅ make_unique / make_shared
 *    ❌ new и передача в конструктор
 * 
 * 3. ПЕРЕДАЧА В ФУНКЦИИ
 *    ✅ По значению (unique_ptr) - передача владения
 *    ✅ По ссылке (const unique_ptr&) - без передачи
 *    ✅ Raw pointer (get()) - только наблюдение
 * 
 * 4. ЦИКЛИЧЕСКИЕ ССЫЛКИ
 *    ❌ shared_ptr → shared_ptr (цикл!)
 *    ✅ shared_ptr → weak_ptr
 * 
 * 5. ПРОИЗВОДИТЕЛЬНОСТЬ
 *    - unique_ptr: zero overhead
 *    - shared_ptr: небольшой overhead (счетчик)
 *    - make_shared лучше чем new
 * 
 * 6. THREAD SAFETY
 *    ✅ Счетчик ссылок shared_ptr - thread-safe
 *    ❌ Сам объект требует синхронизации
 * 
 * 7. MOVE SEMANTICS
 *    ✅ Всегда реализуй move для больших объектов
 *    ✅ Помечай noexcept
 *    ✅ Используй std::move явно
 */

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== Smart Pointers & Memory Management ===\n";
    
    raw_pointers_problems::demo();
    unique_ptr_examples::demo();
    shared_ptr_examples::demo();
    weak_ptr_examples::demo();
    memory_management::demo();
    raii_patterns::demo();
    move_semantics::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ unique_ptr - эксклюзивное владение, zero overhead\n";
    std::cout << "✓ shared_ptr - разделяемое владение, счетчик ссылок\n";
    std::cout << "✓ weak_ptr - наблюдение, разрыв циклов\n";
    std::cout << "✓ make_unique / make_shared предпочтительнее\n";
    std::cout << "✓ RAII - автоматическое управление ресурсами\n";
    std::cout << "✓ Move semantics - эффективная передача владения\n";
    std::cout << "✓ Избегай raw pointers для владения\n";
    
    return 0;
}
// • Memory alignment
// • Cache-friendly structures