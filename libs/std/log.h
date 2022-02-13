#ifndef LOG_H
#define LOG_H

#include <efi/efi.h>
#include <std/log/scanner.h>

typedef struct {
    bool left_justify;
    bool force_positive_sign;
    bool positive_space;
    bool pound;
    bool left_pad_zero;

    size_t width;
    size_t precision;

    bool is_short;
    bool is_long;
    bool is_long_double;
} FormatDescriptor;

EfiStatus log(const char *fmt, ...);

#endif
