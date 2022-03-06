#ifndef BOOT_EFI_H
#define BOOT_EFI_H

#include <hw/efi/efi.h>

// init global system table
EfiStatus efi_init(EfiSystemTable *st);

// efi output string function
void efi_output_string(const char *str);

void efi_set_char16_buf(EfiChar16 *buf);

#endif