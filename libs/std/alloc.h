#ifndef STD_ALLOC_H
#define STD_ALLOC_H

typedef bool (*AllocateFunc)(u8 **ptr, size_t bytes);
typedef void (*FreeFunc)(u8 *ptr);

// Allocator contains function pointers to dynamically allocate and free heap memory
typedef struct {
    AllocateFunc alloc;
    FreeFunc free;
} Allocator;

// Get global allocator
Allocator alloc();

// Set global allocator
void init_alloc(Allocator allocator);

#endif
