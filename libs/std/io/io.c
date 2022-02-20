#include <std/io.h>

// memory for global printer
Printer _printer;

Printer printer() {
    return _printer;
}

void init_printer(Printer printer) {
    _printer = printer;
}

void disable_printer() {
    _printer.output_string = 0;
}

bool printer_is_disabled() {
    return (_printer.output_string == 0);
}
