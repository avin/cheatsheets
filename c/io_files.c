#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------
// 📌 Чтение текстового файла построчно
// ---------------------------------------------------
void read_lines(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("fopen");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        printf("line (%zd): %s", read, line);
    }

    free(line);
    fclose(file);
}

// ---------------------------------------------------
// 📌 Запись в текстовый файл
// ---------------------------------------------------
void write_report(const char *path, const char *title, const char *body) {
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("fopen");
        return;
    }

    fprintf(file, "# %s\n\n%s\n", title, body);
    fclose(file);
}

// ---------------------------------------------------
// 📌 Бинарное чтение и запись
// ---------------------------------------------------
typedef struct {
    int id;
    double value;
} Record;

void save_records(const char *path, const Record *records, size_t count) {
    FILE *file = fopen(path, "wb");
    if (!file) {
        perror("fopen");
        return;
    }
    size_t written = fwrite(records, sizeof(Record), count, file);
    if (written != count) {
        perror("fwrite");
    }
    fclose(file);
}

void load_records(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("fopen");
        return;
    }

    Record record;
    while (fread(&record, sizeof(record), 1, file) == 1) {
        printf("record id=%d value=%.2f\n", record.id, record.value);
    }
    if (!feof(file)) {
        perror("fread");
    }
    fclose(file);
}

// ---------------------------------------------------
// 📌 Позиционирование в файле
// ---------------------------------------------------
long file_size(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        return -1;
    }
    if (fseek(file, 0L, SEEK_END) != 0) {
        fclose(file);
        return -1;
    }
    long size = ftell(file);
    fclose(file);
    return size;
}

int main(void) {
    write_report("report.txt", "Metrics", "CPU usage: 15%\nMemory: 128MB");
    read_lines("report.txt");

    Record records[] = {
        {.id = 1, .value = 1.5},
        {.id = 2, .value = 3.75},
    };
    save_records("data.bin", records, sizeof(records) / sizeof(records[0]));
    load_records("data.bin");

    long size = file_size("data.bin");
    printf("data.bin size: %ld bytes\n", size);

    return 0;
}
