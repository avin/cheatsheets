#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------
// 📌 Копирование и конкатенация
// ---------------------------------------------------
void copy_and_concat(void) {
    char buffer[64];
    strcpy(buffer, "Hello");
    strcat(buffer, ", ");
    strncat(buffer, "world!", sizeof(buffer) - strlen(buffer) - 1);
    printf("%s\n", buffer);
}

// ---------------------------------------------------
// 📌 Безопасное форматирование
// ---------------------------------------------------
void safe_formatting(const char *name, int age) {
    char formatted[64];
    int written = snprintf(formatted, sizeof(formatted), "%s is %d years old", name, age);
    if (written < 0 || (size_t)written >= sizeof(formatted)) {
        fputs("output truncated\n", stderr);
    }
    printf("%s\n", formatted);
}

// ---------------------------------------------------
// 📌 Поиск и сравнение
// ---------------------------------------------------
void search_examples(const char *text) {
    if (strstr(text, "C language")) {
        puts("phrase found");
    }

    if (strncmp(text, "Hello", 5) == 0) {
        puts("text starts with Hello");
    }
}

// ---------------------------------------------------
// 📌 Разбиение строки
// ---------------------------------------------------
void split_csv(char *line) {
    const char *delimiter = ",";
    char *token = strtok(line, delimiter);
    while (token) {
        printf("token: %s\n", token);
        token = strtok(NULL, delimiter);
    }
}

int main(void) {
    copy_and_concat();
    safe_formatting("Alice", 32);
    search_examples("Hello C language community");

    char csv[] = "red,green,blue";
    split_csv(csv);

    return 0;
}
