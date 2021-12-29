/*
 *  Kernel entry point.
 */

#include "Kernel/Arch/i686/IDT.h"
#include "Kernel/Arch/i686/VGA.h"

// kmain is called after the kernel bootstrap procedure is finished
extern "C" void kmain(void) {
    Terminal term = Terminal();

    term.writeString("Hello kernel");

    setupIDT();

    int i = 10 / 0;

    term.writeString("Set IDT.");

    while (true) {}
}
