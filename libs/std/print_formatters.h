#ifndef STD_PRINT_FORMATTERS_H
#define STD_PRINT_FORMATTERS_H

// formats and prints out a signed integer (32bit) value
void print_int$(i32 value);

// formats and prints out an unsigned integer (32bit) value
void print_int_unsigned$(u32 value);

// formats and prints out a signed long (64bit) value
void print_long$(i64 value);

// formats and prints out an unsigned long (64bit) value
void print_long_unsigned$(u64 value);

// formats and prints out a 32-bit hex value
void print_hex32$(u32 value);

// formats and prints out a 64-bit hex value
void print_hex64$(u64 value);

#endif