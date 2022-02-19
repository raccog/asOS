#ifndef LOG_H
#define LOG_H

#include <std/log/scanner.h>

#define simple_log(fmt, ...) \
    simple_printf("[%s:%i] " fmt "\n", __FILE__, __LINE__, __VA_ARGS__)

void simple_printf(const char *fmt, ...);

void init_print_buffer();

#endif
