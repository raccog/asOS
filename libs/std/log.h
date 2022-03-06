#ifndef STD_LOG_H
#define STD_LOG_H

#include <std/scanner.h>
#include <std/printf.h>

#define simple_log(fmt, ...) \
    simple_printf("[%s-%s:%s:%i] " fmt "\n", __LIB__, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#endif
