/*
 *  Kernel entry procedure
 *
 *  Derived from https://wiki.osdev.org/QEMU_AArch64_Virt_Bare_Bones
 *
 */

#include <stdint.h>

volatile uint8_t *uart = (uint8_t *) 0x09000000;

extern "C" void putchar(char c) {
    *uart = c;
}

extern "C" void kprint(const char *s) {
    while (*s != '\0') {
        putchar(*s);
        s++;
    }
}

extern "C" void kmain() {
    kprint("Hello world\n");
}
