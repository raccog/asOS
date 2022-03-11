#ifndef BOOT_EFI_H
#define BOOT_EFI_H

#include <hw/efi/efi.h>

// Initialize global efi system table
//
// Also initializes global allocator functions and allocates a chunk of
// pages to be used for dynamic heap allocation.
EfiStatus efi_init(EfiSystemTable *st);

// Frees chunk of pages used to allocate heap memory
EfiStatus efi_exit();

// Prints a string to the efi console
// 
// This is to be used as the global Printer's string output function
// (see printer.h in stdlib for more details)
void efi_puts(const char *str);

// Prints a character to the efi console
// This is to be used as the gloval Printer's putc function
// (see printer.h in stdlib for more details)
void efi_putc(char c);

#endif