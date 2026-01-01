#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <print>  // C++23

// ---------------------------------------------------
// 📌 Базовое определение класса с конструктором и деструктором
// ---------------------------------------------------
class BasicClass {
public:
    BasicClass() {
        std::println("Конструктор по умолчанию");
    }
    
    BasicClass(int val) : value(val) {
        std::println("Конструктор с параметром: {}", val);
    }
    
    ~BasicClass() {
        std::println("Деструктор");
    }
    
    void display() const {
        std::println("Value: {}", value);
    }
    
private:
    int value = 0;
};

// ---------------------------------------------------
// 📌 Конструктор копирования и оператор присваивания
// ---------------------------------------------------
class CopyableClass {
public:
    CopyableClass(int val) : data(new int(val)) {}
    
    // Конструктор копирования
    CopyableClass(const CopyableClass& other) : data(new int(*other.data)) {
        std::println("Copy constructor called");
    }
    
    // Оператор присваивания копированием
    CopyableClass& operator=(const CopyableClass& other) {
        std::println("Copy assignment operator called");
        if (this != &other) {
            delete data;
            data = new int(*other.data);
        }
        return *this;
    }
    
    ~CopyableClass() {
        delete data;
    }
    
private:
    int* data;
};

// ---------------------------------------------------
// 📌 Конструктор перемещения и оператор присваивания перемещением (Move semantics)
// ---------------------------------------------------
class MovableClass {
public:
    MovableClass(int val) : data(new int(val)) {}
    
    // Конструктор перемещения
    MovableClass(MovableClass&& other) noexcept : data(other.data) {
        other.data = nullptr;
        std::println("Move constructor called");
    }
    
    // Оператор присваивания перемещением
    MovableClass& operator=(MovableClass&& other) noexcept {
        std::println("Move assignment operator called");
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
    
    ~MovableClass() {
        delete data;
    }
    
private:
    int* data;
};

// ---------------------------------------------------
// 📌 Правило пяти (Rule of Five)
// ---------------------------------------------------
class RuleOfFive {
public:
    RuleOfFive() = default;
    RuleOfFive(const RuleOfFive& other);                // copy constructor
    RuleOfFive(RuleOfFive&& other) noexcept;            // move constructor
    RuleOfFive& operator=(const RuleOfFive& other);     // copy assignment
    RuleOfFive& operator=(RuleOfFive&& other) noexcept; // move assignment
    ~RuleOfFive();                                      // destructor
};

// ---------------------------------------------------
// 📌 Правило нуля (Rule of Zero) - использование умных указателей
// ---------------------------------------------------
class RuleOfZero {
public:
    RuleOfZero(int val) : data(std::make_unique<int>(val)) {}
    // Компилятор автоматически генерирует все специальные функции
private:
    std::unique_ptr<int> data;
};

// ---------------------------------------------------
// 📌 Базовое наследование
// ---------------------------------------------------
class Animal {
public:
    Animal(const std::string& n) : name(n) {}
    virtual ~Animal() = default;
    
    virtual void makeSound() const {
        std::println("{} издаёт звук", name);
    }
    
    void describe() const {
        std::println("Это животное по имени {}", name);
    }
    
protected:
    std::string name;
};

class Dog : public Animal {
public:
    Dog(const std::string& n) : Animal(n) {}
    
    void makeSound() const override {
        std::println("{} гавкает: Гав-гав!", name);
    }
};

class Cat : public Animal {
public:
    Cat(const std::string& n) : Animal(n) {}
    
    void makeSound() const override {
        std::println("{} мяукает: Мяу!", name);
    }
};

// ---------------------------------------------------
// 📌 Виртуальные функции и полиморфизм
// ---------------------------------------------------
void example_polymorphism() {
    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Dog>("Шарик"));
    animals.push_back(std::make_unique<Cat>("Мурка"));
    
    for (const auto& animal : animals) {
        animal->makeSound();  // Вызов виртуальной функции
    }
}

// ---------------------------------------------------
// 📌 Абстрактный класс и чисто виртуальные функции
// ---------------------------------------------------
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;      // Чисто виртуальная функция
    virtual double perimeter() const = 0;
};

class Circle : public Shape {
public:
    Circle(double r) : radius(r) {}
    
    double area() const override {
        return 3.14159 * radius * radius;
    }
    
    double perimeter() const override {
        return 2 * 3.14159 * radius;
    }
    
private:
    double radius;
};

// ---------------------------------------------------
// 📌 Множественное наследование
// ---------------------------------------------------
class Walkable {
public:
    virtual ~Walkable() = default;
    virtual void walk() const {
        std::println("Ходит");
    }
};

class Swimmable {
public:
    virtual ~Swimmable() = default;
    virtual void swim() const {
        std::println("Плавает");
    }
};

class Duck : public Walkable, public Swimmable {
public:
    void walk() const override {
        std::println("Утка ходит");
    }
    
    void swim() const override {
        std::println("Утка плавает");
    }
};

// ---------------------------------------------------
// 📌 Виртуальное наследование (решение проблемы diamond)
// ---------------------------------------------------
class Base {
public:
    int value = 42;
};

class Derived1 : virtual public Base {};
class Derived2 : virtual public Base {};

class Final : public Derived1, public Derived2 {
    // Теперь value не дублируется, есть только один экземпляр от Base
};

// ---------------------------------------------------
// 📌 Перегрузка операторов
// ---------------------------------------------------
class Complex {
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    // Перегрузка оператора +
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
    
    // Перегрузка оператора +=
    Complex& operator+=(const Complex& other) {
        real += other.real;
        imag += other.imag;
        return *this;
    }
    
    // Перегрузка оператора ==
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }
    
    // Перегрузка оператора вывода (friend function)
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real << " + " << c.imag << "i";
        return os;
    }
    
    // Перегрузка оператора []
    double& operator[](size_t index) {
        return index == 0 ? real : imag;
    }
    
    // Перегрузка оператора вызова функции ()
    double operator()() const {
        return real * real + imag * imag;  // Модуль в квадрате
    }
    
private:
    double real, imag;
};

// ---------------------------------------------------
// 📌 Перегрузка операторов инкремента/декремента
// ---------------------------------------------------
class Counter {
public:
    Counter(int v = 0) : value(v) {}
    
    // Префиксный инкремент
    Counter& operator++() {
        ++value;
        return *this;
    }
    
    // Постфиксный инкремент
    Counter operator++(int) {
        Counter temp = *this;
        ++value;
        return temp;
    }
    
    int getValue() const { return value; }
    
private:
    int value;
};

// ---------------------------------------------------
// 📌 Оператор приведения типа
// ---------------------------------------------------
class Wrapper {
public:
    Wrapper(int v) : value(v) {}
    
    // Оператор неявного преобразования в int
    operator int() const {
        return value;
    }
    
    // Явный оператор преобразования (C++11)
    explicit operator bool() const {
        return value != 0;
    }
    
private:
    int value;
};

// ---------------------------------------------------
// 📌 Статические члены класса
// ---------------------------------------------------
class MyClass {
public:
    static int staticCounter;
    
    MyClass() {
        ++staticCounter;
    }
    
    static void printCount() {
        std::println("Создано объектов: {}", staticCounter);
    }
};

// Определение статической переменной вне класса
int MyClass::staticCounter = 0;

// ---------------------------------------------------
// 📌 Константные методы и mutable
// ---------------------------------------------------
class Cache {
public:
    int compute(int x) const {
        if (cacheValid) {
            return cachedValue;
        }
        cachedValue = x * x;  // Модификация mutable члена в const методе
        cacheValid = true;
        return cachedValue;
    }
    
private:
    mutable int cachedValue = 0;
    mutable bool cacheValid = false;
};

// ---------------------------------------------------
// 📌 Friend функции и классы
// ---------------------------------------------------
class Box;  // Forward declaration

class BoxPrinter {
public:
    void printBox(const Box& box);
};

class Box {
    friend class BoxPrinter;  // BoxPrinter может обращаться к private членам
    friend void externalPrint(const Box& box);  // friend функция
    
public:
    Box(int w, int h) : width(w), height(h) {}
    
private:
    int width, height;
};

void externalPrint(const Box& box) {
    std::println("Box: {}x{}", box.width, box.height);
}

void BoxPrinter::printBox(const Box& box) {
    std::println("Printer: Box is {}x{}", box.width, box.height);
}

// ---------------------------------------------------
// 📌 Вложенные классы
// ---------------------------------------------------
class Outer {
public:
    class Inner {
    public:
        void display() {
            std::println("Inner class method");
        }
    };
    
    Inner getInner() {
        return Inner();
    }
};

// ---------------------------------------------------
// 📌 CRTP (Curiously Recurring Template Pattern)
// ---------------------------------------------------
template<typename Derived>
class Base_CRTP {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};

class Derived_CRTP : public Base_CRTP<Derived_CRTP> {
public:
    void implementation() {
        std::println("CRTP implementation");
    }
};

// ---------------------------------------------------
// 📌 Использование final и override
// ---------------------------------------------------
class BaseClass {
public:
    virtual void method1() {}
    virtual void method2() {}
};

class DerivedClass : public BaseClass {
public:
    void method1() override final {  // Переопределяет и запрещает дальнейшее переопределение
        std::println("Derived method1");
    }
    
    void method2() override {  // Переопределяет
        std::println("Derived method2");
    }
};

class FinalClass final : public BaseClass {  // Класс нельзя наследовать
    void method1() override {}
};

// ---------------------------------------------------
// 📌 Делегирующие конструкторы (C++11)
// ---------------------------------------------------
class Point {
public:
    Point() : Point(0, 0) {}  // Делегирует другому конструктору
    Point(int x) : Point(x, 0) {}
    Point(int x, int y) : x_(x), y_(y) {}
    
private:
    int x_, y_;
};

// ---------------------------------------------------
// 📌 Наследование конструкторов (C++11)
// ---------------------------------------------------
class Base2 {
public:
    Base2(int x) : value(x) {}
protected:
    int value;
};

class Derived2 : public Base2 {
public:
    using Base2::Base2;  // Наследует конструкторы базового класса
};

// ---------------------------------------------------
// 📌 Default и delete для специальных функций
// ---------------------------------------------------
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;            // Запрет копирования
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;                // Разрешено перемещение
    NonCopyable& operator=(NonCopyable&&) = default;
};

// ---------------------------------------------------
// 📌 Explicit объектный параметр (Deducing this, C++23)
// ---------------------------------------------------
struct Data23 {
    std::string value;
    
    // Один метод для всех квалификаторов (const, &, &&)
    template<typename Self>
    auto&& getValue(this Self&& self) {
        return std::forward<Self>(self).value;
    }
};

// ---------------------------------------------------
// 📌 Паттерн Pimpl (Pointer to Implementation)
// ---------------------------------------------------
class Widget {
public:
    Widget();
    ~Widget();
    
    void doSomething();
    
private:
    class Impl;  // Forward declaration
    std::unique_ptr<Impl> pImpl;
};

// В .cpp файле:
// class Widget::Impl {
//     // Приватная реализация
// };
//
// Widget::Widget() : pImpl(std::make_unique<Impl>()) {}
// Widget::~Widget() = default;
// void Widget::doSomething() { pImpl->doWork(); }

int main() {
    // Примеры использования
    BasicClass obj1;
    BasicClass obj2(42);
    obj2.display();
    
    // Полиморфизм
    example_polymorphism();
    
    // Перегрузка операторов
    Complex c1(1, 2);
    Complex c2(3, 4);
    Complex c3 = c1 + c2;
    std::cout << "c3 = " << c3 << std::endl;
    
    // Множественное наследование
    Duck donald;
    donald.walk();
    donald.swim();
    
    // Счетчик объектов
    MyClass::printCount();
    MyClass a, b, c;
    MyClass::printCount();
    
    return 0;
}
