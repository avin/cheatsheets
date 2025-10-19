#include <stdio.h>

// ---------------------------------------------------
// 📌 Условные операторы
// ---------------------------------------------------
void check_temperature(int celsius) {
    if (celsius < 0) {
        puts("below freezing");
    } else if (celsius < 25) {
        puts("mild weather");
    } else {
        puts("hot");
    }

    const char *status = (celsius >= 18 && celsius <= 24) ? "comfortable" : "adjust thermostat";
    printf("status: %s\n", status);
}

// ---------------------------------------------------
// 📌 switch-case
// ---------------------------------------------------
void describe_day(int weekday) {
    switch (weekday) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            puts("workday");
            break;
        case 6:
        case 7:
            puts("weekend");
            break;
        default:
            puts("unknown day");
    }
}

// ---------------------------------------------------
// 📌 Циклы for/while/do-while
// ---------------------------------------------------
void loop_examples(void) {
    for (int i = 0; i < 3; ++i) {
        printf("for loop i=%d\n", i);
    }

    int countdown = 3;
    while (countdown > 0) {
        printf("countdown=%d\n", countdown);
        --countdown;
    }

    int idx = 0;
    do {
        printf("do-while idx=%d\n", idx);
        ++idx;
    } while (idx < 2);
}

// ---------------------------------------------------
// 📌 Использование goto и меток
// ---------------------------------------------------
void find_first_positive(const int *values, size_t length) {
    size_t i = 0;
search:
    if (i == length) {
        puts("no positive number");
        return;
    }
    if (values[i] > 0) {
        printf("first positive: %d at index %zu\n", values[i], i);
        return;
    }
    ++i;
    goto search;
}

int main(void) {
    check_temperature(10);
    check_temperature(28);

    describe_day(6);
    describe_day(9);

    loop_examples();

    int data[] = {-5, -3, 0, 7, 9};
    find_first_positive(data, sizeof(data) / sizeof(data[0]));

    return 0;
}
