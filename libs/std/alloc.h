#ifndef STD_ALLOC_H
#define STD_ALLOC_H

#include <std/bool.h>
#include <std/int.h>

typedef bool (*AllocateFunc)(u8 **ptr, size_t bytes);
typedef void (*FreeFunc)(u8 *ptr);

// allocator contains function pointers to dynamically allocate and free memory
typedef struct {
    AllocateFunc alloc;
    FreeFunc free;
} Allocator;

// get global allocator
Allocator alloc();

// init global allocator
void init_alloc(Allocator allocator);

#endif
