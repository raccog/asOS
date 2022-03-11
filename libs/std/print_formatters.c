#include <std/print_formatters.h>

#include <std/printer.h>

// Int buffer size contains room for:
//      10 digits
//      A negative sign
//      The null terminator
#define IntBufferSize   12

// Long buffer size contains room for:
//      20 digits
//      A negative sign
//      The null terminator
#define LongBufferSize   22

typedef enum {
    Bits8,
    Bits16,
    Bits32,
    Bits64
} ValueBits;

static void print_zero() {
    printer().putc('0');
}

static void print_negative_sign() {
    printer().putc('-');
}

static void print_digits(u64 value, ValueBits bits, size_t buf_size) {
    // Buffer that contains the final string representation
    char buf[buf_size];
    size_t buf_idx = 0;

    // Assert that number of bits is valid
    // Also set starting divisor
    u64 divisor;
    switch (bits) {
        case Bits8:
            divisor = 100ul;
            break;
        case Bits16:
            divisor = 10000ul;
            break;
        case Bits32:
            divisor = 1000000000ul;
            break;
        case Bits64:
            divisor = 10000000000000000000ul;
            break;
        default:
            simple_log("Bits (%i) is not valid", bits);
            return;
    }

    // Print each digit
    while (divisor >= 1) {
        // continue until there is a digit to print out
        if (divisor > value) {
            divisor /= 10;
            continue;
        }
        
        // isolate the digit
        u8 digit = (value / divisor) % 10;

        // convert digit to char and place into the buffer
        buf[buf_idx++] = '0' + digit;

        // break loop if at the end
        if (divisor == 1) {
            break;
        } else {
            divisor /= 10;
        }
    }

    // add null terminator
    buf[buf_idx] = '\0';

    // print to screen
    printer().puts(buf);
}

static void print_number_unsigned(u64 value, ValueBits bits, size_t buf_size) {
    // If value is zero, print and return
    if (value == 0) {
        print_zero();
        return;
    }

    print_digits(value, bits, buf_size);
}

static void print_number_signed(i64 value, ValueBits bits, size_t buf_size) {
    // If value is zero, print and return
    if (value == 0) {
        print_zero();
        return;
    // Add negative sign if value is less than 0
    } else if (value < 0) {
        print_negative_sign();
        value *= -1;
    }

    print_digits((u64)value, bits, buf_size);
}

static void print_hex(u64 value, ValueBits bits) {
    // Print out hex prefix
    printer().putc('0');
    printer().putc('x');

    // Decide how many hex digits to print
    int digits;
    switch (bits) {
        case Bits8:
            digits = 2;
            break;
        case Bits16:
            digits = 4;
            break;
        case Bits32:
            digits = 8;
            break;
        case Bits64:
            digits = 16;
            break;
        default:
            simple_log("Bits (%i) is not valid", bits);
            return;
    }

    // Buffer that contains the final string representation
    char buf[digits + 1];
    size_t buf_idx = 0;

    // Print out each hex digit
    for (int i = digits - 1; i >= 0; --i) {
        u8 hex_val = (value >> (i * 4)) & 0xf;
        if (hex_val < 0xa) {
            buf[buf_idx++] = '9' - (9 - hex_val);
        } else {
            buf[buf_idx++] = 'f' - (0xf - hex_val);
        }
    }

    // add null terminator
    buf[buf_idx] = '\0';

    // print to screen
    printer().puts(buf);
}

void print_int$(i32 value) {
    print_number_signed(value, Bits32, IntBufferSize);
}

void print_long$(i64 value) {
    print_number_signed(value, Bits64, LongBufferSize);
}

void print_int_unsigned$(u32 value) {
    print_number_unsigned(value, Bits32, IntBufferSize);
}

void print_long_unsigned$(u64 value) {
    print_number_unsigned(value, Bits64, LongBufferSize);
}

void print_hex32$(u32 value) {
    print_hex(value, Bits32);
}

void print_hex64$(u64 value) {
    print_hex(value, Bits64);
}