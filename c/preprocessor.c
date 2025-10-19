#include <stdio.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#if defined(DEBUG)
#define LOG_DEBUG(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", __VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) (void)0
#endif

static inline int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int main(void) {
    int numbers[] = {4, 8, 15, 16, 23, 42};
    printf("array size: %zu\n", ARRAY_SIZE(numbers));
    printf("max(10, 7)=%d\n", MAX(10, 7));

    LOG_DEBUG("file=%s line=%s", __FILE__, TOSTRING(__LINE__));

    printf("clamp 120 to [0, 100] -> %d\n", clamp(120, 0, 100));

    return 0;
}
