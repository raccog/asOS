#include <std/alloc.h>
#include <std/scanner.h>
#include <std/printer.h>
#include <std/print_formatters.h>
#include <std/printf.h>

// print buffer
static char *print_buffer;

// print a 32bit unsigned integer as hexadecimal
static void print_hex(Scanner *scanner, int value) {
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
static void print_ptr(Scanner *scanner, long value) {
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
static void print_int(Scanner *scanner, int value) {
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
static void print_str(Scanner *scanner, const char *str) {
    while (*str != '\0') {
        scanner_put_char(scanner, *str);
        ++str;
    }
}

// print a 32bit unsigned integer as a bit string
static void print_bits(Scanner *scanner, int value) {
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

void simple_printf$(const char *fmt, ...) {
    va_list args;
    char c;

    va_start(args, fmt);

    while ((c = *fmt++) != '\0') {
        if (c == '%') {
            c = *fmt++;
            switch (c) {
                case 'c':
                    printer().putc((char)va_arg(args, i32));
                    break;
                case 'i':
                case 'd':
                    if (*fmt == 'l') {
                        ++fmt;
                        print_long$(va_arg(args, i64));
                    } else {
                        print_int$(va_arg(args, i32));
                    }
                    break;
                case 's':
                    printer().puts(va_arg(args, const char *));
                    break;
                case 'u':
                    if (*fmt == 'l') {
                        ++fmt;
                        print_long_unsigned$(va_arg(args, u64));
                    } else {
                        print_int_unsigned$(va_arg(args, u32));
                    }
                    break;
                case 'x':
                    if (*fmt == 'l') {
                        ++fmt;
                        print_hex64$(va_arg(args, u64));
                    } else {
                        print_hex32$(va_arg(args, u32));
                    }
                    break;
                case 'p':
                    print_hex64$((u64)va_arg(args, void *));
                    break;
                case '%':
                default:
                    printer().putc(c);
            }
        } else {
            printer().putc(c);
        }
    }

    printer().putc('\n');   // TODO: remove this line. just for testing

    va_end(args);
}

static void set_print_buffer(char *buf) {
    print_buffer = buf;
}

