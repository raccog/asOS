#include <stdbool.h>
#include <stdint.h>

typedef bool (*AllocateFunc)(uint8_t **ptr, size_t bytes);
typedef void (*FreeFunc)(uint8_t *ptr);

typedef struct {
    AllocateFunc alloc;
    FreeFunc free;
} Allocator;

Allocator alloc();

void init_alloc(Allocator allocator);
