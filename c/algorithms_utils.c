#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------
// 📌 Компаратор для qsort/bsearch
// ---------------------------------------------------
int compare_ints(const void *a, const void *b) {
    const int lhs = *(const int *)a;
    const int rhs = *(const int *)b;
    if (lhs < rhs) return -1;
    if (lhs > rhs) return 1;
    return 0;
}

// ---------------------------------------------------
// 📌 Сортировка и поиск
// ---------------------------------------------------
void sort_and_search(int *values, size_t length, int target) {
    qsort(values, length, sizeof(int), compare_ints);

    printf("sorted: ");
    for (size_t i = 0; i < length; ++i) {
        printf("%d ", values[i]);
    }
    puts("");

    int *found = bsearch(&target, values, length, sizeof(int), compare_ints);
    if (found) {
        printf("found %d at index %ld\n", target, found - values);
    } else {
        printf("%d not found\n", target);
    }
}

// ---------------------------------------------------
// 📌 Стек на массиве
// ---------------------------------------------------
typedef struct {
    int *data;
    size_t capacity;
    size_t top;
} IntStack;

IntStack stack_create(size_t capacity) {
    IntStack stack = {0};
    stack.data = malloc(capacity * sizeof(int));
    stack.capacity = capacity;
    stack.top = 0;
    return stack;
}

void stack_push(IntStack *stack, int value) {
    if (stack->top >= stack->capacity) {
        puts("stack overflow");
        return;
    }
    stack->data[stack->top++] = value;
}

int stack_pop(IntStack *stack) {
    if (stack->top == 0) {
        puts("stack is empty");
        return 0;
    }
    return stack->data[--stack->top];
}

void stack_destroy(IntStack *stack) {
    free(stack->data);
    stack->data = NULL;
    stack->capacity = stack->top = 0;
}

// ---------------------------------------------------
// 📌 Очередь на кольцевом буфере
// ---------------------------------------------------
typedef struct {
    int *data;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t size;
} IntQueue;

IntQueue queue_create(size_t capacity) {
    IntQueue queue = {0};
    queue.data = malloc(capacity * sizeof(int));
    queue.capacity = capacity;
    return queue;
}

void queue_push(IntQueue *queue, int value) {
    if (queue->size == queue->capacity) {
        puts("queue overflow");
        return;
    }
    queue->data[queue->tail] = value;
    queue->tail = (queue->tail + 1) % queue->capacity;
    ++queue->size;
}

int queue_pop(IntQueue *queue) {
    if (queue->size == 0) {
        puts("queue empty");
        return 0;
    }
    int value = queue->data[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    --queue->size;
    return value;
}

void queue_destroy(IntQueue *queue) {
    free(queue->data);
    queue->data = NULL;
    queue->capacity = queue->head = queue->tail = queue->size = 0;
}

int main(void) {
    int values[] = {5, 2, 9, 1, 7};
    sort_and_search(values, sizeof(values) / sizeof(values[0]), 7);

    IntStack stack = stack_create(4);
    stack_push(&stack, 10);
    stack_push(&stack, 20);
    printf("pop: %d\n", stack_pop(&stack));
    stack_destroy(&stack);

    IntQueue queue = queue_create(4);
    queue_push(&queue, 1);
    queue_push(&queue, 2);
    printf("queue pop: %d\n", queue_pop(&queue));
    queue_destroy(&queue);

    return 0;
}
