#ifndef STD_LOG_H
#define STD_LOG_H

#include <std/log/scanner.h>

#define simple_log(fmt, ...) \
    simple_printf("[%s:%i] " fmt "\n", __FILE__, __LINE__, __VA_ARGS__)

void simple_printf(const char *fmt, ...);

// allocates print buffer using dynamic allocator
void alloc_print_buffer();

// allocates print buffer using stack
void stack_alloc_print_buffer();

#endif
