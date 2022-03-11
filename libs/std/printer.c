#include <std/printer.h>

// memory for global printer
static Printer _printer;

Printer printer() {
    return _printer;
}

void init_printer(Printer printer) {
    _printer = printer;
}
