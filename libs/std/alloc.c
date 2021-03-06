#include <std/alloc.h>

// memory for global allocator
static Allocator _allocator;

Allocator alloc() {
    return _allocator;
}

void init_alloc(Allocator allocator) {
    _allocator = allocator;
}

