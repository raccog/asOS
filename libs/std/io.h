#ifndef IO_H
#define IO_H

typedef void (*OutputStringFunc)(const char *);

typedef struct {
    OutputStringFunc output_string;
} Printer;

Printer printer();

void init_printer(Printer printer);

#endif
