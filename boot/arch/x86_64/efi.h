#ifndef BOOT_EFI_H
#define BOOT_EFI_H

#include <hw/efi/efi.h>

// init global system table
EfiStatus efi_init(EfiSystemTable *st);

// efi page allocation function
EfiStatus efi_page_alloc(EfiPhysicalAddress *buf, size_t num_pages, EfiAllocateType alloc_type);

// efi page free function
EfiStatus efi_page_free(EfiPhysicalAddress buf);

// efi print function
EfiStatus efi_print_str16(EfiChar16 *str);

// efi output string function
void efi_output_string(const char *str);

void efi_set_char16_buf(EfiChar16 *buf);

#endif