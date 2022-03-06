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

// efi output string function
//
// This is to be used as the global Printer's string output function
// (see printer.h in stdlib for more details)
void efi_output_string(const char *str);

// Set the buffer used to print out strings.
// 
// This buffer should be allocated before this function is called.
void efi_set_char16_buf(EfiChar16 *buf);

#endif