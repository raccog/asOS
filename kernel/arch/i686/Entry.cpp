#include "kernel/arch/i686/VGA.h"

extern "C" void kmain(void) {
    Terminal term = Terminal();

    term.writeString("Hello kernel");
}
