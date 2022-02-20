#ifndef STD_IO_H
#define STD_IO_H

#include <std/bool.h>

typedef void (*OutputStringFunc)(const char *);

// printer contains function pointer to print a string
typedef struct {
    OutputStringFunc output_string;
} Printer;

// get global printer
Printer printer();

// set global printer
void init_printer(Printer printer);

// sets printer function pointers to null
void disable_printer();

// returns true if the printer function pointers are null
bool printer_is_disabled();

#endif
