#include <iostream>
#include <string>
#include <map>
#include <vector>

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
    std::cout << person.name << std::endl;    // через точку
    std::cout << person.age << std::endl;     // через точку
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
    std::cout << firstName << ", " << height << std::endl;
}

// ---------------------------------------------------
// 📌 Итерация по std::map
// ---------------------------------------------------
void example_map_iteration() {
    std::map<std::string, int> m = { {"age", 25}, {"score", 100} };
    for (const auto& kv : m) {
        std::cout << kv.first << " = " << kv.second << std::endl;
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
    std::cout << "Площадь: " << rect.area() << std::endl;
    std::cout << "Периметр: " << rect.perimeter() << std::endl;
}