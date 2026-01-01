#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <flat_map>        // C++23
#include <flat_set>        // C++23
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <print>           // C++23

// ---------------------------------------------------
// 📌 Создание std::set и std::unordered_set
// ---------------------------------------------------
void example_sets() {
    std::set<int> ordered = {1, 2, 3};              // сортированный
    std::unordered_set<int> unordered = {1, 2, 3};  // несортированный (хеш)
}

// ---------------------------------------------------
// 📌 Создание std::map и std::unordered_map
// ---------------------------------------------------
void example_maps() {
    std::map<std::string, int> ordered_map;
    std::unordered_map<std::string, int> unordered_map;
}

// ---------------------------------------------------
// 📌 Плоские контейнеры (C++23) - flat_set и flat_map
// ---------------------------------------------------
void example_flat_containers() {
    // flat_set - множество на основе отсортированного вектора
    // Преимущества: лучшая locality, меньше памяти, быстрее итерация
    std::flat_set<int> fset = {3, 1, 4, 1, 5, 9};
    
    // flat_map - словарь на основе двух отсортированных векторов
    std::flat_map<std::string, int> fmap;
    fmap["apple"] = 1;
    fmap["banana"] = 2;
    fmap["cherry"] = 3;
    
    // Операции аналогичны обычным set/map
    fset.insert(2);
    fset.erase(3);
    bool has = fset.contains(4);  // C++20/23
    
    // Доступ к базовому контейнеру (уникально для flat_*)
    auto keys = fmap.keys();    // ссылка на вектор ключей
    auto values = fmap.values(); // ссылка на вектор значений
}

// ---------------------------------------------------
// 📌 Добавление / удаление
// ---------------------------------------------------
void example_insert_erase() {
    std::set<int> s = {1, 2, 3};
    s.insert(4);
    s.erase(2);

    std::map<std::string, int> m;
    m["a"] = 1;
    m.erase("a");
}

// ---------------------------------------------------
// 📌 Проверка наличия (C++20 contains())
// ---------------------------------------------------
void example_has() {
    std::set<int> s = {1, 2, 3};
    
    // C++20/23 способ - более читаемый
    bool has1 = s.contains(1);
    
    // Старый способ (всё ещё работает)
    bool has1_old = s.count(1) > 0;

    std::unordered_map<std::string, int> um = {{"a",1}};
    
    // C++20/23 способ
    bool hasA = um.contains("a");
    
    // Старый способ
    bool hasA_old = um.find("a") != um.end();
}

// ---------------------------------------------------
// 📌 Итерирование
// ---------------------------------------------------
void example_iteration() {
    std::set<int> s = {1, 2, 3};
    for (int val : s) std::println("{}", val);

    std::map<std::string, int> m = { {"k1", 1}, {"k2", 2} };
    for (const auto& kv : m) std::println("{} => {}", kv.first, kv.second);
    
    // C++17 структурированные привязки
    for (const auto& [key, value] : m) {
        std::println("{} => {}", key, value);
    }
}

// ---------------------------------------------------
// 📌 Счётчик частот (std::unordered_map) и сортировка по значению (std::vector + std::sort)
// ---------------------------------------------------
void example_freq(const std::vector<int>& data) {
    std::unordered_map<int, int> freq;
    for (int x : data) {
        ++freq[x];
    }
    std::vector<std::pair<int,int>> vec(freq.begin(), freq.end());
    std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
}

// ---------------------------------------------------
// 📌 Множества: объединение, пересечение, разность (std::set)
// ---------------------------------------------------
void example_set_operations(const std::set<int>& a, const std::set<int>& b) {
    std::set<int> uni;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::inserter(uni, uni.begin()));

    std::set<int> inter;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::inserter(inter, inter.begin()));

    std::set<int> diff;
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::inserter(diff, diff.begin()));
}

// ---------------------------------------------------
// 📌 Мульти-множество (std::multiset) и мультимапа (std::multimap)
// ---------------------------------------------------
void example_multiset_multimap() {
    std::multiset<int> ms = {1, 2, 2, 3};
    ms.insert(2);
    ms.erase(2); // удалит ВСЕ элементы со значением 2

    std::multimap<std::string, int> mm;
    mm.emplace("a", 1);
    mm.emplace("a", 2);
    auto range = mm.equal_range("a");
    for (auto it = range.first; it != range.second; ++it) {
        std::println("{} => {}", it->first, it->second);
    }
}

// ---------------------------------------------------
// 📌 Сравнение flat контейнеров с обычными
// ---------------------------------------------------
void example_flat_vs_normal() {
    // Обычный set - дерево, O(log n) вставка/поиск
    std::set<int> normal_set;
    for (int i = 0; i < 1000; ++i) {
        normal_set.insert(i);
    }
    
    // Flat set - отсортированный вектор
    // Лучше для: частых чтений, итераций, меньшего потребления памяти
    // Хуже для: частых вставок/удалений (требует сдвигов)
    std::flat_set<int> flat_set;
    for (int i = 0; i < 1000; ++i) {
        flat_set.insert(i);
    }
    
    // Когда использовать flat_*:
    // - Контейнер создаётся редко, читается часто
    // - Нужна лучшая cache locality
    // - Важна экономия памяти
    // - Много итераций по всем элементам
}
