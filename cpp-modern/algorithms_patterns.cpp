/*
 * ============================================
 * ПАТТЕРНЫ STL АЛГОРИТМОВ (C++20/23)
 * ============================================
 * 
 * Справочник по всем основным алгоритмам STL
 * с примерами использования и паттернами.
 * 
 * Требования: C++20 или выше
 * Компиляция: g++ -std=c++20 algorithms_patterns.cpp
 */

#include <algorithm>      // Основные алгоритмы
#include <numeric>        // Численные алгоритмы
#include <ranges>         // C++20 ranges
#include <execution>      // Parallel execution policies
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <random>
#include <iterator>
#include <functional>

// ============================================
// 📌 NON-MODIFYING SEQUENCE OPERATIONS
// ============================================

void demo_find_algorithms() {
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // find - поиск первого элемента
    auto it = std::find(vec.begin(), vec.end(), 5);
    if (it != vec.end()) {
        std::cout << "Найден: " << *it << '\n';
    }
    
    // find_if - поиск с предикатом
    auto it2 = std::find_if(vec.begin(), vec.end(), 
        [](int x) { return x > 5; });
    std::cout << "Первый > 5: " << *it2 << '\n';
    
    // find_if_not - поиск элемента, НЕ удовлетворяющего предикату
    auto it3 = std::find_if_not(vec.begin(), vec.end(),
        [](int x) { return x < 5; });
    std::cout << "Первый НЕ < 5: " << *it3 << '\n';
    
    // adjacent_find - поиск двух соседних одинаковых элементов
    std::vector<int> vec2{1, 2, 2, 3, 4, 4, 5};
    auto adj = std::adjacent_find(vec2.begin(), vec2.end());
    if (adj != vec2.end()) {
        std::cout << "Дубликаты: " << *adj << '\n';
    }
    
    // search - поиск подпоследовательности
    std::vector<int> pattern{3, 4, 5};
    auto found = std::search(vec.begin(), vec.end(), 
                            pattern.begin(), pattern.end());
    std::cout << "Паттерн найден на позиции: " 
              << std::distance(vec.begin(), found) << '\n';
    
    // search_n - поиск n последовательных одинаковых элементов
    std::vector<int> vec3{1, 2, 3, 3, 3, 4, 5};
    auto three = std::search_n(vec3.begin(), vec3.end(), 3, 3);
    std::cout << "Три тройки начинаются с позиции: "
              << std::distance(vec3.begin(), three) << '\n';
}

void demo_count_algorithms() {
    std::vector<int> vec{1, 2, 3, 2, 4, 2, 5};
    
    // count - подсчет элементов
    auto cnt = std::count(vec.begin(), vec.end(), 2);
    std::cout << "Количество 2: " << cnt << '\n';
    
    // count_if - подсчет с предикатом
    auto even_cnt = std::count_if(vec.begin(), vec.end(),
        [](int x) { return x % 2 == 0; });
    std::cout << "Четных чисел: " << even_cnt << '\n';
}

void demo_predicates() {
    std::vector<int> vec{2, 4, 6, 8, 10};
    std::vector<int> vec2{1, 3, 5};
    
    // all_of - все элементы удовлетворяют условию
    bool all_even = std::all_of(vec.begin(), vec.end(),
        [](int x) { return x % 2 == 0; });
    std::cout << "Все четные: " << all_even << '\n';
    
    // any_of - хотя бы один элемент удовлетворяет условию
    bool has_even = std::any_of(vec2.begin(), vec2.end(),
        [](int x) { return x % 2 == 0; });
    std::cout << "Есть четные: " << has_even << '\n';
    
    // none_of - ни один элемент не удовлетворяет условию
    bool no_even = std::none_of(vec2.begin(), vec2.end(),
        [](int x) { return x % 2 == 0; });
    std::cout << "Нет четных: " << no_even << '\n';
}

void demo_comparison() {
    std::vector<int> vec1{1, 2, 3, 4, 5};
    std::vector<int> vec2{1, 2, 3, 4, 5};
    std::vector<int> vec3{1, 2, 9, 4, 5};
    
    // equal - проверка равенства последовательностей
    bool eq = std::equal(vec1.begin(), vec1.end(), vec2.begin());
    std::cout << "vec1 == vec2: " << eq << '\n';
    
    // mismatch - поиск первого различия
    auto [it1, it3] = std::mismatch(vec1.begin(), vec1.end(), 
                                     vec3.begin());
    if (it1 != vec1.end()) {
        std::cout << "Различие: " << *it1 << " vs " << *it3 << '\n';
    }
    
    // is_permutation - проверка, является ли одна последовательность
    // перестановкой другой
    std::vector<int> perm{5, 4, 3, 2, 1};
    bool is_perm = std::is_permutation(vec1.begin(), vec1.end(),
                                       perm.begin());
    std::cout << "Является перестановкой: " << is_perm << '\n';
}

// ============================================
// 📌 MODIFYING SEQUENCE OPERATIONS
// ============================================

void demo_copy_move() {
    std::vector<int> src{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> dst;
    
    // copy - копирование всех элементов
    std::copy(src.begin(), src.end(), std::back_inserter(dst));
    
    // copy_if - копирование с фильтрацией
    std::vector<int> evens;
    std::copy_if(src.begin(), src.end(), std::back_inserter(evens),
        [](int x) { return x % 2 == 0; });
    
    // copy_n - копирование первых n элементов
    std::vector<int> first_three;
    std::copy_n(src.begin(), 3, std::back_inserter(first_three));
    
    // copy_backward - копирование в обратном порядке итерирования
    std::vector<int> backward(src.size());
    std::copy_backward(src.begin(), src.end(), backward.end());
    
    // move - перемещение элементов (полезно для move-only типов)
    std::vector<std::string> str_src{"hello", "world"};
    std::vector<std::string> str_dst;
    std::move(str_src.begin(), str_src.end(), 
              std::back_inserter(str_dst));
    // str_src теперь содержит пустые строки
}

void demo_transform() {
    std::vector<int> vec{1, 2, 3, 4, 5};
    std::vector<int> result;
    
    // transform - применение функции к каждому элементу
    std::transform(vec.begin(), vec.end(), std::back_inserter(result),
        [](int x) { return x * x; });  // Квадраты чисел
    
    // transform с двумя входными последовательностями
    std::vector<int> vec2{10, 20, 30, 40, 50};
    std::vector<int> sums;
    std::transform(vec.begin(), vec.end(), vec2.begin(),
                   std::back_inserter(sums),
                   [](int a, int b) { return a + b; });
}

void demo_replace() {
    std::vector<int> vec{1, 2, 3, 2, 4, 2, 5};
    
    // replace - замена значений
    std::replace(vec.begin(), vec.end(), 2, 99);
    // vec = {1, 99, 3, 99, 4, 99, 5}
    
    // replace_if - замена с предикатом
    std::vector<int> vec2{1, 2, 3, 4, 5, 6};
    std::replace_if(vec2.begin(), vec2.end(),
        [](int x) { return x % 2 == 0; }, 0);
    // Все четные заменены на 0
    
    // replace_copy - замена с копированием в новый контейнер
    std::vector<int> vec3{1, 2, 3, 2, 4};
    std::vector<int> result;
    std::replace_copy(vec3.begin(), vec3.end(),
                      std::back_inserter(result), 2, 99);
    // vec3 не изменен, result содержит измененную копию
}

void demo_fill_generate() {
    // fill - заполнение значением
    std::vector<int> vec(5);
    std::fill(vec.begin(), vec.end(), 42);
    // vec = {42, 42, 42, 42, 42}
    
    // fill_n - заполнение n элементов
    std::vector<int> vec2(10);
    std::fill_n(vec2.begin(), 5, 7);
    // Первые 5 элементов = 7
    
    // generate - заполнение результатом вызова функции
    std::vector<int> vec3(5);
    int n = 0;
    std::generate(vec3.begin(), vec3.end(), [&n] { return n++; });
    // vec3 = {0, 1, 2, 3, 4}
    
    // generate_n - генерация n элементов
    std::vector<int> random_nums;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    std::generate_n(std::back_inserter(random_nums), 10,
        [&] { return dis(gen); });
}

void demo_remove_unique() {
    // remove - НЕ удаляет элементы, а перемещает их в конец!
    // Возвращает итератор на начало "мусора"
    std::vector<int> vec{1, 2, 3, 2, 4, 2, 5};
    auto new_end = std::remove(vec.begin(), vec.end(), 2);
    // vec = {1, 3, 4, 5, ?, ?, ?} - "мусор" в конце
    
    // Правильный способ удаления - erase-remove идиома
    vec.erase(new_end, vec.end());
    // Теперь vec = {1, 3, 4, 5}
    
    // remove_if с erase
    std::vector<int> vec2{1, 2, 3, 4, 5, 6, 7, 8};
    vec2.erase(
        std::remove_if(vec2.begin(), vec2.end(),
            [](int x) { return x % 2 == 0; }),
        vec2.end()
    );
    // vec2 теперь содержит только нечетные числа
    
    // unique - удаление последовательных дубликатов
    // (обычно используется после sort)
    std::vector<int> vec3{1, 1, 2, 2, 2, 3, 3, 4, 5, 5};
    vec3.erase(
        std::unique(vec3.begin(), vec3.end()),
        vec3.end()
    );
    // vec3 = {1, 2, 3, 4, 5}
}

void demo_reverse_rotate() {
    // reverse - разворот последовательности
    std::vector<int> vec{1, 2, 3, 4, 5};
    std::reverse(vec.begin(), vec.end());
    // vec = {5, 4, 3, 2, 1}
    
    // rotate - циклический сдвиг
    std::vector<int> vec2{1, 2, 3, 4, 5};
    std::rotate(vec2.begin(), vec2.begin() + 2, vec2.end());
    // vec2 = {3, 4, 5, 1, 2}
    // Элемент на позиции begin()+2 стал первым
    
    // Практический пример: сдвиг влево на 1
    std::vector<int> vec3{1, 2, 3, 4, 5};
    std::rotate(vec3.begin(), vec3.begin() + 1, vec3.end());
    // vec3 = {2, 3, 4, 5, 1}
}

void demo_shuffle_sample() {
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // shuffle - случайная перестановка
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(vec.begin(), vec.end(), gen);
    // vec теперь в случайном порядке
    
    // sample (C++17) - случайная выборка n элементов
    std::vector<int> full_deck(52);
    std::iota(full_deck.begin(), full_deck.end(), 1); // 1..52
    std::vector<int> hand;
    std::sample(full_deck.begin(), full_deck.end(),
                std::back_inserter(hand), 5, gen);
    // hand содержит 5 случайных карт
}

// ============================================
// 📌 SORTING OPERATIONS
// ============================================

void demo_sorting() {
    // sort - быстрая сортировка (O(n log n))
    std::vector<int> vec{5, 2, 8, 1, 9, 3};
    std::sort(vec.begin(), vec.end());
    // vec = {1, 2, 3, 5, 8, 9}
    
    // Сортировка по убыванию
    std::sort(vec.begin(), vec.end(), std::greater<>());
    
    // stable_sort - устойчивая сортировка
    // (сохраняет относительный порядок равных элементов)
    struct Person {
        std::string name;
        int age;
    };
    std::vector<Person> people{
        {"Alice", 30}, {"Bob", 25}, {"Charlie", 30}
    };
    std::stable_sort(people.begin(), people.end(),
        [](const Person& a, const Person& b) { 
            return a.age < b.age; 
        });
    // Alice и Charlie сохранят свой порядок
    
    // partial_sort - частичная сортировка (топ-N)
    std::vector<int> scores{85, 92, 78, 95, 88, 73, 90};
    std::partial_sort(scores.begin(), scores.begin() + 3, scores.end(),
                      std::greater<>());
    // Первые 3 элемента - наибольшие в отсортированном порядке
    // Остальные - в неопределенном порядке
    
    // nth_element - n-й элемент на своем месте
    std::vector<int> nums{5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::nth_element(nums.begin(), nums.begin() + 4, nums.end());
    // nums[4] - медиана, слева меньше, справа больше
}

void demo_custom_comparators() {
    struct Product {
        std::string name;
        double price;
        int rating;
    };
    
    std::vector<Product> products{
        {"Laptop", 999.99, 5},
        {"Mouse", 29.99, 4},
        {"Keyboard", 79.99, 5}
    };
    
    // Лямбда-компаратор
    std::sort(products.begin(), products.end(),
        [](const Product& a, const Product& b) {
            return a.price < b.price;
        });
    
    // Сортировка по нескольким критериям
    std::sort(products.begin(), products.end(),
        [](const Product& a, const Product& b) {
            if (a.rating != b.rating) return a.rating > b.rating;
            return a.price < b.price;
        });
    // Сначала по рейтингу (убыв.), затем по цене (возр.)
    
    // Использование std::tie для сравнения
    std::sort(products.begin(), products.end(),
        [](const Product& a, const Product& b) {
            return std::tie(a.rating, a.price) > 
                   std::tie(b.rating, b.price);
        });
}

void demo_partitioning() {
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // partition - разбиение на две части
    auto pivot = std::partition(vec.begin(), vec.end(),
        [](int x) { return x % 2 == 0; });
    // Четные элементы перед нечетными
    // pivot указывает на начало второй части
    
    // stable_partition - устойчивое разбиение
    std::vector<int> vec2{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::stable_partition(vec2.begin(), vec2.end(),
        [](int x) { return x % 2 == 0; });
    // Сохраняет относительный порядок
    
    // is_sorted / is_sorted_until
    std::vector<int> sorted{1, 2, 3, 4, 5};
    bool is_sorted = std::is_sorted(sorted.begin(), sorted.end());
    
    std::vector<int> partial{1, 2, 3, 9, 4, 5};
    auto until = std::is_sorted_until(partial.begin(), partial.end());
    // until указывает на 9 (первый "неотсортированный")
}

// ============================================
// 📌 BINARY SEARCH (требуют отсортированного диапазона)
// ============================================

void demo_binary_search() {
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    // ВАЖНО: массив должен быть отсортирован!
    
    // binary_search - проверка наличия элемента
    bool found = std::binary_search(vec.begin(), vec.end(), 5);
    std::cout << "5 найден: " << found << '\n';
    
    // lower_bound - первый элемент >= значения
    auto lb = std::lower_bound(vec.begin(), vec.end(), 5);
    std::cout << "lower_bound(5): " << *lb << '\n';
    
    // upper_bound - первый элемент > значения
    auto ub = std::upper_bound(vec.begin(), vec.end(), 5);
    std::cout << "upper_bound(5): " << *ub << '\n';
    
    // equal_range - диапазон элементов, равных значению
    auto [first, last] = std::equal_range(vec.begin(), vec.end(), 5);
    std::cout << "Диапазон для 5: от " << *first << " до " << *last << '\n';
    
    // Практический пример: вставка с сохранением порядка
    auto insert_pos = std::lower_bound(vec.begin(), vec.end(), 5);
    vec.insert(insert_pos, 5); // Вставляем еще одну 5
}

void demo_binary_search_custom() {
    struct Event {
        int time;
        std::string name;
    };
    
    std::vector<Event> events{
        {100, "Start"}, {200, "Middle"}, {300, "End"}
    };
    
    // Бинарный поиск с кастомным компаратором
    auto it = std::lower_bound(events.begin(), events.end(), 200,
        [](const Event& e, int t) { return e.time < t; });
    
    if (it != events.end() && it->time == 200) {
        std::cout << "Событие в 200: " << it->name << '\n';
    }
}

// ============================================
// 📌 SET OPERATIONS (требуют отсортированных диапазонов)
// ============================================

void demo_set_operations() {
    std::vector<int> a{1, 2, 3, 4, 5};
    std::vector<int> b{3, 4, 5, 6, 7};
    std::vector<int> result;
    
    // set_union - объединение
    std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                   std::back_inserter(result));
    // result = {1, 2, 3, 4, 5, 6, 7}
    
    // set_intersection - пересечение
    result.clear();
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::back_inserter(result));
    // result = {3, 4, 5}
    
    // set_difference - разность (A - B)
    result.clear();
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                        std::back_inserter(result));
    // result = {1, 2}
    
    // set_symmetric_difference - симметричная разность
    result.clear();
    std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                                  std::back_inserter(result));
    // result = {1, 2, 6, 7}
    
    // includes - проверка, что все элементы b есть в a
    std::vector<int> subset{2, 3};
    bool contains = std::includes(a.begin(), a.end(), 
                                  subset.begin(), subset.end());
    std::cout << "a содержит subset: " << contains << '\n';
    
    // merge - слияние двух отсортированных последовательностей
    result.clear();
    std::merge(a.begin(), a.end(), b.begin(), b.end(),
               std::back_inserter(result));
    // result отсортирован и может содержать дубликаты
}

// ============================================
// 📌 HEAP OPERATIONS
// ============================================

void demo_heap_operations() {
    std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
    
    // make_heap - создание max-heap
    std::make_heap(vec.begin(), vec.end());
    // vec[0] - максимальный элемент
    
    // push_heap - добавление элемента в heap
    vec.push_back(10);
    std::push_heap(vec.begin(), vec.end());
    
    // pop_heap - удаление максимального элемента
    std::pop_heap(vec.begin(), vec.end());
    int max = vec.back();
    vec.pop_back();
    std::cout << "Макс элемент: " << max << '\n';
    
    // sort_heap - сортировка heap (после этого это уже не heap!)
    std::sort_heap(vec.begin(), vec.end());
    // vec теперь отсортирован по возрастанию
    
    // is_heap - проверка, является ли диапазон heap'ом
    std::vector<int> vec2{9, 5, 4, 1, 3};
    bool is_h = std::is_heap(vec2.begin(), vec2.end());
    
    // Min-heap: используем greater<>
    std::vector<int> min_heap{3, 1, 4, 1, 5};
    std::make_heap(min_heap.begin(), min_heap.end(), std::greater<>());
    // min_heap[0] - минимальный элемент
}

// ============================================
// 📌 MIN/MAX OPERATIONS
// ============================================

void demo_minmax() {
    // min / max - минимум и максимум двух значений
    int a = 5, b = 10;
    std::cout << "min: " << std::min(a, b) << '\n';
    std::cout << "max: " << std::max(a, b) << '\n';
    
    // minmax - возвращает пару {min, max}
    auto [min_val, max_val] = std::minmax(a, b);
    
    // min/max с initializer_list
    int min_of_many = std::min({5, 2, 8, 1, 9});
    
    // min_element / max_element - поиск в диапазоне
    std::vector<int> vec{5, 2, 8, 1, 9, 3};
    auto min_it = std::min_element(vec.begin(), vec.end());
    auto max_it = std::max_element(vec.begin(), vec.end());
    std::cout << "Минимум: " << *min_it << '\n';
    std::cout << "Максимум: " << *max_it << '\n';
    
    // minmax_element - оба сразу
    auto [min_it2, max_it2] = std::minmax_element(vec.begin(), vec.end());
    
    // clamp (C++17) - ограничение значения диапазоном
    int value = 15;
    int clamped = std::clamp(value, 0, 10); // = 10
    std::cout << "Clamped: " << clamped << '\n';
    
    // Пример: нормализация координат
    struct Point { int x, y; };
    Point p{150, -50};
    Point clamped_p{
        std::clamp(p.x, 0, 100),
        std::clamp(p.y, 0, 100)
    };
}

// ============================================
// 📌 NUMERIC OPERATIONS
// ============================================

void demo_numeric_algorithms() {
    std::vector<int> vec{1, 2, 3, 4, 5};
    
    // accumulate - суммирование (или другая бинарная операция)
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "Сумма: " << sum << '\n';
    
    // accumulate с кастомной операцией (произведение)
    int product = std::accumulate(vec.begin(), vec.end(), 1,
        [](int a, int b) { return a * b; });
    std::cout << "Произведение: " << product << '\n';
    
    // reduce (C++17) - параллелизуемая версия accumulate
    int sum2 = std::reduce(vec.begin(), vec.end(), 0);
    
    // inner_product - скалярное произведение
    std::vector<int> vec2{2, 3, 4, 5, 6};
    int dot = std::inner_product(vec.begin(), vec.end(), 
                                  vec2.begin(), 0);
    // 1*2 + 2*3 + 3*4 + 4*5 + 5*6 = 70
    
    // transform_reduce (C++17) - transform + reduce
    int sum_of_squares = std::transform_reduce(
        vec.begin(), vec.end(), 0, std::plus<>(),
        [](int x) { return x * x; }
    );
    // 1² + 2² + 3² + 4² + 5² = 55
    
    // partial_sum - частичные суммы
    std::vector<int> partial;
    std::partial_sum(vec.begin(), vec.end(), 
                     std::back_inserter(partial));
    // partial = {1, 3, 6, 10, 15}
    
    // inclusive_scan (C++17) - как partial_sum
    std::vector<int> scan;
    std::inclusive_scan(vec.begin(), vec.end(),
                        std::back_inserter(scan));
    
    // exclusive_scan (C++17) - без включения текущего элемента
    std::vector<int> exc_scan;
    std::exclusive_scan(vec.begin(), vec.end(),
                        std::back_inserter(exc_scan), 0);
    // exc_scan = {0, 1, 3, 6, 10}
    
    // adjacent_difference - разности соседних элементов
    std::vector<int> diffs;
    std::adjacent_difference(vec.begin(), vec.end(),
                             std::back_inserter(diffs));
    // diffs = {1, 1, 1, 1, 1}
    
    // iota - заполнение последовательными значениями
    std::vector<int> seq(10);
    std::iota(seq.begin(), seq.end(), 1);
    // seq = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    
    // gcd / lcm (C++17) - НОД и НОК
    int g = std::gcd(24, 36);  // 12
    int l = std::lcm(24, 36);  // 72
    
    // midpoint (C++20) - среднее без переполнения
    int mid = std::midpoint(100, 200);  // 150
    
    // lerp (C++20) - линейная интерполяция
    double interpolated = std::lerp(0.0, 10.0, 0.5);  // 5.0
}

// ============================================
// 📌 EXECUTION POLICIES (C++17)
// ============================================

void demo_parallel_algorithms() {
    std::vector<int> large_vec(1'000'000);
    std::iota(large_vec.begin(), large_vec.end(), 1);
    
    // std::execution::seq - последовательное выполнение (по умолчанию)
    std::sort(std::execution::seq, 
              large_vec.begin(), large_vec.end());
    
    // std::execution::par - параллельное выполнение
    // Может использовать несколько потоков
    std::sort(std::execution::par,
              large_vec.begin(), large_vec.end());
    
    // std::execution::par_unseq - параллельное + векторизация
    // Может использовать SIMD инструкции
    std::sort(std::execution::par_unseq,
              large_vec.begin(), large_vec.end());
    
    // std::execution::unseq (C++20) - только векторизация
    std::sort(std::execution::unseq,
              large_vec.begin(), large_vec.end());
    
    // Примеры других алгоритмов с execution policies:
    
    // Параллельный transform
    std::vector<double> data(1'000'000);
    std::transform(std::execution::par,
                   data.begin(), data.end(), data.begin(),
                   [](double x) { return std::sqrt(x); });
    
    // Параллельный count_if
    auto count = std::count_if(std::execution::par,
                               large_vec.begin(), large_vec.end(),
                               [](int x) { return x % 2 == 0; });
    
    // ВАЖНО: Параллельные алгоритмы требуют thread-safe операций!
    // Плохо - race condition:
    // int sum = 0;
    // std::for_each(std::execution::par, vec.begin(), vec.end(),
    //     [&sum](int x) { sum += x; }); // ОПАСНО!
    
    // Хорошо - используйте reduce:
    int safe_sum = std::reduce(std::execution::par,
                               large_vec.begin(), large_vec.end());
}

void performance_considerations() {
    /*
     * КОГДА ИСПОЛЬЗОВАТЬ ПАРАЛЛЕЛЬНЫЕ АЛГОРИТМЫ:
     * 
     * ✅ Хорошо подходят:
     * - Большие объемы данных (> 10,000 элементов)
     * - Вычислительно затратные операции на элемент
     * - sort, transform, reduce, for_each
     * 
     * ❌ Плохо подходят:
     * - Маленькие массивы (overhead от создания потоков)
     * - Операции с зависимостями между элементами
     * - Работа с shared state без синхронизации
     * 
     * OVERHEAD:
     * - Создание потоков занимает время
     * - Нужно учитывать стоимость операции на элемент
     * - Тестируйте на реальных данных!
     */
    
    // Пример: когда параллелизм невыгоден
    std::vector<int> small_vec{1, 2, 3, 4, 5};
    // Это МЕДЛЕННЕЕ чем seq:
    // std::sort(std::execution::par, small_vec.begin(), small_vec.end());
    
    // Пример: когда параллелизм выгоден
    std::vector<double> big_data(10'000'000);
    std::iota(big_data.begin(), big_data.end(), 1.0);
    // Это БЫСТРЕЕ чем seq:
    std::transform(std::execution::par,
                   big_data.begin(), big_data.end(), big_data.begin(),
                   [](double x) { return std::sin(x) * std::cos(x); });
}

// ============================================
// 📌 RANGES ALGORITHMS (C++20)
// ============================================

void demo_ranges_basics() {
    namespace rng = std::ranges;
    
    std::vector<int> vec{5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // Ranges алгоритмы принимают сам контейнер, а не итераторы
    rng::sort(vec);  // Вместо std::sort(vec.begin(), vec.end())
    
    // Поиск
    auto it = rng::find(vec, 5);
    if (it != vec.end()) {
        std::cout << "Найден: " << *it << '\n';
    }
    
    // count_if
    int even_count = rng::count_if(vec, [](int x) { return x % 2 == 0; });
    
    // copy_if
    std::vector<int> evens;
    rng::copy_if(vec, std::back_inserter(evens),
                 [](int x) { return x % 2 == 0; });
    
    // transform
    std::vector<int> squared;
    rng::transform(vec, std::back_inserter(squared),
                   [](int x) { return x * x; });
}

void demo_projections() {
    // Проекции позволяют трансформировать элементы перед операцией
    struct Person {
        std::string name;
        int age;
    };
    
    std::vector<Person> people{
        {"Alice", 30}, {"Bob", 25}, {"Charlie", 35}
    };
    
    // Сортировка по возрасту с помощью проекции
    std::ranges::sort(people, {}, &Person::age);
    // Вместо: std::sort(people.begin(), people.end(),
    //              [](const Person& a, const Person& b) {
    //                  return a.age < b.age;
    //              });
    
    // Поиск по имени
    auto it = std::ranges::find(people, "Bob", &Person::name);
    
    // Проекция с лямбдой
    std::ranges::sort(people, {},
        [](const Person& p) { return p.age; });
    
    // max_element с проекцией
    auto oldest = std::ranges::max_element(people, {}, &Person::age);
    std::cout << "Самый старший: " << oldest->name << '\n';
}

void demo_constrained_algorithms() {
    // Ranges алгоритмы используют концепты для проверки типов
    std::vector<int> vec{1, 2, 3, 4, 5};
    
    // Это компилируется - vector имеет random_access_iterator
    std::ranges::sort(vec);
    
    std::list<int> lst{1, 2, 3, 4, 5};
    // Это НЕ компилируется - list имеет bidirectional_iterator
    // std::ranges::sort(lst);  // Ошибка компиляции!
    
    // Но это работает:
    std::ranges::reverse(lst);  // reverse требует только bidirectional
    
    // Ranges проверяют концепты во время компиляции,
    // давая более понятные сообщения об ошибках
}

void demo_ranges_views_with_algorithms() {
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Комбинирование views и алгоритмов
    namespace rv = std::ranges::views;
    
    // Подсчет четных чисел > 5
    auto filtered = vec | rv::filter([](int x) { return x % 2 == 0; })
                        | rv::filter([](int x) { return x > 5; });
    int count = std::ranges::distance(filtered);
    
    // Сумма квадратов нечетных чисел
    auto odd_squares = vec 
        | rv::filter([](int x) { return x % 2 == 1; })
        | rv::transform([](int x) { return x * x; });
    int sum = std::accumulate(odd_squares.begin(), odd_squares.end(), 0);
    
    // Ranges возвращают подтипы range, а не новые контейнеры (ленивость)
}

// ============================================
// 📌 COMMON PATTERNS
// ============================================

void pattern_erase_remove() {
    // ERASE-REMOVE IDIOM - стандартный способ удаления элементов
    
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // Удаление всех четных чисел
    vec.erase(
        std::remove_if(vec.begin(), vec.end(),
            [](int x) { return x % 2 == 0; }),
        vec.end()
    );
    
    // C++20 ranges упрощает это:
    namespace rng = std::ranges;
    std::vector<int> vec2{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto [first, last] = rng::remove_if(vec2, 
        [](int x) { return x % 2 == 0; });
    vec2.erase(first, last);
    
    // Или используйте std::erase_if (C++20) для std::vector:
    std::vector<int> vec3{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::erase_if(vec3, [](int x) { return x % 2 == 0; });
    // Самый простой способ!
}

void pattern_transform_reduce() {
    // TRANSFORM-REDUCE - преобразование и сокращение в одной операции
    
    std::vector<int> prices{100, 200, 150, 300};
    std::vector<int> quantities{2, 1, 3, 2};
    
    // Общая стоимость: sum(price[i] * quantity[i])
    int total = std::transform_reduce(
        prices.begin(), prices.end(),
        quantities.begin(),
        0,  // начальное значение
        std::plus<>(),  // операция сокращения
        std::multiplies<>()  // операция трансформации
    );
    std::cout << "Общая стоимость: " << total << '\n';
    
    // Сумма квадратов
    std::vector<double> values{1.5, 2.5, 3.5};
    double sum_of_squares = std::transform_reduce(
        values.begin(), values.end(),
        0.0,
        std::plus<>(),
        [](double x) { return x * x; }
    );
}

void pattern_sorting_with_lambdas() {
    // СОРТИРОВКА С ЛЯМБДАМИ - гибкая настройка порядка
    
    struct Task {
        std::string name;
        int priority;
        std::chrono::system_clock::time_point deadline;
    };
    
    std::vector<Task> tasks;
    // ... заполнение tasks
    
    // Сортировка по приоритету (убыв.), затем по deadline (возр.)
    std::ranges::sort(tasks, [](const Task& a, const Task& b) {
        if (a.priority != b.priority) {
            return a.priority > b.priority;  // Больший приоритет первым
        }
        return a.deadline < b.deadline;  // Раньше deadline первым
    });
    
    // С помощью std::tie
    std::ranges::sort(tasks, [](const Task& a, const Task& b) {
        return std::tie(b.priority, a.deadline) < 
               std::tie(a.priority, b.deadline);
    });
    
    // С проекцией (только по одному полю)
    std::ranges::sort(tasks, {}, &Task::priority);
}

void pattern_filtering_transformation() {
    // FILTERING + TRANSFORMATION CHAINS
    
    std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Старый способ: много промежуточных контейнеров
    std::vector<int> evens;
    std::copy_if(numbers.begin(), numbers.end(),
                 std::back_inserter(evens),
                 [](int x) { return x % 2 == 0; });
    
    std::vector<int> doubled;
    std::transform(evens.begin(), evens.end(),
                   std::back_inserter(doubled),
                   [](int x) { return x * 2; });
    
    // C++20 ranges: ленивые вычисления, нет промежуточных контейнеров
    namespace rv = std::ranges::views;
    auto result = numbers
        | rv::filter([](int x) { return x % 2 == 0; })
        | rv::transform([](int x) { return x * 2; });
    
    // Материализация в vector при необходимости
    std::vector<int> materialized(result.begin(), result.end());
    
    // Или просто итерация
    for (int val : result) {
        std::cout << val << ' ';
    }
}

void pattern_custom_predicates() {
    // CUSTOM PREDICATES - переиспользуемые условия
    
    // Функциональный объект
    struct IsEven {
        bool operator()(int x) const { return x % 2 == 0; }
    };
    
    std::vector<int> vec{1, 2, 3, 4, 5, 6};
    
    // Использование function object
    auto count1 = std::count_if(vec.begin(), vec.end(), IsEven{});
    
    // Лямбда
    auto is_even = [](int x) { return x % 2 == 0; };
    auto count2 = std::count_if(vec.begin(), vec.end(), is_even);
    
    // std::function (больше overhead)
    std::function<bool(int)> is_even_fn = [](int x) { return x % 2 == 0; };
    auto count3 = std::count_if(vec.begin(), vec.end(), is_even_fn);
    
    // Параметризованный предикат
    auto is_divisible_by = [](int divisor) {
        return [divisor](int x) { return x % divisor == 0; };
    };
    
    auto div_by_3 = is_divisible_by(3);
    auto count4 = std::count_if(vec.begin(), vec.end(), div_by_3);
}

// ============================================
// 📌 PERFORMANCE TIPS
// ============================================

void performance_tips() {
    /*
     * ⚡ СЛОЖНОСТЬ АЛГОРИТМОВ:
     * 
     * O(1):
     * - min, max, swap
     * 
     * O(log n):
     * - binary_search, lower_bound, upper_bound (на sorted range)
     * - heap operations (push/pop)
     * 
     * O(n):
     * - find, count, copy, transform, accumulate
     * - is_sorted, remove, unique, reverse, rotate
     * 
     * O(n log n):
     * - sort, stable_sort, merge
     * - set operations на sorted ranges
     * 
     * O(n²):
     * - is_permutation (worst case)
     * - search без оптимизаций
     */
    
    /*
     * 🔍 КАТЕГОРИИ ИТЕРАТОРОВ (от слабых к сильным):
     * 
     * Input Iterator: только чтение, single-pass
     * Output Iterator: только запись, single-pass
     * Forward Iterator: чтение/запись, multi-pass
     * Bidirectional Iterator: + движение назад (list, set, map)
     * Random Access Iterator: + произвольный доступ (vector, deque, array)
     * Contiguous Iterator (C++20): + смежность в памяти (vector, array)
     * 
     * Некоторые алгоритмы требуют определенных категорий:
     * - sort требует Random Access
     * - reverse требует Bidirectional
     * - find требует только Input
     */
    
    /*
     * 📊 КОГДА ИСПОЛЬЗОВАТЬ RANGES:
     * 
     * ✅ Используйте ranges когда:
     * - Хотите более читаемый код
     * - Нужны проекции
     * - Работаете с views (ленивые вычисления)
     * - Хотите лучшие сообщения об ошибках компиляции
     * 
     * ❌ Избегайте ranges когда:
     * - Нужна максимальная производительность (есть небольшой overhead)
     * - Работаете с устаревшим кодом
     * - Используете компилятор без полной поддержки C++20
     */
    
    /*
     * ⚠️ GOTCHAS С ПАРАЛЛЕЛЬНЫМИ АЛГОРИТМАМИ:
     * 
     * 1. Race conditions:
     *    std::for_each(std::execution::par, v.begin(), v.end(),
     *        [&counter](int x) { ++counter; }); // ОПАСНО!
     * 
     * 2. Исключения:
     *    Если алгоритм бросает исключение в параллельном режиме,
     *    std::terminate() вызывается немедленно!
     * 
     * 3. Детерминированность:
     *    Результаты могут отличаться между запусками из-за
     *    floating-point операций в разном порядке
     * 
     * 4. Overhead:
     *    Не используйте par для маленьких данных или простых операций
     * 
     * 5. Iterator invalidation:
     *    Некоторые алгоритмы (remove, unique) могут инвалидировать
     *    итераторы непредсказуемым образом в параллельном режиме
     */
}

void performance_best_practices() {
    std::vector<int> data(1'000'000);
    std::iota(data.begin(), data.end(), 1);
    
    // ✅ ХОРОШО: reserve перед вставкой
    std::vector<int> result;
    result.reserve(data.size());
    std::copy(data.begin(), data.end(), std::back_inserter(result));
    
    // ❌ ПЛОХО: без reserve - множество реаллокаций
    std::vector<int> result2;
    std::copy(data.begin(), data.end(), std::back_inserter(result2));
    
    // ✅ ХОРОШО: erase-remove для удаления
    data.erase(
        std::remove_if(data.begin(), data.end(),
            [](int x) { return x % 2 == 0; }),
        data.end()
    );
    
    // ❌ ПЛОХО: удаление в цикле
    // for (auto it = data.begin(); it != data.end(); ) {
    //     if (*it % 2 == 0) {
    //         it = data.erase(it);  // O(n) на каждой итерации!
    //     } else {
    //         ++it;
    //     }
    // }
    
    // ✅ ХОРОШО: используйте binary_search на отсортированных данных
    std::vector<int> sorted{1, 2, 3, 4, 5, 6, 7, 8, 9};
    bool found = std::binary_search(sorted.begin(), sorted.end(), 5);
    
    // ❌ ПЛОХО: линейный поиск на отсортированных данных
    // auto it = std::find(sorted.begin(), sorted.end(), 5);
    
    // ✅ ХОРОШО: используйте stable_sort только когда нужна стабильность
    std::sort(data.begin(), data.end());  // Быстрее
    
    // ❌ ПЛОХО: stable_sort когда стабильность не нужна
    // std::stable_sort(data.begin(), data.end());  // Медленнее
}

// ============================================
// 📌 ГЛАВНАЯ ФУНКЦИЯ - ДЕМОНСТРАЦИЯ ВСЕХ ПРИМЕРОВ
// ============================================

int main() {
    std::cout << "=== Non-Modifying Algorithms ===\n";
    demo_find_algorithms();
    demo_count_algorithms();
    demo_predicates();
    demo_comparison();
    
    std::cout << "\n=== Modifying Algorithms ===\n";
    demo_copy_move();
    demo_transform();
    demo_replace();
    demo_fill_generate();
    demo_remove_unique();
    demo_reverse_rotate();
    demo_shuffle_sample();
    
    std::cout << "\n=== Sorting ===\n";
    demo_sorting();
    demo_custom_comparators();
    demo_partitioning();
    
    std::cout << "\n=== Binary Search ===\n";
    demo_binary_search();
    demo_binary_search_custom();
    
    std::cout << "\n=== Set Operations ===\n";
    demo_set_operations();
    
    std::cout << "\n=== Heap Operations ===\n";
    demo_heap_operations();
    
    std::cout << "\n=== Min/Max ===\n";
    demo_minmax();
    
    std::cout << "\n=== Numeric Algorithms ===\n";
    demo_numeric_algorithms();
    
    std::cout << "\n=== Parallel Execution ===\n";
    demo_parallel_algorithms();
    performance_considerations();
    
    std::cout << "\n=== Ranges (C++20) ===\n";
    demo_ranges_basics();
    demo_projections();
    demo_constrained_algorithms();
    demo_ranges_views_with_algorithms();
    
    std::cout << "\n=== Common Patterns ===\n";
    pattern_erase_remove();
    pattern_transform_reduce();
    pattern_sorting_with_lambdas();
    pattern_filtering_transformation();
    pattern_custom_predicates();
    
    std::cout << "\n=== Performance Tips ===\n";
    performance_tips();
    performance_best_practices();
    
    return 0;
}