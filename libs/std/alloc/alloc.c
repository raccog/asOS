#include <std/alloc.h>

Allocator _allocator;

Allocator alloc() {
    return _allocator;
}

void init_alloc(Allocator allocator) {
    _allocator = allocator;
}

