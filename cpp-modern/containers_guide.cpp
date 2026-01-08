/*
 * ============================================
 * ПОЛНОЕ РУКОВОДСТВО ПО КОНТЕЙНЕРАМ STL
 * ============================================
 * 
 * Все контейнеры C++20/23 с примерами, decision tree
 * и рекомендациями по выбору.
 * 
 * Требования: C++20 или выше
 * Компиляция: g++ -std=c++20 containers_guide.cpp
 */

#include <vector>
#include <array>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <span>         // C++20
#include <string>
#include <string_view>
#include <iostream>
#include <algorithm>
#include <ranges>

// ============================================
// 📌 DECISION TREE - КАКОЙ КОНТЕЙНЕР ВЫБРАТЬ?
// ============================================

/*
┌─────────────────────────────────────────────────────────────────────────┐
│                   DECISION TREE ДЛЯ ВЫБОРА КОНТЕЙНЕРА                   │
└─────────────────────────────────────────────────────────────────────────┘

НАЧАЛО
│
├─ Нужен ли случайный доступ (по индексу)?
│  │
│  ├─ ДА → Нужен ли фиксированный размер?
│  │      │
│  │      ├─ ДА → std::array<T, N>
│  │      │       ✓ Compile-time размер
│  │      │       ✓ Stack allocation
│  │      │       ✓ Нет overhead
│  │      │
│  │      └─ НЕТ → Вставки/удаления с обоих концов?
│  │             │
│  │             ├─ ДА → std::deque<T>
│  │             │       ✓ O(1) push/pop с обоих концов
│  │             │       ✓ O(1) random access
│  │             │       ✗ Не контигуэнтная память
│  │             │
│  │             └─ НЕТ → std::vector<T> (DEFAULT CHOICE)
│  │                     ✓ O(1) amortized push_back
│  │                     ✓ O(1) random access
│  │                     ✓ Cache-friendly
│  │                     ✓ Минимальный overhead
│  │
│  └─ НЕТ → Частые вставки/удаления в середине?
│         │
│         ├─ ДА → std::list<T> (doubly-linked)
│         │       ✓ O(1) insert/erase anywhere (с итератором)
│         │       ✗ O(n) поиск
│         │       ✗ Большой memory overhead
│         │       или std::forward_list<T> (singly-linked)
│         │
│         └─ НЕТ → Нужен только LIFO/FIFO доступ?
│                │
│                ├─ LIFO → std::stack<T>
│                ├─ FIFO → std::queue<T>
│                └─ Priority → std::priority_queue<T>
│
├─ Нужен ли поиск по ключу?
│  │
│  ├─ ДА → Нужна сортировка?
│  │      │
│  │      ├─ ДА → Уникальные ключи?
│  │      │      │
│  │      │      ├─ ДА, только ключи → std::set<T>
│  │      │      │                     ✓ O(log n) insert/find/erase
│  │      │      │                     ✓ Автоматическая сортировка
│  │      │      │                     ✓ Итерация в порядке
│  │      │      │
│  │      │      ├─ ДА, ключ-значение → std::map<K, V>
│  │      │      │                       ✓ O(log n) operations
│  │      │      │
│  │      │      ├─ НЕТ, только ключи → std::multiset<T>
│  │      │      └─ НЕТ, ключ-значение → std::multimap<K, V>
│  │      │
│  │      └─ НЕТ → Уникальные ключи?
│  │             │
│  │             ├─ ДА, только ключи → std::unordered_set<T>
│  │             │                     ✓ O(1) average insert/find/erase
│  │             │                     ✗ Нет порядка
│  │             │
│  │             ├─ ДА, ключ-значение → std::unordered_map<K, V>
│  │             │                       ✓ O(1) average operations
│  │             │                       ✓ DEFAULT для hash maps
│  │             │
│  │             ├─ НЕТ, только ключи → std::unordered_multiset<T>
│  │             └─ НЕТ, ключ-значение → std::unordered_multimap<K, V>
│  │
│  └─ НЕТ → Только view на существующие данные?
│         │
│         ├─ ДА → std::span<T> (C++20)
│         │       ✓ Non-owning view
│         │       ✓ Нет копирования
│         │       
│         └─ НЕТ → std::string для текста
│                 std::vector<T> для остального


┌─────────────────────────────────────────────────────────────────────────┐
│                      PERFORMANCE CHARACTERISTICS                        │
└─────────────────────────────────────────────────────────────────────────┘

Container           | Insert  | Erase   | Find    | Random | Memory
                    | Front   | Front   |         | Access | Overhead
────────────────────┼─────────┼─────────┼─────────┼────────┼──────────
vector              | O(n)    | O(n)    | O(n)    | O(1)   | Low
deque               | O(1)    | O(1)    | O(n)    | O(1)   | Medium
list                | O(1)    | O(1)    | O(n)    | O(n)   | High
forward_list        | O(1)    | O(1)    | O(n)    | O(n)   | Medium
────────────────────┼─────────┼─────────┼─────────┼────────┼──────────
set/map             | O(log n)| O(log n)| O(log n)| N/A    | High
unordered_set/map   | O(1)*   | O(1)*   | O(1)*   | N/A    | High
────────────────────┼─────────┼─────────┼─────────┼────────┼──────────
stack/queue         | O(1)    | O(1)    | N/A     | N/A    | Like base
priority_queue      | O(log n)| O(log n)| N/A     | N/A    | Like base

* Average case. Worst case O(n) при коллизиях hash.
*/

// ============================================
// 📌 SEQUENTIAL CONTAINERS - std::vector
// ============================================

void demo_vector_basics() {
    std::cout << "=== std::vector ===\n";
    
    // Создание
    std::vector<int> v1;                        // Пустой
    std::vector<int> v2(10);                    // 10 элементов, инициализированных 0
    std::vector<int> v3(10, 42);                // 10 элементов = 42
    std::vector<int> v4{1, 2, 3, 4, 5};         // Initializer list
    std::vector<int> v5(v4.begin(), v4.end());  // Из итераторов
    
    // Capacity vs Size
    std::vector<int> vec;
    std::cout << "Size: " << vec.size() << '\n';          // 0
    std::cout << "Capacity: " << vec.capacity() << '\n';  // 0 (или небольшое число)
    
    vec.push_back(1);
    std::cout << "After push: Size=" << vec.size() 
              << " Capacity=" << vec.capacity() << '\n';  // Capacity выросла
    
    // reserve() - предварительное выделение памяти
    vec.reserve(100);  // Capacity >= 100, size не изменился
    std::cout << "After reserve(100): Size=" << vec.size() 
              << " Capacity=" << vec.capacity() << '\n';
    
    // Избегаем реаллокаций при известном размере
    std::vector<int> optimized;
    optimized.reserve(1000);  // Одна аллокация вместо множества
    for (int i = 0; i < 1000; ++i) {
        optimized.push_back(i);  // Без реаллокаций
    }
    
    // shrink_to_fit() - освобождение лишней памяти
    vec.shrink_to_fit();  // Capacity станет равна size (но не гарантировано)
    
    // emplace_back vs push_back
    struct Point { int x, y; Point(int x, int y) : x(x), y(y) {} };
    std::vector<Point> points;
    
    points.push_back(Point{1, 2});    // Создает временный объект, затем move
    points.emplace_back(3, 4);        // Конструирует in-place, эффективнее
    
    // Удаление элементов
    vec = {1, 2, 3, 4, 5};
    vec.erase(vec.begin() + 2);       // Удалить 3-й элемент (O(n))
    vec.erase(vec.begin(), vec.begin() + 2);  // Удалить диапазон
    
    // pop_back - эффективное удаление с конца
    vec.pop_back();  // O(1)
    
    // clear vs resize
    vec.clear();              // size = 0, capacity не изменилась
    vec.resize(10);           // size = 10, заполнено default values
    vec.resize(5, 99);        // size = 5, новые элементы = 99
}

void demo_vector_pitfalls() {
    std::cout << "\n=== Vector Pitfalls ===\n";
    
    // ОПАСНОСТЬ: Инвалидация итераторов при реаллокации
    std::vector<int> vec{1, 2, 3, 4, 5};
    auto it = vec.begin();
    
    vec.reserve(100);  // OK, capacity растет, но итераторы валидны если не было реаллокации
    
    vec.push_back(6);  // Может вызвать реаллокацию!
    // it теперь может быть невалидным!
    
    // ПРАВИЛЬНО: не храним итераторы при модификации
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << ' ';
    }
    
    // ОПАСНОСТЬ: vector<bool> - это НЕ контейнер bool'ов!
    std::vector<bool> bits{true, false, true};
    // bool& ref = bits[0];  // НЕ КОМПИЛИРУЕТСЯ!
    // vector<bool> хранит биты, возвращает прокси-объект
    
    // Используйте вместо этого:
    std::vector<char> bool_as_char;
    std::deque<bool> bool_deque;  // Настоящие bool'ы
}

// ============================================
// 📌 std::array - COMPILE-TIME FIXED SIZE
// ============================================

void demo_array() {
    std::cout << "\n=== std::array ===\n";
    
    // Фиксированный размер во время компиляции
    std::array<int, 5> arr1{1, 2, 3, 4, 5};
    std::array<int, 5> arr2 = {1, 2, 3};  // Остальные = 0
    
    // Stack allocation - нет динамической памяти
    std::array<int, 1000> large_arr;  // На стеке!
    
    // Совместим с C API
    int* c_array = arr1.data();
    
    // vs C массивы
    int c_arr[5] = {1, 2, 3, 4, 5};
    // c_arr.size();  // Ошибка - нет метода
    std::cout << "Size: " << arr1.size() << '\n';  // OK
    
    // Можно возвращать из функций (в отличие от C массивов)
    auto create_array = []() -> std::array<int, 3> {
        return {1, 2, 3};
    };
    
    // Безопасный доступ
    try {
        arr1.at(10);  // Бросит std::out_of_range
    } catch (const std::out_of_range&) {
        std::cout << "Out of range!\n";
    }
    
    // arr1[10];  // Undefined behavior (как C массивы)
    
    // Когда использовать:
    // ✓ Размер известен во время компиляции
    // ✓ Нужна производительность стека
    // ✓ Небольшой размер (не переполняйте stack)
}

// ============================================
// 📌 std::deque - DOUBLE-ENDED QUEUE
// ============================================

void demo_deque() {
    std::cout << "\n=== std::deque ===\n";
    
    std::deque<int> deq;
    
    // Эффективная вставка с обоих концов
    deq.push_back(1);     // O(1) в конец
    deq.push_front(0);    // O(1) в начало
    deq.push_back(2);
    // deq = {0, 1, 2}
    
    deq.pop_front();      // O(1) удаление с начала
    deq.pop_back();       // O(1) удаление с конца
    
    // Random access
    std::cout << "deq[0] = " << deq[0] << '\n';  // O(1)
    
    // Когда использовать deque вместо vector:
    // ✓ Нужны push/pop с обоих концов
    // ✓ Не нужна контигуэнтная память
    // ✗ Немного медленнее для итерации (не cache-friendly)
    // ✗ Больший memory overhead
    
    // Внутреннее устройство:
    // deque состоит из блоков (обычно 512 байт)
    // Блоки не обязательно контигуэнтны в памяти
}

// ============================================
// 📌 std::list / std::forward_list
// ============================================

void demo_list() {
    std::cout << "\n=== std::list (doubly-linked) ===\n";
    
    std::list<int> lst{1, 2, 3, 4, 5};
    
    // O(1) вставка/удаление в любом месте (с итератором)
    auto it = std::find(lst.begin(), lst.end(), 3);
    lst.insert(it, 99);  // O(1) вставка перед 3
    // lst = {1, 2, 99, 3, 4, 5}
    
    lst.erase(it);       // O(1) удаление элемента
    
    // Специфичные для list операции
    std::list<int> lst2{10, 20, 30};
    
    // splice - перенос элементов из другого list
    it = std::find(lst.begin(), lst.end(), 99);
    lst.splice(it, lst2);  // Вставляет весь lst2 перед 99
    // lst2 теперь пуст, элементы перенесены в lst
    
    // remove - удаляет все элементы со значением
    lst.remove(99);  // O(n)
    
    // unique - удаляет последовательные дубликаты
    lst = {1, 1, 2, 2, 2, 3, 3, 1};
    lst.unique();  // {1, 2, 3, 1}
    
    // sort - сортировка (НЕ используйте std::sort!)
    lst.sort();  // O(n log n), специализирована для list
    
    // Когда НЕ использовать list:
    // ✗ Нужен случайный доступ
    // ✗ Поиск элементов (O(n))
    // ✗ Cache-unfriendly
    // ✗ Большой memory overhead (2 указателя на элемент)
    
    std::cout << "\n=== std::forward_list (singly-linked) ===\n";
    
    std::forward_list<int> fwd{1, 2, 3, 4, 5};
    
    // Меньший memory overhead (1 указатель на элемент)
    // Но нет обратной итерации и некоторых операций
    
    fwd.push_front(0);   // O(1)
    // fwd.push_back();  // НЕТ ТАКОГО МЕТОДА!
    
    // Вставка ПОСЛЕ элемента (не перед!)
    auto fwd_it = fwd.begin();
    fwd.insert_after(fwd_it, 99);
    
    // Когда использовать:
    // ✓ Нужна минимальная память для linked list
    // ✓ Только forward iteration
}

// ============================================
// 📌 ASSOCIATIVE CONTAINERS - std::set
// ============================================

void demo_set() {
    std::cout << "\n=== std::set ===\n";
    
    std::set<int> s{5, 2, 8, 1, 9, 3};
    // Автоматическая сортировка: {1, 2, 3, 5, 8, 9}
    
    // Вставка
    auto [it, inserted] = s.insert(4);  // C++17 structured bindings
    std::cout << "Inserted: " << inserted << ", value: " << *it << '\n';
    
    auto [it2, inserted2] = s.insert(4);  // Дубликат
    std::cout << "Inserted: " << inserted2 << '\n';  // false
    
    // Поиск
    if (s.contains(5)) {  // C++20
        std::cout << "5 found\n";
    }
    
    // До C++20:
    if (s.find(5) != s.end()) {
        std::cout << "5 found\n";
    }
    
    // Удаление
    s.erase(3);           // O(log n)
    size_t removed = s.erase(100);  // Возвращает количество удаленных
    
    // Итерация в отсортированном порядке
    for (int val : s) {
        std::cout << val << ' ';
    }
    std::cout << '\n';
    
    // Custom comparator
    auto comp = [](int a, int b) { return a > b; };  // Обратный порядок
    std::set<int, decltype(comp)> desc_set(comp);
    desc_set.insert({1, 5, 3, 9, 2});
    // Порядок: {9, 5, 3, 2, 1}
    
    // Node handles (C++17) - перенос элементов без копирования
    std::set<int> s1{1, 2, 3};
    std::set<int> s2{4, 5, 6};
    
    auto node = s1.extract(2);  // Извлекаем узел
    s2.insert(std::move(node)); // Вставляем в другой set
    
    // merge - слияние двух sets
    s1.merge(s2);  // Переносит уникальные элементы из s2 в s1
    
    // multiset - позволяет дубликаты
    std::multiset<int> ms{1, 2, 2, 3, 3, 3};
    std::cout << "Count of 3: " << ms.count(3) << '\n';  // 3
}

// ============================================
// 📌 std::map
// ============================================

void demo_map() {
    std::cout << "\n=== std::map ===\n";
    
    std::map<std::string, int> ages;
    
    // Вставка: operator[]
    ages["Alice"] = 30;   // Вставляет если нет, иначе заменяет
    ages["Bob"] = 25;
    
    // operator[] создает элемент если его нет!
    std::cout << ages["Charlie"] << '\n';  // 0 (default int)
    // "Charlie" теперь в map!
    
    // insert - не заменяет существующие
    ages.insert({"Alice", 35});  // Не заменит, Alice остается 30
    
    auto [it, inserted] = ages.insert({"David", 40});
    std::cout << "Inserted David: " << inserted << '\n';
    
    // emplace - конструирует in-place
    ages.emplace("Eve", 28);
    
    // try_emplace (C++17) - не создает value если ключ есть
    ages.try_emplace("Alice", 35);  // Не создает 35, Alice уже есть
    ages.try_emplace("Frank", 32);  // Создает Frank
    
    // insert_or_assign (C++17) - вставить или заменить
    ages.insert_or_assign("Alice", 31);  // Заменяет!
    
    // Поиск
    if (auto it = ages.find("Bob"); it != ages.end()) {
        std::cout << "Bob's age: " << it->second << '\n';
    }
    
    // Безопасный доступ с at()
    try {
        std::cout << ages.at("Alice") << '\n';  // OK
        std::cout << ages.at("Unknown") << '\n';  // Бросит exception
    } catch (const std::out_of_range&) {
        std::cout << "Key not found\n";
    }
    
    // Итерация
    for (const auto& [name, age] : ages) {  // C++17 structured bindings
        std::cout << name << ": " << age << '\n';
    }
    
    // Custom key type
    struct Person {
        std::string name;
        int id;
        
        auto operator<=>(const Person&) const = default;  // C++20
    };
    
    std::map<Person, std::string> person_data;
    person_data[Person{"Alice", 1}] = "Engineer";
}

// ============================================
// 📌 UNORDERED CONTAINERS
// ============================================

void demo_unordered_map() {
    std::cout << "\n=== std::unordered_map ===\n";
    
    std::unordered_map<std::string, int> scores;
    
    // O(1) average операции
    scores["Alice"] = 100;
    scores["Bob"] = 85;
    scores["Charlie"] = 90;
    
    // Поиск O(1) average
    if (scores.contains("Alice")) {  // C++20
        std::cout << "Alice's score: " << scores["Alice"] << '\n';
    }
    
    // Нет гарантированного порядка!
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score << '\n';
    }
    
    // Custom hash function
    struct Point {
        int x, y;
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    struct PointHash {
        size_t operator()(const Point& p) const {
            return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
        }
    };
    
    std::unordered_map<Point, std::string, PointHash> point_names;
    point_names[Point{0, 0}] = "Origin";
    
    // Bucket interface - для диагностики hash таблицы
    std::cout << "Bucket count: " << scores.bucket_count() << '\n';
    std::cout << "Load factor: " << scores.load_factor() << '\n';
    std::cout << "Max load factor: " << scores.max_load_factor() << '\n';
    
    // Резервирование buckets
    scores.reserve(1000);  // Предотвращает rehashing
}

// ============================================
// 📌 CONTAINER ADAPTORS
// ============================================

void demo_adaptors() {
    std::cout << "\n=== Container Adaptors ===\n";
    
    // std::stack - LIFO
    std::stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    
    while (!stack.empty()) {
        std::cout << stack.top() << ' ';  // 3 2 1
        stack.pop();
    }
    std::cout << '\n';
    
    // std::queue - FIFO
    std::queue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    
    while (!queue.empty()) {
        std::cout << queue.front() << ' ';  // 1 2 3
        queue.pop();
    }
    std::cout << '\n';
    
    // std::priority_queue - max heap по умолчанию
    std::priority_queue<int> pq;
    pq.push(3);
    pq.push(1);
    pq.push(5);
    pq.push(2);
    
    while (!pq.empty()) {
        std::cout << pq.top() << ' ';  // 5 3 2 1 (наибольший первым)
        pq.pop();
    }
    std::cout << '\n';
    
    // Min heap
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
    min_heap.push(3);
    min_heap.push(1);
    min_heap.push(5);
    
    std::cout << "Min: " << min_heap.top() << '\n';  // 1
    
    // Кастомный underlying container
    std::stack<int, std::deque<int>> deque_stack;
    std::queue<int, std::list<int>> list_queue;
}

// ============================================
// 📌 std::span (C++20) - NON-OWNING VIEW
// ============================================

void demo_span() {
    std::cout << "\n=== std::span (C++20) ===\n";
    
    std::vector<int> vec{1, 2, 3, 4, 5};
    int arr[] = {10, 20, 30, 40, 50};
    
    // span - не владеет данными, только ссылается
    std::span<int> s1(vec);            // Из vector
    std::span<int> s2(arr);            // Из C array
    std::span<int> s3(vec.data(), 3);  // Первые 3 элемента
    
    // Модификация через span
    s1[0] = 100;  // Изменяет vec[0]!
    
    // Const span
    std::span<const int> readonly(vec);
    // readonly[0] = 1;  // Ошибка компиляции
    
    // Dynamic vs fixed extent
    std::span<int> dynamic_span(vec);          // Размер во время выполнения
    std::span<int, 5> fixed_span(arr);         // Размер в compile-time
    
    // Полезно для функций, принимающих разные контейнеры
    auto print_span = [](std::span<const int> s) {
        for (int val : s) {
            std::cout << val << ' ';
        }
        std::cout << '\n';
    };
    
    print_span(vec);  // vector
    print_span(arr);  // C array
    print_span(s3);   // другой span
    
    // Subspans
    auto first_three = s1.first(3);
    auto last_two = s1.last(2);
    auto middle = s1.subspan(1, 3);  // С индекса 1, 3 элемента
}

// ============================================
// 📌 std::string & std::string_view
// ============================================

void demo_string() {
    std::cout << "\n=== std::string ===\n";
    
    std::string s1 = "Hello";
    std::string s2 = " World";
    std::string s3 = s1 + s2;  // Конкатенация
    
    // SSO (Small String Optimization)
    // Короткие строки (обычно <= 15 символов) хранятся внутри объекта,
    // без динамической аллокации
    std::string small = "Short";     // Обычно без аллокации
    std::string large = "This is a very long string that won't fit in SSO";  // Аллокация
    
    // string_view (C++17) - non-owning view на строку
    std::string str = "Hello World";
    std::string_view view = str;  // Не копирует!
    std::string_view substr = view.substr(0, 5);  // "Hello", не копирует
    
    // ОПАСНОСТЬ: Dangling reference
    std::string_view get_view() {
        std::string temp = "Temporary";
        return temp;  // ОПАСНО! temp уничтожится
    }
    // auto v = get_view();  // v указывает на уничтоженную строку!
    
    // Функции с string_view эффективнее
    auto process = [](std::string_view sv) {
        std::cout << "Processing: " << sv << '\n';
    };
    
    process("Literal");  // Без копирования
    process(str);        // Без копирования
    process(str.substr(0, 5));  // substr создает string - копирование!
    
    // Правильно для substr:
    process(std::string_view(str).substr(0, 5));  // Без копирования
}

// ════════════════════════════════════════════════════════════════════════════════════
// PERFORMANCE COMPARISON (СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ)
// ════════════════════════════════════════════════════════════════════════════════════

void performance_tips() {
    std::cout << "\n=== Performance Tips ===\n";
    
    /*
     * КОГДА ИСПОЛЬЗОВАТЬ КАКОЙ КОНТЕЙНЕР:
     * 
     * vector - DEFAULT CHOICE для большинства случаев
     *   ✓ Cache-friendly (контигуэнтная память)
     *   ✓ Минимальный overhead
     *   ✓ Быстрый random access
     *   ✗ Медленная вставка в начало/середину
     * 
     * deque - когда нужна вставка с обоих концов
     *   ✓ O(1) push/pop front и back
     *   ✓ Random access
     *   ✗ Не контигуэнтная память
     *   ✗ Больший overhead чем vector
     * 
     * list - РЕДКО (почти всегда vector лучше)
     *   ✓ O(1) вставка/удаление в середине (с итератором)
     *   ✗ Очень медленный поиск
     *   ✗ Cache-unfriendly
     *   ✗ Большой memory overhead
     * 
     * set/map - когда нужна сортировка + поиск
     *   ✓ O(log n) операции
     *   ✓ Автоматическая сортировка
     *   ✗ Медленнее чем unordered для больших данных
     * 
     * unordered_set/map - DEFAULT для hash tables
     *   ✓ O(1) average операции
     *   ✓ Быстрее set/map для больших данных
     *   ✗ Нет порядка
     *   ✗ Worst case O(n) при плохой hash функции
     * 
     * array - когда размер известен в compile-time
     *   ✓ Нет overhead
     *   ✓ Stack allocation
     *   ✗ Фиксированный размер
     * 
     * span - для view на существующие данные
     *   ✓ Нет копирования
     *   ✓ Унифицированный интерфейс
     *   ✗ Не владеет данными (следите за lifetime)
     */
    
    // Правило большого пальца:
    // 1. По умолчанию используйте vector
    // 2. Если нужен поиск по ключу → unordered_map
    // 3. Если нужна сортировка → set/map
    // 4. Если нужна вставка с обоих концов → deque
    // 5. Список используйте РЕДКО (только если действительно нужен)
}


