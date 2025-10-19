#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------
// 📌 Указатели и арифметика
// ---------------------------------------------------
void pointer_arithmetic(int *array, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("value[%zu]=%d address=%p\n", i, array[i], (void *)&array[i]);
    }

    int *ptr = array;
    printf("*ptr=%d *(ptr+2)=%d\n", *ptr, *(ptr + 2));
}

// ---------------------------------------------------
// 📌 Динамическое выделение памяти
// ---------------------------------------------------
int *create_sequence(size_t length) {
    int *buffer = malloc(length * sizeof(int));
    if (!buffer) {
        perror("malloc");
        return NULL;
    }
    for (size_t i = 0; i < length; ++i) {
        buffer[i] = (int)(i * i);
    }
    return buffer;
}

void resize_sequence(int **buffer, size_t old_length, size_t new_length) {
    int *resized = realloc(*buffer, new_length * sizeof(int));
    if (!resized) {
        free(*buffer);
        *buffer = NULL;
        return;
    }
    for (size_t i = old_length; i < new_length; ++i) {
        resized[i] = (int)(i * i);
    }
    *buffer = resized;
}

// ---------------------------------------------------
// 📌 Двойные указатели и освобождение
// ---------------------------------------------------
void free_matrix(int **matrix, size_t rows) {
    if (!matrix) {
        return;
    }
    for (size_t r = 0; r < rows; ++r) {
        free(matrix[r]);
    }
    free(matrix);
}

int **create_identity_matrix(size_t size) {
    int **matrix = calloc(size, sizeof(int *));
    if (!matrix) {
        return NULL;
    }
    for (size_t r = 0; r < size; ++r) {
        matrix[r] = calloc(size, sizeof(int));
        if (!matrix[r]) {
            free_matrix(matrix, r);
            return NULL;
        }
        matrix[r][r] = 1;
    }
    return matrix;
}

int main(void) {
    int sample[] = {1, 2, 3, 4};
    pointer_arithmetic(sample, sizeof(sample) / sizeof(sample[0]));

    size_t length = 4;
    int *seq = create_sequence(length);
    if (!seq) {
        return EXIT_FAILURE;
    }

    resize_sequence(&seq, length, 6);
    if (!seq) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < 6; ++i) {
        printf("seq[%zu]=%d\n", i, seq[i]);
    }
    free(seq);

    int **identity = create_identity_matrix(3);
    if (identity) {
        for (size_t r = 0; r < 3; ++r) {
            for (size_t c = 0; c < 3; ++c) {
                printf("%d ", identity[r][c]);
            }
            puts("");
        }
        free_matrix(identity, 3);
    }

    return 0;
}
