#ifndef STD_IO_H
#define STD_IO_H

typedef void (*OutputStringFunc)(const char *);

// printer contains function pointer to print a string
typedef struct {
    OutputStringFunc output_string;
} Printer;

// get global printer
Printer printer();

// set global printer
void init_printer(Printer printer);

#endif
