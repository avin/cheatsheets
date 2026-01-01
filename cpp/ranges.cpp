#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>

namespace views = std::views;
namespace ranges = std::ranges;

// ---------------------------------------------------
// 📌 Базовые views: filter, transform, take, drop
// ---------------------------------------------------
void example_basic_views() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Filter - фильтрация чётных чисел
    auto evens = numbers | views::filter([](int n) { return n % 2 == 0; });
    
    // Transform - умножение на 2
    auto doubled = numbers | views::transform([](int n) { return n * 2; });
    
    // Take - взять первые 5 элементов
    auto first_five = numbers | views::take(5);
    
    // Drop - пропустить первые 3 элемента
    auto after_three = numbers | views::drop(3);
    
    // Композиция views
    auto result = numbers 
        | views::filter([](int n) { return n % 2 == 0; })
        | views::transform([](int n) { return n * n; })
        | views::take(3);
}

// ---------------------------------------------------
// 📌 Zip views (C++23) - объединение нескольких диапазонов
// ---------------------------------------------------
void example_zip_views() {
    std::vector<int> ids = {1, 2, 3, 4};
    std::vector<std::string> names = {"Alice", "Bob", "Charlie", "Diana"};
    std::vector<int> scores = {95, 87, 92, 88};
    
    // Zip - создаёт кортежи из элементов нескольких диапазонов
    for (auto [id, name, score] : views::zip(ids, names, scores)) {
        std::println("ID: {}, Name: {}, Score: {}", id, name, score);
    }
    
    // Zip_transform - применяет функцию к элементам нескольких диапазонов
    auto combined = views::zip_transform(
        [](int id, const std::string& name, int score) {
            return std::format("{}: {} ({})", id, name, score);
        },
        ids, names, scores
    );
}

// ---------------------------------------------------
// 📌 Chunk и slide views (C++23) - разбиение на подгруппы
// ---------------------------------------------------
void example_chunk_slide() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Chunk - разбивает на группы по N элементов
    for (auto chunk : numbers | views::chunk(3)) {
        std::print("Chunk: ");
        for (int n : chunk) {
            std::print("{} ", n);
        }
        std::println("");
    }
    
    // Slide - создаёт скользящее окно размера N
    for (auto window : numbers | views::slide(3)) {
        std::print("Window: ");
        for (int n : window) {
            std::print("{} ", n);
        }
        std::println("");
    }
}

// ---------------------------------------------------
// 📌 Cartesian product (C++23) - декартово произведение
// ---------------------------------------------------
void example_cartesian_product() {
    std::vector<int> sizes = {1, 2, 3};
    std::vector<std::string> colors = {"red", "green", "blue"};
    
    // Создаёт все комбинации элементов из нескольких диапазонов
    for (auto [size, color] : views::cartesian_product(sizes, colors)) {
        std::println("Size: {}, Color: {}", size, color);
    }
}

// ---------------------------------------------------
// 📌 Enumerate (C++23) - итерация с индексом
// ---------------------------------------------------
void example_enumerate() {
    std::vector<std::string> fruits = {"apple", "banana", "cherry", "date"};
    
    // Enumerate предоставляет индекс и значение
    for (auto [index, fruit] : fruits | views::enumerate) {
        std::println("{}: {}", index, fruit);
    }
}

// ---------------------------------------------------
// 📌 Join_with (C++23) - объединение с разделителем
// ---------------------------------------------------
void example_join_with() {
    std::vector<std::vector<int>> groups = {{1, 2}, {3, 4}, {5, 6}};
    
    // Join_with объединяет диапазоны с указанным разделителем
    auto joined = groups | views::join_with(0);
    
    for (int n : joined) {
        std::print("{} ", n);  // Выведет: 1 2 0 3 4 0 5 6
    }
    std::println("");
}

// ---------------------------------------------------
// 📌 Fold операции (C++23) - fold_left, fold_right
// ---------------------------------------------------
void example_fold() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // fold_left - левая свёртка (аккумулятор слева)
    auto sum = ranges::fold_left(numbers, 0, std::plus<>{});
    std::println("Sum: {}", sum);
    
    // fold_left с лямбдой
    auto product = ranges::fold_left(numbers, 1, 
        [](int acc, int n) { return acc * n; });
    std::println("Product: {}", product);
    
    // fold_right - правая свёртка
    auto folded_right = ranges::fold_right(numbers, 0, 
        [](int n, int acc) { return n - acc; });
}

// ---------------------------------------------------
// 📌 Дополнительные полезные views
// ---------------------------------------------------
void example_additional_views() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // Reverse - разворот диапазона
    auto reversed = numbers | views::reverse;
    
    // Elements - выбор элемента из tuple/pair
    std::vector<std::pair<int, std::string>> pairs = {{1, "one"}, {2, "two"}};
    auto keys = pairs | views::elements<0>;    // только первые элементы
    auto values = pairs | views::elements<1>;  // только вторые элементы
    
    // Keys и values для map
    std::map<int, std::string> map = {{1, "one"}, {2, "two"}};
    auto map_keys = map | views::keys;
    auto map_values = map | views::values;
    
    // Take_while и drop_while
    auto taken = numbers | views::take_while([](int n) { return n < 4; });
    auto dropped = numbers | views::drop_while([](int n) { return n < 3; });
    
    // Join - объединение вложенных диапазонов
    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4}, {5}};
    auto flattened = nested | views::join;
    
    // Split - разделение по значению
    std::string text = "one,two,three";
    auto parts = text | views::split(',');
}

// ---------------------------------------------------
// 📌 Ленивое выполнение и композиция
// ---------------------------------------------------
void example_lazy_evaluation() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Views выполняются лениво - вычисления происходят только при итерации
    auto pipeline = numbers
        | views::filter([](int n) { 
            std::println("Filtering {}", n); 
            return n % 2 == 0; 
        })
        | views::transform([](int n) { 
            std::println("Transforming {}", n); 
            return n * n; 
        })
        | views::take(2);
    
    std::println("Pipeline created, not yet executed");
    
    // Вычисления начинаются здесь
    for (int n : pipeline) {
        std::println("Result: {}", n);
    }
}

// ---------------------------------------------------
// 📌 Материализация views в контейнеры
// ---------------------------------------------------
void example_materialize() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    auto evens = numbers | views::filter([](int n) { return n % 2 == 0; });
    
    // Преобразование view в vector
    std::vector<int> evens_vec(evens.begin(), evens.end());
    
    // Или с использованием ranges::to (C++23)
    auto evens_vec2 = numbers 
        | views::filter([](int n) { return n % 2 == 0; })
        | ranges::to<std::vector>();
}

// ---------------------------------------------------
// 📌 Комплексные примеры композиции
// ---------------------------------------------------
void example_complex_pipeline() {
    std::vector<std::string> words = {
        "apple", "apricot", "banana", "berry", "cherry", 
        "date", "dragonfruit", "elderberry"
    };
    
    // Найти все слова, начинающиеся с 'a' или 'b', 
    // преобразовать в верхний регистр, взять первые 3
    auto result = words
        | views::filter([](const std::string& w) { 
            return w[0] == 'a' || w[0] == 'b'; 
        })
        | views::transform([](const std::string& w) {
            std::string upper = w;
            ranges::transform(upper, upper.begin(), ::toupper);
            return upper;
        })
        | views::take(3);
    
    for (const auto& word : result) {
        std::println("{}", word);
    }
}

// ---------------------------------------------------
// 📌 Работа с числовыми последовательностями
// ---------------------------------------------------
void example_iota_view() {
    // iota_view - генерирует последовательность чисел
    auto numbers = views::iota(1, 11);  // от 1 до 10
    
    // Бесконечная последовательность (используйте с take!)
    auto infinite = views::iota(0) | views::take(10);
    
    // Комбинация с другими views
    auto squares = views::iota(1, 11)
        | views::transform([](int n) { return n * n; });
}

// ---------------------------------------------------
// 📌 Фильтрация и обработка пар ключ-значение
// ---------------------------------------------------
void example_map_processing() {
    std::map<std::string, int> scores = {
        {"Alice", 95}, {"Bob", 72}, {"Charlie", 88}, 
        {"Diana", 91}, {"Eve", 65}
    };
    
    // Найти всех с баллом >= 80 и вывести имена
    auto high_scorers = scores
        | views::filter([](const auto& pair) { return pair.second >= 80; })
        | views::keys;
    
    for (const auto& name : high_scorers) {
        std::println("High scorer: {}", name);
    }
}

// ---------------------------------------------------
// 📌 Stride (через chunk + transform) - взять каждый N-й элемент
// ---------------------------------------------------
void example_stride() {
    std::vector<int> numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // Взять каждый второй элемент
    auto every_second = numbers 
        | views::chunk(2)
        | views::transform([](auto chunk) { return *chunk.begin(); });
    
    for (int n : every_second) {
        std::print("{} ", n);  // 0 2 4 6 8
    }
    std::println("");
}
