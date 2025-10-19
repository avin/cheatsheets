#include <stdio.h>
#include <stdbool.h>

// ---------------------------------------------------
// 📌 Переменные, константы и базовые типы
// ---------------------------------------------------
void basics_variables(void) {
    int counter = 42;
    const double pi = 3.1415926535;
    char initial = 'A';
    bool enabled = true;

    printf("counter=%d pi=%.2f initial=%c enabled=%d\n", counter, pi, initial, enabled);
}

// ---------------------------------------------------
// 📌 Функции и прототипы
// ---------------------------------------------------
int add(int a, int b); // прототип позволяет вызывать функцию до определения

int add(int a, int b) {
    return a + b;
}

// ---------------------------------------------------
// 📌 Передача параметров по значению
// ---------------------------------------------------
void swap_by_value(int a, int b) {
    int tmp = a;
    a = b;
    b = tmp;
    printf("inside swap_by_value: a=%d b=%d\n", a, b);
}

// ---------------------------------------------------
// 📌 Указатели и передача по адресу
// ---------------------------------------------------
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// ---------------------------------------------------
// 📌 Функция main
// ---------------------------------------------------
int main(void) {
    basics_variables();

    int result = add(10, 32);
    printf("10 + 32 = %d\n", result);

    int x = 1;
    int y = 2;
    swap_by_value(x, y);
    printf("after swap_by_value: x=%d y=%d\n", x, y);

    swap(&x, &y);
    printf("after swap: x=%d y=%d\n", x, y);

    return 0;
}
