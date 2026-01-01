/*
 * ============================================
 * C++ RANGES БИБЛИОТЕКА - ПОЛНОЕ РУКОВОДСТВО
 * ============================================
 * 
 * Современная работа с последовательностями в C++20/23:
 * - Views (ленивое вычисление)
 * - Range algorithms с проекциями
 * - Композиция через pipe operator
 * - C++23 новые views
 * - Создание custom ranges
 * 
 * Компиляция: g++ -std=c++23 ranges_complete.cpp
 */

#include <iostream>
#include <ranges>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <numeric>
#include <functional>

namespace rng = std::ranges;
namespace vw = std::views;

// ============================================
// 📌 RANGES BASICS
// ============================================

namespace ranges_basics {

void demo_range_concept() {
    std::cout << "=== Range Concept ===\n";
    
    // Range - все, что имеет begin() и end()
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // Range algorithm вместо iterator-based
    auto it = rng::find(vec, 3);  // Передаем весь range!
    if (it != vec.end()) {
        std::cout << "Found: " << *it << '\n';
    }
    
    // Iterator-sentinel pair
    auto even = [](int x) { return x % 2 == 0; };
    auto first_even = rng::find_if(vec, even);
    std::cout << "First even: " << *first_even << '\n';
}

void demo_borrowed_ranges() {
    std::cout << "\n=== Borrowed Ranges ===\n";
    
    // ✅ Borrowed range - безопасно возвращать итераторы
    auto get_view() {
        std::vector<int> vec = {1, 2, 3};
        // views::all создает view (легковесный)
        return vw::all(vec);  // ❌ Опасно! vec уничтожается
    }
    
    // Правильно: возвращать owning range
    auto get_vector() {
        return std::vector{1, 2, 3};  // ✅ Безопасно
    }
}

void demo() {
    std::cout << "=== Ranges Basics ===\n";
    demo_range_concept();
    demo_borrowed_ranges();
}

} // namespace ranges_basics

// ============================================
// 📌 FACTORY VIEWS
// ============================================

namespace factory_views {

void demo() {
    std::cout << "\n=== Factory Views ===\n";
    
    // empty<T> - пустой view
    auto empty = vw::empty<int>;
    std::cout << "Empty size: " << rng::distance(empty) << '\n';
    
    // single - один элемент
    auto single = vw::single(42);
    std::cout << "Single: ";
    for (auto x : single) std::cout << x << ' ';
    std::cout << '\n';
    
    // iota - бесконечная последовательность
    auto first_10 = vw::iota(1) | vw::take(10);
    std::cout << "First 10: ";
    for (auto x : first_10) std::cout << x << ' ';
    std::cout << '\n';
    
    // iota с границами
    auto range_1_to_5 = vw::iota(1, 6);
    std::cout << "1 to 5: ";
    for (auto x : range_1_to_5) std::cout << x << ' ';
    std::cout << '\n';
    
    // repeat (C++23) - повторяющееся значение
    #ifdef __cpp_lib_ranges_repeat
    auto repeated = vw::repeat(7) | vw::take(5);
    std::cout << "Repeat 7: ";
    for (auto x : repeated) std::cout << x << ' ';
    std::cout << '\n';
    #endif
}

} // namespace factory_views

// ============================================
// 📌 ADAPTOR VIEWS
// ============================================

namespace adaptor_views {

void demo_filter_transform() {
    std::cout << "\n=== Filter & Transform ===\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // filter - только четные
    auto evens = numbers | vw::filter([](int x) { return x % 2 == 0; });
    std::cout << "Evens: ";
    for (auto x : evens) std::cout << x << ' ';
    std::cout << '\n';
    
    // transform - квадраты
    auto squares = numbers | vw::transform([](int x) { return x * x; });
    std::cout << "Squares: ";
    for (auto x : squares) std::cout << x << ' ';
    std::cout << '\n';
    
    // Композиция: четные квадраты
    auto even_squares = numbers 
        | vw::filter([](int x) { return x % 2 == 0; })
        | vw::transform([](int x) { return x * x; });
    
    std::cout << "Even squares: ";
    for (auto x : even_squares) std::cout << x << ' ';
    std::cout << '\n';
}

void demo_take_drop() {
    std::cout << "\n=== Take & Drop ===\n";
    
    auto nums = vw::iota(1, 20);
    
    // take - первые N
    auto first_5 = nums | vw::take(5);
    std::cout << "First 5: ";
    for (auto x : first_5) std::cout << x << ' ';
    std::cout << '\n';
    
    // drop - пропустить N
    auto skip_5 = nums | vw::drop(5);
    std::cout << "Skip 5: ";
    for (auto x : skip_5 | vw::take(5)) std::cout << x << ' ';
    std::cout << '\n';
    
    // take_while - пока условие истинно
    auto less_than_7 = nums | vw::take_while([](int x) { return x < 7; });
    std::cout << "< 7: ";
    for (auto x : less_than_7) std::cout << x << ' ';
    std::cout << '\n';
    
    // drop_while - пропустить пока условие истинно
    auto from_7 = nums | vw::drop_while([](int x) { return x < 7; });
    std::cout << "From 7: ";
    for (auto x : from_7 | vw::take(3)) std::cout << x << ' ';
    std::cout << '\n';
}

void demo_reverse_elements() {
    std::cout << "\n=== Reverse & Elements ===\n";
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // reverse
    auto rev = vec | vw::reverse;
    std::cout << "Reversed: ";
    for (auto x : rev) std::cout << x << ' ';
    std::cout << '\n';
    
    // elements<N> - N-й элемент tuple
    std::vector<std::tuple<int, std::string, double>> data = {
        {1, "one", 1.1},
        {2, "two", 2.2},
        {3, "three", 3.3}
    };
    
    auto first_elements = data | vw::elements<0>;
    std::cout << "First elements: ";
    for (auto x : first_elements) std::cout << x << ' ';
    std::cout << '\n';
}

void demo_keys_values() {
    std::cout << "\n=== Keys & Values ===\n";
    
    std::map<std::string, int> ages = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };
    
    // keys
    auto names = ages | vw::keys;
    std::cout << "Names: ";
    for (const auto& name : names) std::cout << name << ' ';
    std::cout << '\n';
    
    // values
    auto ages_only = ages | vw::values;
    std::cout << "Ages: ";
    for (auto age : ages_only) std::cout << age << ' ';
    std::cout << '\n';
}

void demo_join_split() {
    std::cout << "\n=== Join & Split ===\n";
    
    // join - объединяет range of ranges
    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4}, {5, 6}};
    auto flattened = nested | vw::join;
    std::cout << "Joined: ";
    for (auto x : flattened) std::cout << x << ' ';
    std::cout << '\n';
    
    // split - разбивает по делимитеру
    std::string text = "hello,world,cpp";
    auto parts = text | vw::split(',');
    std::cout << "Split parts:\n";
    for (const auto& part : parts) {
        for (char c : part) std::cout << c;
        std::cout << '\n';
    }
}

void demo() {
    demo_filter_transform();
    demo_take_drop();
    demo_reverse_elements();
    demo_keys_values();
    demo_join_split();
}

} // namespace adaptor_views

// ============================================
// 📌 C++23 NEW VIEWS
// ============================================

namespace cpp23_views {

void demo_zip() {
    std::cout << "\n=== Zip (C++23) ===\n";
    
    #ifdef __cpp_lib_ranges_zip
    std::vector<int> ids = {1, 2, 3};
    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};
    std::vector<int> ages = {30, 25, 35};
    
    // zip - объединяет несколько ranges
    auto zipped = vw::zip(ids, names, ages);
    for (auto [id, name, age] : zipped) {
        std::cout << id << ": " << name << " (" << age << ")\n";
    }
    
    // zip_transform
    auto sum_pairs = vw::zip_transform(
        std::plus<>{},
        std::vector{1, 2, 3},
        std::vector{10, 20, 30}
    );
    std::cout << "Sums: ";
    for (auto x : sum_pairs) std::cout << x << ' ';
    std::cout << '\n';
    #else
    std::cout << "zip not available (C++23)\n";
    #endif
}

void demo_adjacent() {
    std::cout << "\n=== Adjacent (C++23) ===\n";
    
    #ifdef __cpp_lib_ranges_adjacent
    std::vector<int> nums = {1, 2, 3, 4, 5};
    
    // adjacent<N> - окно из N элементов
    auto pairs = nums | vw::adjacent<2>;
    std::cout << "Adjacent pairs:\n";
    for (auto [a, b] : pairs) {
        std::cout << "(" << a << ", " << b << ")\n";
    }
    
    // adjacent_transform - применяет функцию
    auto sums = nums | vw::adjacent_transform<2>([](int a, int b) {
        return a + b;
    });
    std::cout << "Sums: ";
    for (auto x : sums) std::cout << x << ' ';
    std::cout << '\n';
    #else
    std::cout << "adjacent not available (C++23)\n";
    #endif
}

void demo_chunk_slide() {
    std::cout << "\n=== Chunk & Slide (C++23) ===\n";
    
    #ifdef __cpp_lib_ranges_chunk
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // chunk - разбивает на куски
    auto chunks = nums | vw::chunk(3);
    std::cout << "Chunks of 3:\n";
    for (auto chunk : chunks) {
        for (auto x : chunk) std::cout << x << ' ';
        std::cout << '\n';
    }
    
    // slide - скользящее окно
    auto windows = nums | vw::slide(3);
    std::cout << "Sliding window 3:\n";
    for (auto window : windows) {
        for (auto x : window) std::cout << x << ' ';
        std::cout << '\n';
    }
    #else
    std::cout << "chunk/slide not available (C++23)\n";
    #endif
}

void demo_enumerate() {
    std::cout << "\n=== Enumerate (C++23) ===\n";
    
    #ifdef __cpp_lib_ranges_enumerate
    std::vector<std::string> fruits = {"apple", "banana", "cherry"};
    
    // enumerate - добавляет индексы
    auto indexed = fruits | vw::enumerate;
    for (auto [idx, fruit] : indexed) {
        std::cout << idx << ": " << fruit << '\n';
    }
    #else
    std::cout << "enumerate not available (C++23)\n";
    
    // Workaround для C++20
    std::vector<std::string> fruits = {"apple", "banana", "cherry"};
    auto indexed = vw::zip(vw::iota(0), fruits);
    for (auto [idx, fruit] : indexed) {
        std::cout << idx << ": " << fruit << '\n';
    }
    #endif
}

void demo() {
    demo_zip();
    demo_adjacent();
    demo_chunk_slide();
    demo_enumerate();
}

} // namespace cpp23_views

// ============================================
// 📌 RANGE ALGORITHMS
// ============================================

namespace range_algorithms {

void demo_basic_algorithms() {
    std::cout << "\n=== Range Algorithms ===\n";
    
    std::vector<int> vec = {5, 2, 8, 1, 9, 3, 7};
    
    // sort
    rng::sort(vec);
    std::cout << "Sorted: ";
    for (auto x : vec) std::cout << x << ' ';
    std::cout << '\n';
    
    // find
    auto it = rng::find(vec, 8);
    if (it != vec.end()) {
        std::cout << "Found 8 at position: " 
                  << std::distance(vec.begin(), it) << '\n';
    }
    
    // count_if
    auto count = rng::count_if(vec, [](int x) { return x > 5; });
    std::cout << "Count > 5: " << count << '\n';
    
    // any_of / all_of / none_of
    std::cout << "Any > 10: " << std::boolalpha 
              << rng::any_of(vec, [](int x) { return x > 10; }) << '\n';
    std::cout << "All > 0: " 
              << rng::all_of(vec, [](int x) { return x > 0; }) << '\n';
}

void demo_projections() {
    std::cout << "\n=== Projections ===\n";
    
    struct Person {
        std::string name;
        int age;
    };
    
    std::vector<Person> people = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };
    
    // Сортировка по возрасту с помощью проекции
    rng::sort(people, {}, &Person::age);
    
    std::cout << "Sorted by age:\n";
    for (const auto& p : people) {
        std::cout << p.name << ": " << p.age << '\n';
    }
    
    // Поиск с проекцией
    auto it = rng::find(people, "Bob", &Person::name);
    if (it != people.end()) {
        std::cout << "Found Bob, age: " << it->age << '\n';
    }
}

void demo_transform_copy() {
    std::cout << "\n=== Transform & Copy ===\n";
    
    std::vector<int> src = {1, 2, 3, 4, 5};
    std::vector<int> dst;
    
    // copy_if
    rng::copy_if(src, std::back_inserter(dst), 
                 [](int x) { return x % 2 == 0; });
    std::cout << "Even numbers: ";
    for (auto x : dst) std::cout << x << ' ';
    std::cout << '\n';
    
    // transform
    std::vector<int> squares;
    rng::transform(src, std::back_inserter(squares),
                   [](int x) { return x * x; });
    std::cout << "Squares: ";
    for (auto x : squares) std::cout << x << ' ';
    std::cout << '\n';
}

void demo() {
    demo_basic_algorithms();
    demo_projections();
    demo_transform_copy();
}

} // namespace range_algorithms

// ============================================
// 📌 RANGE COMPOSITION
// ============================================

namespace range_composition {

void demo_pipe_operator() {
    std::cout << "\n=== Pipe Operator ===\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Цепочка операций
    auto result = numbers
        | vw::filter([](int x) { return x % 2 == 0; })  // Четные
        | vw::transform([](int x) { return x * x; })    // Квадраты
        | vw::take(3);                                  // Первые 3
    
    std::cout << "First 3 even squares: ";
    for (auto x : result) std::cout << x << ' ';
    std::cout << '\n';
}

void demo_lazy_evaluation() {
    std::cout << "\n=== Lazy Evaluation ===\n";
    
    int counter = 0;
    auto expensive = [&counter](int x) {
        ++counter;
        std::cout << "Processing " << x << '\n';
        return x * 2;
    };
    
    auto nums = vw::iota(1, 10);
    auto view = nums | vw::transform(expensive);
    
    std::cout << "View created, counter = " << counter << '\n';
    
    // Вычисление происходит только при итерации!
    std::cout << "Taking first 3:\n";
    for (auto x : view | vw::take(3)) {
        std::cout << "Got: " << x << '\n';
    }
    
    std::cout << "Total processed: " << counter << '\n';
}

void demo_performance() {
    std::cout << "\n=== Performance Considerations ===\n";
    
    std::vector<int> data(10000);
    std::iota(data.begin(), data.end(), 1);
    
    // ✅ Ленивое вычисление - эффективно
    auto view = data
        | vw::filter([](int x) { return x % 2 == 0; })
        | vw::transform([](int x) { return x * x; })
        | vw::take(10);
    
    // Обрабатывается только то, что нужно!
    int sum = 0;
    for (auto x : view) {
        sum += x;
    }
    std::cout << "Sum of first 10 even squares: " << sum << '\n';
    
    // ❌ Eager материализация - может быть медленнее
    std::vector<int> temp1, temp2;
    rng::copy_if(data, std::back_inserter(temp1),
                 [](int x) { return x % 2 == 0; });
    rng::transform(temp1, std::back_inserter(temp2),
                   [](int x) { return x * x; });
    
    std::cout << "Performance tip: views избегают промежуточных копий\n";
}

void demo() {
    demo_pipe_operator();
    demo_lazy_evaluation();
    demo_performance();
}

} // namespace range_composition

// ============================================
// 📌 CUSTOM RANGES
// ============================================

namespace custom_ranges {

// Простой custom view - числа Фибоначчи
class fibonacci_view : public rng::view_interface<fibonacci_view> {
    struct iterator {
        using value_type = size_t;
        using difference_type = std::ptrdiff_t;
        
        size_t a = 0, b = 1;
        size_t count = 0;
        size_t max_count;
        
        iterator(size_t max) : max_count(max) {}
        
        size_t operator*() const { return a; }
        
        iterator& operator++() {
            auto next = a + b;
            a = b;
            b = next;
            ++count;
            return *this;
        }
        
        iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator==(const iterator& other) const {
            return count == other.count;
        }
        
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };
    
    size_t count_;
    
public:
    fibonacci_view(size_t n) : count_(n) {}
    
    auto begin() const { return iterator(count_); }
    auto end() const { 
        iterator it(count_);
        it.count = count_;
        return it;
    }
};

// Фабричная функция
auto fibonacci(size_t n) {
    return fibonacci_view(n);
}

void demo() {
    std::cout << "\n=== Custom Ranges ===\n";
    
    std::cout << "First 10 Fibonacci numbers: ";
    for (auto x : fibonacci(10)) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
    
    // Можно использовать с ranges!
    auto fib_squares = fibonacci(10)
        | vw::transform([](size_t x) { return x * x; })
        | vw::take(5);
    
    std::cout << "First 5 Fibonacci squares: ";
    for (auto x : fib_squares) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

} // namespace custom_ranges

// ============================================
// 📌 RANGES TO CONTAINERS (C++23)
// ============================================

namespace ranges_to_containers {

void demo() {
    std::cout << "\n=== Ranges to Containers ===\n";
    
    #ifdef __cpp_lib_ranges_to_container
    auto view = vw::iota(1, 10)
        | vw::filter([](int x) { return x % 2 == 0; })
        | vw::transform([](int x) { return x * x; });
    
    // Материализация в контейнер
    auto vec = view | rng::to<std::vector>();
    auto set = view | rng::to<std::set>();
    
    std::cout << "Vector: ";
    for (auto x : vec) std::cout << x << ' ';
    std::cout << '\n';
    #else
    std::cout << "ranges::to not available (C++23)\n";
    
    // Workaround для C++20
    auto view = vw::iota(1, 10)
        | vw::filter([](int x) { return x % 2 == 0; })
        | vw::transform([](int x) { return x * x; });
    
    std::vector<int> vec;
    rng::copy(view, std::back_inserter(vec));
    
    std::cout << "Vector (C++20 way): ";
    for (auto x : vec) std::cout << x << ' ';
    std::cout << '\n';
    #endif
}

} // namespace ranges_to_containers

// ============================================
// 📌 PRACTICAL EXAMPLES
// ============================================

namespace practical_examples {

void demo_data_processing() {
    std::cout << "\n=== Data Processing Pipeline ===\n";
    
    struct Sale {
        std::string product;
        double price;
        int quantity;
    };
    
    std::vector<Sale> sales = {
        {"Laptop", 1200.0, 2},
        {"Mouse", 25.0, 10},
        {"Keyboard", 75.0, 5},
        {"Monitor", 300.0, 3},
        {"Headphones", 50.0, 8}
    };
    
    // Найти топ-3 продукта по выручке
    auto top_revenue = sales
        | vw::transform([](const Sale& s) {
            return std::pair{s.product, s.price * s.quantity};
          })
        | vw::take(3);  // В реальности нужна сортировка
    
    std::cout << "Top products:\n";
    for (const auto& [product, revenue] : top_revenue) {
        std::cout << product << ": $" << revenue << '\n';
    }
}

void demo_string_processing() {
    std::cout << "\n=== String Processing ===\n";
    
    std::string text = "hello world from cpp ranges";
    
    // Подсчет длинных слов
    auto words = text | vw::split(' ');
    int long_words = rng::count_if(words, [](auto word) {
        return rng::distance(word) > 4;
    });
    
    std::cout << "Words longer than 4 chars: " << long_words << '\n';
    
    // Заглавные буквы
    auto uppercase = text
        | vw::transform([](char c) { return std::toupper(c); });
    
    std::cout << "Uppercase: ";
    for (char c : uppercase) std::cout << c;
    std::cout << '\n';
}

void demo_functional_patterns() {
    std::cout << "\n=== Functional Programming ===\n";
    
    // Map-filter-reduce pattern
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Sum of squares of even numbers
    auto sum = rng::fold_left(
        numbers
            | vw::filter([](int x) { return x % 2 == 0; })
            | vw::transform([](int x) { return x * x; }),
        0,
        std::plus<>{}
    );
    
    std::cout << "Sum of even squares: " << sum << '\n';
    
    // Аналог: numbers.filter(even).map(square).reduce(+)
}

void demo() {
    demo_data_processing();
    demo_string_processing();
    demo_functional_patterns();
}

} // namespace practical_examples

// ============================================
// 📌 BEST PRACTICES
// ============================================

/*
 * BEST PRACTICES ДЛЯ RANGES:
 * 
 * 1. ИСПОЛЬЗУЙ VIEWS ДЛЯ КОМПОЗИЦИИ
 *    ✅ auto result = data | filter(...) | transform(...)
 *    ❌ vector<T> temp; copy_if(...); transform(...)
 * 
 * 2. МАТЕРИАЛИЗУЙ ТОЛЬКО КОГДА НУЖНО
 *    ✅ for (auto x : view) { ... }
 *    ❌ auto vec = view | ranges::to<vector>(); // без нужды
 * 
 * 3. ПОМНИ О LIFETIME
 *    ❌ auto view = get_temp_vector() | vw::all; // dangling!
 *    ✅ auto vec = get_vector(); auto view = vec | vw::all;
 * 
 * 4. ИСПОЛЬЗУЙ PROJECTIONS
 *    ✅ ranges::sort(people, {}, &Person::age);
 *    ❌ ranges::sort(people, [](auto& a, auto& b) {...});
 * 
 * 5. PIPE ДЛЯ ЧИТАЕМОСТИ
 *    ✅ data | filter(...) | transform(...) | take(10)
 *    ❌ take(transform(filter(data, ...), ...), 10)
 * 
 * 6. ЛЕНИВОСТЬ = ПРОИЗВОДИТЕЛЬНОСТЬ
 *    - Views не создают промежуточные контейнеры
 *    - Вычисление on-demand
 *    - Отлично для больших данных
 * 
 * 7. CUSTOM VIEWS ДЛЯ ПЕРЕИСПОЛЬЗОВАНИЯ
 *    - Наследуй view_interface
 *    - Реализуй begin()/end()
 *    - Интегрируется с ranges
 */

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    std::cout << "=== C++ Ranges Complete Guide ===\n";
    
    ranges_basics::demo();
    factory_views::demo();
    adaptor_views::demo();
    cpp23_views::demo();
    range_algorithms::demo();
    range_composition::demo();
    custom_ranges::demo();
    ranges_to_containers::demo();
    practical_examples::demo();
    
    std::cout << "\n=== Резюме ===\n";
    std::cout << "✓ Views - ленивое вычисление без копий\n";
    std::cout << "✓ Pipe operator | для композиции\n";
    std::cout << "✓ Range algorithms с проекциями\n";
    std::cout << "✓ C++23: zip, enumerate, chunk, slide\n";
    std::cout << "✓ Функциональный стиль программирования\n";
    std::cout << "✓ Производительность через lazy evaluation\n";
    std::cout << "✓ Читаемость через декларативный стиль\n";
    
    return 0;
}