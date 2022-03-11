#include <std/alloc.h>
#include <std/scanner.h>
#include <std/printer.h>
#include <std/printf.h>

// print buffer
char *print_buffer;

// print a 32bit unsigned integer as hexadecimal
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

// print a 64bit address pointer
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

// print a 32bit signed integer
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

// print a null-terminated string
void print_str(Scanner *scanner, const char *str) {
    while (*str != '\0') {
        scanner_put_char(scanner, *str);
        ++str;
    }
}

// print a 32bit unsigned integer as a bit string
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
            // format an argument
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
            // output a new line
            scanner_put_char(&scanner, '\r');
            scanner_put_char(&scanner, '\n');
        } else {
            // output a non-formatted character
            scanner_put_char(&scanner, c);
        }
    }

    // null-terminate string
    scanner_put_char(&scanner, '\0');

    // output string and free buffer
    printer().puts(scanner.out_buf);
    alloc().free((u8 *)scanner.out_buf);

    va_end(args);
}

void set_print_buffer(char *buf) {
    print_buffer = buf;
}

