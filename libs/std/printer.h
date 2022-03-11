#ifndef STD_PRINTER_H
#define STD_PRINTER_H

typedef void (*OutputCharFunc)(char c);
typedef void (*OutputStringFunc)(const char *);

// Printer contains function pointer to print a string
typedef struct {
    OutputCharFunc putc;
    OutputStringFunc puts;
} Printer;

// Get global printer
Printer printer();

// Set global printer
void init_printer(Printer printer);

#endif
