#include <std/alloc.h>
#include <std/arg.h>
#include <std/int.h>
#include <std/io.h>
#include <std/log.h>

// properties of a format string
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

    bool capital_letters;
} FormatDescriptor;

// pads scanner with n characters
void pad_scanner(Scanner *scanner, char c, size_t n) {
    while (n > 0) {
        scanner_put_char(scanner, c);
        --n;
    }
}

// converts an integer's digits into characters and places them into a scanner's output buffer
//
// todo: add short and long range types
void print_integer(Scanner *scanner, FormatDescriptor *descriptor, int value) {
    int divisor = 1;
    size_t digit_count = 0;
    bool is_negative = false;
    int extra_digit = 0;
    int padding;

    // check for negative integer
    if (value < 0) {
        is_negative = true;
        value *= -1;
    }

    // check for extra sign digit
    if (is_negative || descriptor->force_positive_sign || descriptor->positive_space) {
        ++extra_digit;
    }

    // count the number of digits and get the maximum divisor that is a power of 10
    do {
        divisor *= 10;
        ++digit_count;
    } while (value / divisor > 0);
    divisor /= 10;

    // pad the buffer with spaces
    if (!descriptor->left_justify && descriptor->width > 0) {
        if ((descriptor->width > descriptor->precision + extra_digit) &&
                (!descriptor->left_pad_zero && descriptor->precision > 0)) {
            padding = (descriptor->precision > digit_count) ? descriptor->precision : digit_count;
            padding = descriptor->width - padding;
            padding -= extra_digit;
            pad_scanner(scanner, ' ', padding);
        }
    }

    // place sign if needed
    if (is_negative) {
        scanner_put_char(scanner, '-');
    } else if (descriptor->force_positive_sign) {
        scanner_put_char(scanner, '+');
    } else if (descriptor->positive_space) {
        scanner_put_char(scanner, ' ');
    }

    // pad the buffer with zeros
    if (descriptor->precision > 0) {
        pad_scanner(scanner, '0', descriptor->precision - digit_count);
    }
    if (descriptor->width > 0 && descriptor->precision == 0 && descriptor->left_pad_zero) {
        pad_scanner(scanner, '0', descriptor->width - digit_count - extra_digit);
    }
    
    // place digits in buffer
    for (size_t i = 0; i < digit_count; ++i) {
        scanner_put_char(scanner, '9' - (char)(9 - value / divisor));
        value = value % divisor;
        divisor /= 10;
    }

    // pad the right side of the buffer with spaces
    if (descriptor->left_justify && descriptor->width > 0 &&
            !(descriptor->left_pad_zero && descriptor->precision == 0)) {
        padding = (descriptor->precision > digit_count) ? descriptor->precision : digit_count;
        padding += extra_digit;
        if (descriptor->width > padding) {
            padding = descriptor->width - padding;
            pad_scanner(scanner, ' ', padding);
        }
    }
}

// convert 4-bit number to a hex character
char to_hex_char(FormatDescriptor *descriptor, u8 value) {
    char c;

    // digits from 0-9
    if (value < 0xa) {
        c = '9';
        c -= (char)(0x9 - value);
        return c;
    }

    // digits from a-f
    if (descriptor->capital_letters) {
        c = 'F';
    } else {
        c = 'f';
    }
    c -= (char)(0xf - value);
    return c;
}

// print an unsigned, 32-bit integer in hex format
void print_hex(Scanner *scanner, FormatDescriptor *descriptor, int value) {
    char c;
    u8 hex_val;
    bool is_first = true;
    size_t char_count = 0, hex_count = 0;

    // count the number of characters that will be displayed
    // this is used for padding
    if (descriptor->pound) {
        char_count += 2;
    }
    // count number of leading zeros
    for (int i = 7; i >= 0; --i) {
        if (((value >> (i * 4)) & 0xf) == 0) {
            continue;
        }
        hex_count = i + 1;
        break;
    }
    // compare leading zeros to precision
    if (descriptor->precision > hex_count) {
        hex_count = descriptor->precision;
    }
    char_count += hex_count;

    // pad if needed
    if (descriptor->width > char_count) {
        for (size_t i = 0; i < descriptor->width - char_count; ++i) {
            scanner_put_char(scanner, ' ');
        }
    }

    // add '0x' or '0X' if needed
    if (descriptor->pound) {
        scanner_put_char(scanner, '0');
        if (descriptor->capital_letters) {
            c = 'X';
        } else {
            c = 'x';
        }
        scanner_put_char(scanner, c);
    }

    // add extra zeros if needed
    for (size_t i = 8; i < descriptor->precision; ++i) {
        scanner_put_char(scanner, '0');
    }

    // print each hex value
    for (int i = 7; i >= 0; --i) {
        // get value of current hex digit
        hex_val = (value >> (4 * i)) & 0xf;

        // skip unneeded zeros
        if (i + 1 > descriptor->precision && i != 0 && hex_val == 0 && is_first) {
            continue;
        }

        // convert and print
        c = to_hex_char(descriptor, hex_val);
        scanner_put_char(scanner, c);
        is_first = false;
    }
}

// check for a percent sign
bool parse_percent(Scanner *scanner) {
    char c = scanner_current_char(scanner);

    if (c == '%') {
        scanner_put_char(scanner, c);
        ++(scanner->in_idx);
        return true;
    }
    return false;
}

// check for formatting flags
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

// check for padding width
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

// check for digit precision
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

// check length
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

// check type
bool parse_type(Scanner *scanner, FormatDescriptor *descriptor, va_list args) {
    char c = scanner_current_char(scanner);

    switch (c) {
        case 'd':
        case 'i':
            print_integer(scanner, descriptor, va_arg(args, int));
            break;
        case 'X':
            descriptor->capital_letters = true;
            print_hex(scanner, descriptor, va_arg(args, int));
            break;
        case 'x':
            descriptor->capital_letters = false;
            print_hex(scanner, descriptor, va_arg(args, int));
            break;
        default:
            return false;
    }
    ++scanner->in_idx;
    return true;
}

// format args into a scanner's buffer
bool format_str(Scanner *scanner, va_list args) {
    FormatDescriptor descriptor;
    char c;

    // init format descriptor
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
    descriptor.capital_letters = false;

    // parse each part of the format string

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

void log(const char *fmt, ...) {
    va_list args;
    Scanner scanner, scanner_backup;
    char c;

    // init scanners and args
    scanner_init(&scanner);
    scanner_init(&scanner_backup);
    va_start(args, fmt);
    scanner.in_buf = fmt;

    // allocate char buffer
    alloc().alloc((u8 **)&scanner.out_buf, 0);

    // loop through all characters in string
    while ((c = scanner_next_char(&scanner)) != '\0') {
        // parse format string and place argument
        if (c == '%') {
            scanner_copy(&scanner, &scanner_backup);
            if (!format_str(&scanner, args)) {
                // restore scanner from backup if formatting failed
                scanner_copy(&scanner_backup, &scanner);
                scanner_put_char(&scanner, '%');
            }
        } else {
            // place normal character
            scanner_put_char(&scanner, c);
        }
    }

    // go to next line and place null terminator
    scanner_put_char(&scanner, '\r');
    scanner_put_char(&scanner, '\n');
    scanner_put_char(&scanner, '\0');

    // output string and free buffer
    printer().output_string(scanner.out_buf);
    alloc().free((u8 *)scanner.out_buf);

    va_end(args);
}
