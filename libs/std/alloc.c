#include <std/alloc.h>

#include <std/log.h>

// memory for global allocator
Allocator _allocator;

Allocator alloc() {
    return _allocator;
}

void init_alloc(Allocator allocator) {
    _allocator = allocator;
}

