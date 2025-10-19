#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------
// 📌 Простейший assert и логирование
// ---------------------------------------------------
int divide(int a, int b) {
    assert(b != 0);
    return a / b;
}

// ---------------------------------------------------
// 📌 perror и strerror
// ---------------------------------------------------
void open_file_or_die(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "unable to open %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

// ---------------------------------------------------
// 📌 Минималистичные тесты
// ---------------------------------------------------
typedef void (*TestCase)(void);

void test_divide(void) {
    if (divide(10, 2) != 5) {
        fprintf(stderr, "test_divide failed\n");
        exit(EXIT_FAILURE);
    }
}

void run_tests(const TestCase *tests, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        tests[i]();
    }
    puts("all tests passed");
}

int main(void) {
    TestCase tests[] = {test_divide};
    run_tests(tests, sizeof(tests) / sizeof(tests[0]));

    // Продемонстрировать perror (закомментировано чтобы не завершать программу)
    // open_file_or_die("missing.txt");

    return 0;
}
