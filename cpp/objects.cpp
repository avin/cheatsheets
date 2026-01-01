#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <print>  // C++23

// ---------------------------------------------------
// 📌 Создание структуры и доступ к полям
// ---------------------------------------------------
struct Person {
    std::string name;
    int age;
    Person(const std::string& n, int a) : name(n), age(a) {}
};

void example_struct() {
    Person person{"Иван", 25};
    std::println("{}", person.name);    // через точку
    std::println("{}", person.age);     // через точку
}

// ---------------------------------------------------
// 📌 Деструктуризация объектов (структурированные привязки)
// ---------------------------------------------------
struct Person2 {
    std::string name;
    int height = 180;
    Person2(const std::string& n, int h) : name(n), height(h) {}
};

void example_destructuring() {
    Person2 person{"Иван", 0};
    auto& [firstName, height] = person;
    std::println("{}, {}", firstName, height);
}

// ---------------------------------------------------
// 📌 Итерация по std::map
// ---------------------------------------------------
void example_map_iteration() {
    std::map<std::string, int> m = { {"age", 25}, {"score", 100} };
    for (const auto& kv : m) {
        std::println("{} = {}", kv.first, kv.second);
    }
}

// ---------------------------------------------------
// 📌 Объединение / копирование std::map
// ---------------------------------------------------
void example_merge_maps() {
    std::map<std::string, int> defaults = { {"active", 1}, {"role", 0} };
    std::map<std::string, int> settings = { {"role", 1}, {"theme", 2} };
    std::map<std::string, int> merged = defaults;
    for (const auto& kv : settings) {
        merged[kv.first] = kv.second;
    }
}

// ---------------------------------------------------
// 📌 Проверка наличия ключа в std::map
// ---------------------------------------------------
void example_has_key() {
    std::map<std::string, int> settings = { {"theme", 1} };
    bool hasTheme = settings.find("theme") != settings.end();
    bool hasHeight = settings.count("height") > 0;
}

// ---------------------------------------------------
// 📌 Пример класса с методами и конструктором/деструктором
// ---------------------------------------------------
class Rectangle {
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() const { return width * height; }
    double perimeter() const { return 2 * (width + height); }
private:
    double width;
    double height;
};

void example_class_usage() {
    Rectangle rect(3.0, 4.0);
    std::println("Площадь: {}", rect.area());
    std::println("Периметр: {}", rect.perimeter());
}

// ---------------------------------------------------
// 📌 Deducing this (C++23) - явный объектный параметр
// ---------------------------------------------------
class Builder {
    std::string data;
public:
    // До C++23 нужно было писать 2 версии (const и non-const)
    // или 4 версии (для lvalue и rvalue)
    
    // C++23: один метод работает для всех случаев
    template<typename Self>
    auto&& get_data(this Self&& self) {
        return std::forward<Self>(self).data;
    }
    
    // Упрощает реализацию методов с разной квалификацией
    template<typename Self>
    auto&& with_prefix(this Self&& self, std::string prefix) {
        self.data = prefix + self.data;
        return std::forward<Self>(self);
    }
};

void example_deducing_this() {
    Builder b;
    
    // Автоматически выбирается правильная версия (const/non-const, lvalue/rvalue)
    std::string& data1 = b.get_data();           // lvalue
    const std::string& data2 = std::as_const(b).get_data();  // const lvalue
    std::string data3 = Builder{}.get_data();    // rvalue
}

// ---------------------------------------------------
// 📌 CRTP pattern упрощается с deducing this
// ---------------------------------------------------
template<typename Derived>
class CRTPBase_Old {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};

// C++23: проще и понятнее
class NewBase {
public:
    template<typename Self>
    void interface(this Self&& self) {
        self.implementation();
    }
};

class Derived : public NewBase {
public:
    void implementation() {
        std::println("Implementation called");
    }
};

// ---------------------------------------------------
// 📌 Многомерный subscript оператор (C++23)
// ---------------------------------------------------
class Matrix {
    std::vector<std::vector<int>> data;
public:
    Matrix(size_t rows, size_t cols) : data(rows, std::vector<int>(cols, 0)) {}
    
    // C++23: можно использовать operator[] с несколькими аргументами
    int& operator[](size_t row, size_t col) {
        return data[row][col];
    }
    
    const int& operator[](size_t row, size_t col) const {
        return data[row][col];
    }
};

void example_multidim_subscript() {
    Matrix m(3, 4);
    
    // C++23: прямой доступ как к многомерному массиву
    m[0, 1] = 42;
    m[1, 2] = 100;
    
    int value = m[0, 1];
    std::println("Value at [0,1]: {}", value);
    
    // До C++23 приходилось писать: m[0][1] или m.at(0, 1)
}
