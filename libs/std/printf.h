#ifndef STD_PRINTF_H
#define STD_PRINTF_H

// Format variables into a string and print out the formatted string
void simple_printf(const char *fmt, ...);

// Sets the print buffer
//
// This buffer should already be allocated.
void set_print_buffer(char *buf);

#endif
