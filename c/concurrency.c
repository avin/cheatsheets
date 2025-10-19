#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ---------------------------------------------------
// 📌 Общая работа с pthreads
// ---------------------------------------------------
typedef struct {
    int id;
    const char *message;
} WorkerArgs;

void *worker_routine(void *arg) {
    WorkerArgs *args = arg;
    printf("worker %d: %s\n", args->id, args->message);
    return NULL;
}

void spawn_workers(void) {
    pthread_t threads[2];
    WorkerArgs args[] = {
        {.id = 1, .message = "hello"},
        {.id = 2, .message = "world"},
    };

    for (size_t i = 0; i < 2; ++i) {
        if (pthread_create(&threads[i], NULL, worker_routine, &args[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    for (size_t i = 0; i < 2; ++i) {
        pthread_join(threads[i], NULL);
    }
}

// ---------------------------------------------------
// 📌 Мьютексы и условные переменные
// ---------------------------------------------------
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
    int ready;
} SharedState;

void *producer(void *arg) {
    SharedState *state = arg;
    sleep(1);
    pthread_mutex_lock(&state->mutex);
    state->value = 42;
    state->ready = 1;
    pthread_cond_signal(&state->cond);
    pthread_mutex_unlock(&state->mutex);
    return NULL;
}

void *consumer(void *arg) {
    SharedState *state = arg;
    pthread_mutex_lock(&state->mutex);
    while (!state->ready) {
        pthread_cond_wait(&state->cond, &state->mutex);
    }
    printf("consumer received: %d\n", state->value);
    pthread_mutex_unlock(&state->mutex);
    return NULL;
}

// ---------------------------------------------------
// 📌 Атомики и lock-free счётчики
// ---------------------------------------------------
typedef struct {
    atomic_int counter;
} AtomicCounter;

void *incrementer(void *arg) {
    AtomicCounter *c = arg;
    for (int i = 0; i < 100000; ++i) {
        atomic_fetch_add_explicit(&c->counter, 1, memory_order_relaxed);
    }
    return NULL;
}

int main(void) {
    spawn_workers();

    SharedState state = {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .cond = PTHREAD_COND_INITIALIZER,
        .value = 0,
        .ready = 0,
    };

    pthread_t producer_thread;
    pthread_t consumer_thread;
    pthread_create(&producer_thread, NULL, producer, &state);
    pthread_create(&consumer_thread, NULL, consumer, &state);
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    AtomicCounter counter;
    atomic_init(&counter.counter, 0);
    pthread_t threads[4];
    for (size_t i = 0; i < 4; ++i) {
        pthread_create(&threads[i], NULL, incrementer, &counter);
    }
    for (size_t i = 0; i < 4; ++i) {
        pthread_join(threads[i], NULL);
    }
    printf("counter=%d\n", atomic_load(&counter.counter));

    return 0;
}
