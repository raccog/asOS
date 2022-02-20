#include <std/alloc.h>
#include <std/arg.h>
#include <std/int.h>
#include <std/io.h>
#include <std/log.h>

// stack allocated print buffer
char stack_print_buffer[256];

// print buffer
char *print_buffer;

void print_hex(Scanner *scanner, int value) {
    scanner_put_char(scanner, '0');
    scanner_put_char(scanner, 'x');

    for (int i = 7; i >= 0; --i) {
        u8 hex_val = (value >> (i * 4)) & 0xf;
        if (hex_val < 0xa) {
            scanner_put_char(scanner, '9' - (9 - hex_val));
        } else {
            scanner_put_char(scanner, 'f' - (0xf - hex_val));
        }
    }
}

void print_ptr(Scanner *scanner, long value) {
    scanner_put_char(scanner, '0');
    scanner_put_char(scanner, 'x');

    for (int i = 15; i >= 0; --i) {
        u8 hex_val = (value >> (i * 4)) & 0xf;
        if (hex_val < 0xa) {
            scanner_put_char(scanner, '9' - (9 - hex_val));
        } else {
            scanner_put_char(scanner, 'f' - (0xf - hex_val));
        }
    }
}

void print_int(Scanner *scanner, int value) {
    size_t digits = 1;
    int divisor;

    if (value < 0) {
        scanner_put_char(scanner, '-');
        value *= -1;
    }

    // count number of digits
    for (divisor = 10; divisor <= value; divisor *= 10) {
        ++digits;
    }
    divisor /= 10;

    while (digits > 0) {
        u8 digit = (value / divisor) % 10;
        scanner_put_char(scanner, '9' - (9 - digit));
        --digits;
        divisor /= 10;
    }
}

void print_str(Scanner *scanner, const char *str) {
    while (*str != '\0') {
        scanner_put_char(scanner, *str);
        ++str;
    }
}

void print_bits(Scanner *scanner, int value) {
    char c;

    scanner_put_char(scanner, '0');
    scanner_put_char(scanner, 'b');

    for (int i = 31; i >= 0; --i) {
        u8 bit = (value >> i) & 1;
        if (bit == 1) {
            c = '1';
        } else {
            c = '0';
        }
        scanner_put_char(scanner, c);
        if (i % 4 == 0) {
            scanner_put_char(scanner, ' ');
        }
    }
}

void simple_printf(const char *fmt, ...) {
    va_list args;
    Scanner scanner;
    char c;

    // init scanner and args
    scanner_init(&scanner);
    va_start(args, fmt);
    scanner.in_buf = fmt;
    scanner.out_buf = print_buffer;

    while ((c = scanner_next_char(&scanner)) != '\0') {
        if (c == '%') {
            c = scanner_next_char(&scanner);
            switch (c) {
                case 'x':
                    print_hex(&scanner, va_arg(args, int));
                    break;
                case 'i':
                case 'd':
                    print_int(&scanner, va_arg(args, int));
                    break;
                case 's':
                    print_str(&scanner, va_arg(args, const char *));
                    break;
                case 'c':
                    scanner_put_char(&scanner, (char)va_arg(args, int));
                    break;
                case 'p':
                    print_ptr(&scanner, va_arg(args, long));
                    break;
                case 'b':
                    print_bits(&scanner, va_arg(args, int));
                    break;
                default:
                    scanner_put_char(&scanner, '%');
                    scanner_put_char(&scanner, c);
            }
        } else if (c == '\n') {
            scanner_put_char(&scanner, '\r');
            scanner_put_char(&scanner, '\n');
        } else {
            scanner_put_char(&scanner, c);
        }
    }

    scanner_put_char(&scanner, '\0');

    // output string and free buffer
    printer().output_string(scanner.out_buf);
    alloc().free((u8 *)scanner.out_buf);

    va_end(args);
}

void alloc_print_buffer() {
    // allocate char buffer
    alloc().alloc((u8 **)&print_buffer, 1024);
}

void stack_alloc_print_buffer() {
    print_buffer = &stack_print_buffer[0];
}
