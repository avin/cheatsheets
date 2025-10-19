#include <stdio.h>
#include <stdint.h>
#include <string.h>

// ---------------------------------------------------
// 📌 Структуры и typedef
// ---------------------------------------------------
typedef struct {
    char name[32];
    uint16_t year;
    double rating;
} Movie;

Movie create_movie(const char *name, uint16_t year, double rating) {
    Movie movie = {0};
    strncpy(movie.name, name, sizeof(movie.name) - 1);
    movie.year = year;
    movie.rating = rating;
    return movie;
}

// ---------------------------------------------------
// 📌 Вложенные структуры и битовые поля
// ---------------------------------------------------
typedef struct {
    unsigned major : 4;
    unsigned minor : 4;
    unsigned patch : 8;
} Version;

typedef struct {
    Movie movie;
    Version version;
} Release;

// ---------------------------------------------------
// 📌 enum и union
// ---------------------------------------------------
typedef enum {
    EVENT_NONE,
    EVENT_KEY,
    EVENT_MOUSE,
} EventType;

typedef struct {
    EventType type;
    union {
        struct {
            int keycode;
        } key;
        struct {
            int x;
            int y;
        } mouse;
    } data;
} Event;

void handle_event(const Event *event) {
    switch (event->type) {
        case EVENT_KEY:
            printf("key pressed: %d\n", event->data.key.keycode);
            break;
        case EVENT_MOUSE:
            printf("mouse at (%d, %d)\n", event->data.mouse.x, event->data.mouse.y);
            break;
        default:
            puts("no event");
    }
}

int main(void) {
    Movie movie = create_movie("The Matrix", 1999, 8.7);
    printf("%s (%u) rating %.1f\n", movie.name, movie.year, movie.rating);

    Release release = {
        .movie = movie,
        .version = {.major = 1, .minor = 2, .patch = 5},
    };
    printf("release v%u.%u.%u\n", release.version.major, release.version.minor, release.version.patch);

    Event key_event = {.type = EVENT_KEY, .data.key = {.keycode = 65}};
    Event mouse_event = {.type = EVENT_MOUSE, .data.mouse = {.x = 120, .y = 64}};
    handle_event(&key_event);
    handle_event(&mouse_event);

    return 0;
}
