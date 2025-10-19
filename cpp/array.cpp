#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <iostream>
#include <unordered_set>

// ---------------------------------------------------
// 📌 Создание вектора
// ---------------------------------------------------
void example_vector() {
    // Пустой вектор
    std::vector<int> v1;

    // Инициализация списком (initializer_list)
    std::vector<int> v2 = {1, 2, 3, 4, 5};

    // Вектор заданного размера, все элементы инициализируются значением по умолчанию (0)
    std::vector<int> v3(5); // {0, 0, 0, 0, 0}

    // Вектор заданного размера с заданным значением для всех элементов
    std::vector<int> v4(5, 42); // {42, 42, 42, 42, 42}

    // Копирование из другого вектора
    std::vector<int> v5(v2); // копия v2

    // Перемещение (move) из другого вектора
    std::vector<int> temp = {7, 8, 9};
    std::vector<int> v6(std::move(temp)); // temp теперь пустой

    // Конструктор из диапазона (из массива C)
    int arr[] = {10, 11, 12, 13};
    std::vector<int> v7(std::begin(arr), std::end(arr)); // {10,11,12,13}

    // Конструктор из диапазона (из другого контейнера: например, из списка std::list)
    std::list<int> lst = {20, 21, 22};
    std::vector<int> v8(std::begin(lst), std::end(lst)); // {20,21,22}

    // Использование assign для перезаписи значений вектора
    std::vector<int> v9;
    v9.assign(3, 99);       // {99, 99, 99}
    v9.assign(v2.begin(), v2.begin() + 3); // копирует первые 3 элемента из v2: {1,2,3}

    // Резервирование памяти без изменения размера
    std::vector<int> v10;
    v10.reserve(100);      // зарезервировать память под 100 элементов
    // после reserve размер остаётся 0, но ёмкость >= 100

    // Изменение размера с инициализацией новых элементов
    std::vector<int> v11 = {1, 2, 3};
    v11.resize(5);         // теперь {1,2,3,0,0}
    v11.resize(2);         // теперь {1,2}

    // Инициализация с помощью итераторов алгоритма std::iota
    std::vector<int> v12(5);
    std::iota(v12.begin(), v12.end(), 1); // {1,2,3,4,5}
}
  
// ---------------------------------------------------
// 📌 Итерация по вектору
// ---------------------------------------------------
void example_iteration(const std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size(); ++i)
        std::cout << arr[i] << std::endl;

    for (const auto& x : arr)
        std::cout << x << std::endl;

    std::for_each(arr.begin(), arr.end(), [](int x) {
        std::cout << x << std::endl;
    });
}

// ---------------------------------------------------
// 📌 Фильтрация
// ---------------------------------------------------
std::vector<int> example_filter(const std::vector<int>& arr) {
    std::vector<int> filtered;
    std::copy_if(arr.begin(), arr.end(), std::back_inserter(filtered),
                 [](int x) { return x > 0; });
    return filtered;
}

// ---------------------------------------------------
// 📌 Поиск и условия
// ---------------------------------------------------
void example_find(const std::vector<int>& arr) {
    auto it = std::find(arr.begin(), arr.end(), 1);
    int index = (it != arr.end()) ? static_cast<int>(std::distance(arr.begin(), it)) : -1;
    bool hasValue = (it != arr.end());
    bool any = std::any_of(arr.begin(), arr.end(), [](int x) { return x > 10; });
    bool all = std::all_of(arr.begin(), arr.end(), [](int x) { return x > 0; });
}

// ---------------------------------------------------
// 📌 Map-преобразование
// ---------------------------------------------------
std::vector<int> example_map(const std::vector<int>& arr) {
    std::vector<int> mapped;
    mapped.reserve(arr.size());
    std::transform(arr.begin(), arr.end(), std::back_inserter(mapped),
                   [](int x) { return x * 2; });
    return mapped;
}

// ---------------------------------------------------
// 📌 Reduce / fold
// ---------------------------------------------------
int example_reduce(const std::vector<int>& arr) {
    return std::accumulate(arr.begin(), arr.end(), 0,
                           [](int acc, int x) { return acc + x; });
}

// ---------------------------------------------------
// 📌 Сортировка (не мутируя оригинал)
// ---------------------------------------------------
std::vector<int> example_sort(const std::vector<int>& arr) {
    std::vector<int> sorted = arr;
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}

// ---------------------------------------------------
// 📌 Минимум / максимум
// ---------------------------------------------------
void example_min_max(const std::vector<int>& arr) {
    if (!arr.empty()) {
        int minVal = *std::min_element(arr.begin(), arr.end());
        int maxVal = *std::max_element(arr.begin(), arr.end());
    }
}

// ---------------------------------------------------
// 📌 Количество элементов
// ---------------------------------------------------
size_t example_count(const std::vector<int>& arr) {
    return arr.size();
}

// ---------------------------------------------------
// 📌 Срезы (subvector)
// ---------------------------------------------------
std::vector<int> example_slice(const std::vector<int>& arr) {
    std::vector<int> firstTwo(arr.begin(), arr.begin() + std::min(arr.size(), size_t(2)));
    std::vector<int> rest(arr.begin() + std::min(arr.size(), size_t(2)), arr.end());
    return rest;
}

// ---------------------------------------------------
// 📌 Объединение векторов
// ---------------------------------------------------
std::vector<int> example_combine(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    std::vector<int> combined = arr1;
    combined.insert(combined.end(), arr2.begin(), arr2.end());
    return combined;
}

// ---------------------------------------------------
// 📌 Разворот вектора (без мутации исходного)
// ---------------------------------------------------
std::vector<int> example_reverse(const std::vector<int>& arr) {
    std::vector<int> reversed = arr;
    std::reverse(reversed.begin(), reversed.end());
    return reversed;
}

// ---------------------------------------------------
// 📌 Удаление элемента по индексу (иммутабельно)
// ---------------------------------------------------
std::vector<int> example_remove_by_index(const std::vector<int>& arr, size_t indexToRemove) {
    std::vector<int> result;
    result.reserve(arr.size());
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i != indexToRemove) result.push_back(arr[i]);
    }
    return result;
}

// ---------------------------------------------------
// 📌 Добавление элементов
// ---------------------------------------------------
void example_add_elements(std::vector<int>& arr) {
    arr.insert(arr.begin(), 0); // в начало
    arr.push_back(99);          // в конец
}

// ---------------------------------------------------
// 📌 Проверка на пустоту
// ---------------------------------------------------
bool example_is_empty(const std::vector<int>& arr) {
    return arr.empty();
}

// ---------------------------------------------------
// 📌 Дедупликация
// ---------------------------------------------------
std::vector<int> example_dedup(const std::vector<int>& arr) {
    std::unordered_set<int> s(arr.begin(), arr.end());
    std::vector<int> unique(s.begin(), s.end());
    return unique;
}

// ---------------------------------------------------
// 📌 Разделение по условию (std::partition)
// ---------------------------------------------------
std::vector<int> example_partition(const std::vector<int>& arr) {
    std::vector<int> part = arr;
    std::partition(part.begin(), part.end(), [](int x) { return x % 2 == 0; });
    return part;
}

// ---------------------------------------------------
// 📌 Генерация последовательных значений (std::iota)
// ---------------------------------------------------
void example_generate(std::vector<int>& arr) {
    std::iota(arr.begin(), arr.end(), 1); // заполняет 1,2,3,...
}

// ---------------------------------------------------
// 📌 Удаление элементов по условию (std::remove_if)
// ---------------------------------------------------
std::vector<int> example_remove_if(const std::vector<int>& arr) {
    std::vector<int> result = arr;
    result.erase(std::remove_if(result.begin(), result.end(), [](int x) { return x < 0; }), result.end());
    return result;
}

// ---------------------------------------------------
// 📌 Произвольное накопление (std::accumulate с лямбдами)
// ---------------------------------------------------
int example_accumulate_custom(const std::vector<int>& arr) {
    return std::accumulate(arr.begin(), arr.end(), 1, [](int acc, int x) { return acc * x; }); // произведение
}

// ---------------------------------------------------
// 📌 Заполнение массива одним значением (std::fill)
// ---------------------------------------------------
void example_fill(std::vector<int>& arr, int value) {
    std::fill(arr.begin(), arr.end(), value);
}

// ---------------------------------------------------
// 📌 Подсчёт вхождений элемента (std::count)
// ---------------------------------------------------
size_t example_count_element(const std::vector<int>& arr, int value) {
    return std::count(arr.begin(), arr.end(), value);
}

// ---------------------------------------------------
// 📌 Сумма элементов с условием (std::accumulate + std::count_if)
// ---------------------------------------------------
int example_conditional_sum(const std::vector<int>& arr) {
    // Сумма чётных элементов
    return std::accumulate(arr.begin(), arr.end(), 0, [](int acc, int x) {
        return acc + (x % 2 == 0 ? x : 0);
    });
}

// ---------------------------------------------------
// 📌 Zip двух массивов (std::transform с двумя диапазонами)
// ---------------------------------------------------
std::vector<int> example_zip(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    std::vector<int> result;
    result.reserve(std::min(arr1.size(), arr2.size()));
    std::transform(arr1.begin(), arr1.begin() + std::min(arr1.size(), arr2.size()),
                   arr2.begin(), std::back_inserter(result),
                   [](int a, int b) { return a + b; });
    return result;
}