/*
 *  Kernel entry point.
 */

#include "kernel/arch/i686/VGA.h"

// kmain is called after the kernel bootstrap procedure is finished
extern "C" void kmain(void) {
    Terminal term = Terminal();

    term.writeString("Hello kernel");
}
