#ifndef SUNNY_LOG_H
#define SUNNY_LOG_H

#include <std/scanner.h>
#include <std/printf.h>

#define simple_log(fmt, ...) \
    simple_printf("[%s:%i] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif
