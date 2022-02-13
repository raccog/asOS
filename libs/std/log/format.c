#include <stdarg.h>

#include <std/alloc.h>
#include <std/io.h>
#include <std/log.h>

EfiStatus _status;

#define STATUS(s) \
    _status = s; \
    if (_status != EFI_SUCCESS) { \
        return _status; \
    }

void pad_scanner(Scanner *scanner, char c, size_t n) {
    while (n > 0) {
        scanner_put_char(scanner, c);
        --n;
    }
}

void print_integer(Scanner *scanner, FormatDescriptor *descriptor, int value) {
    int divisor = 1;
    size_t digit_count = 0;

    if (value < 0) {
        scanner_put_char(scanner, '-');
        value *= -1;
    } else if (descriptor->force_positive_sign) {
        scanner_put_char(scanner, '+');
    } else if (descriptor->positive_space) {
        scanner_put_char(scanner, ' ');
    }

    do {
        divisor *= 10;
        ++digit_count;
    } while (value / divisor > 0);

    divisor /= 10;

    if (!descriptor->left_justify) {
        if (descriptor->width > 0) {
            if (descriptor->left_pad_zero && descriptor->precision == 0) {
                pad_scanner(scanner, '0', descriptor->width - digit_count);
            } else if (descriptor->width > descriptor->precision) {
                if (descriptor->precision > digit_count) {
                    pad_scanner(scanner, ' ', descriptor->width - descriptor->precision);
                    pad_scanner(scanner, '0', descriptor->precision - digit_count);
                } else {
                    pad_scanner(scanner, ' ', descriptor->width - digit_count);
                }
            }
        } else if (descriptor->precision > 0 && descriptor->precision > digit_count) {
            pad_scanner(scanner, '0', descriptor->precision - digit_count);
        }
    }
    
    for (size_t i = 0; i < digit_count; ++i) {
        scanner_put_char(scanner, '9' - (char)(9 - value / divisor));
        value = value % divisor;
        divisor /= 10;
    }
}

bool parse_percent(Scanner *scanner) {
    char c = scanner_current_char(scanner);

    if (c == '%') {
        scanner_put_char(scanner, c);
        ++(scanner->in_idx);
        return true;
    }
    return false;
}

bool parse_flags(Scanner *scanner, FormatDescriptor *descriptor) {
    char c;

    while (true) {
        c = scanner_next_char(scanner);
        switch (c) {
            case '-':
                if (descriptor->left_justify) {
                    return false;
                }
                descriptor->left_justify = true;
                break;
            case '+':
                if (descriptor->force_positive_sign) {
                    return false;
                }
                descriptor->force_positive_sign = true;
                break;
            case ' ':
                if (descriptor->positive_space) {
                    return false;
                }
                descriptor->positive_space = true;
                break;
            case '#':
                if (descriptor->pound) {
                    return false;
                }
                descriptor->pound = true;
                break;
            case '0':
                if (descriptor->left_pad_zero) {
                    return false;
                }
                descriptor->left_pad_zero = true;
                break;
            case '\0':
                return false;
            default:
                --(scanner->in_idx);
                return true;
        }
    }

    // should never reach here
    return false;
}

bool parse_width(Scanner *scanner, FormatDescriptor *descriptor, va_list args) {
    char c;

    while (true) {
        c = scanner_next_char(scanner);
        switch (c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                descriptor->width *= 10;
                descriptor->width += (size_t)(c - '0');
                break;
            case '*':
                if (descriptor->width > 0) {
                    return false;
                }
                descriptor->width = (size_t)va_arg(args, int);
                return true;
            default:
                --(scanner->in_idx);
                return true;
        }
    }

    // should never reach here
    return false;
}

bool parse_precision(Scanner *scanner, FormatDescriptor *descriptor, va_list args) {
    char c = scanner_current_char(scanner);

    if (c != '.') {
        return true;
    }
    ++(scanner->in_idx);

    while (true) {
        c = scanner_next_char(scanner);
        switch (c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                descriptor->precision *= 10;
                descriptor->precision += (size_t)(c - '0');
                break;
            case '*':
                if (descriptor->precision > 0) {
                    return false;
                }
                descriptor->precision = (size_t)va_arg(args, int);
                return true;
            default:
                --(scanner->in_idx);
                return true;
        }
    }

    // should never reach here
    return false;
}

void parse_length(Scanner *scanner, FormatDescriptor *descriptor) {
    char c = scanner_current_char(scanner);

    switch (c) {
        case 'h':
            descriptor->is_short = true;
            break;
        case 'l':
            descriptor->is_long = true;
            break;
        case 'L':
            descriptor->is_long_double = true;
            break;
        default:
            return;
    }

    ++(scanner->in_idx);
}

bool parse_type(Scanner *scanner, FormatDescriptor *descriptor, va_list args) {
    char c = scanner_current_char(scanner);

    switch (c) {
        case 'd':
        case 'i':
            print_integer(scanner, descriptor, va_arg(args, int));
            break;
        default:
            return false;
    }
    ++scanner->in_idx;
    return true;
}

bool format_str(Scanner *scanner, va_list args) {
    FormatDescriptor descriptor;
    char c;

    descriptor.left_justify = false;
    descriptor.force_positive_sign = false;
    descriptor.positive_space = false;
    descriptor.pound = false;
    descriptor.left_pad_zero = false;
    descriptor.width = 0;
    descriptor.precision = 0;
    descriptor.is_short = false;
    descriptor.is_long = false;
    descriptor.is_long_double = false;

    if (parse_percent(scanner)) {
        return true;
    }

    if (!parse_flags(scanner, &descriptor)) {
        return false;
    }

    if (!parse_width(scanner, &descriptor, args)) {
        return false;
    }

    if (!parse_precision(scanner, &descriptor, args)) {
        return false;
    }

    parse_length(scanner, &descriptor);

    if (!parse_type(scanner, &descriptor, args)) {
        return false;
    }

    return true;
}

EfiStatus log(const char *fmt, ...) {
    va_list args;
    Scanner scanner, scanner_backup;
    char c;

    scanner_init(&scanner);
    scanner_init(&scanner_backup);
    va_start(args, fmt);
    scanner.in_buf = fmt;
    alloc().alloc((uint8_t **)&scanner.out_buf, 0);

    while ((c = scanner_next_char(&scanner)) != '\0') {
        if (c == '%') {
            scanner_copy(&scanner, &scanner_backup);
            if (!format_str(&scanner, args)) {
                scanner_copy(&scanner_backup, &scanner);
                scanner_put_char(&scanner, '%');
            }
        } else {
            scanner_put_char(&scanner, c);
        }
    }

    scanner_put_char(&scanner, '\r');
    scanner_put_char(&scanner, '\n');
    scanner_put_char(&scanner, '\0');

    printer().output_string(scanner.out_buf);
    alloc().free((uint8_t *)scanner.out_buf);

    va_end(args);

    return EFI_SUCCESS;
}
