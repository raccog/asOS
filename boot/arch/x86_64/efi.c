#include "efi.h"

#include <std/alloc.h>
#include <std/log.h>

// global system table
EfiSystemTable *system_table;

EfiChar16 *char16_buf;

// efi allocator functions
u8 *efi_buffer;
Allocator efi_allocator;

bool efi_alloc(u8 **ptr, size_t bytes) {
    *ptr = efi_buffer;
    efi_buffer += bytes;
    return true;
}

void efi_free(u8 *_) {
    return;
}

void efi_set_char16_buf(EfiChar16 *buf) {
    char16_buf = buf;
}

// convert char8 string to char16 string
//
// returned string should be freed when not in use
EfiChar16 *char8_to_char16(const char *buf) {
    const char *tmp_buf = buf;
    size_t count = 0;

    // count characters in string
    do {
        ++count;
    } while (*(buf++) != '\0');
    buf = tmp_buf;

    // copy characters to char16 string
    for (size_t i = 0; i < count; ++i) {
        char16_buf[i] = (EfiChar16)buf[i];
    }

    return char16_buf;
}

EfiStatus efi_print_str16(EfiChar16 *str) {
    EfiStatus status = system_table->console_out->output_string(system_table->console_out, str);
    if (status != EFI_SUCCESS) {
        return status;
    }

    return EFI_SUCCESS;
}

void efi_output_string(const char *str) {
    EfiChar16 *buf16 = char8_to_char16(str);

    efi_print_str16(buf16);
    alloc().free((u8 *)buf16);
}

EfiStatus efi_page_alloc(EfiPhysicalAddress *buf, size_t num_pages, EfiAllocateType alloc_type) {
    EfiStatus status = system_table->boot_services->allocate_pages(alloc_type, EfiLoaderData, num_pages, buf);
    if (status != EFI_SUCCESS) {
        simple_log("Error during UEFI page allocation. Code: %i", status);
        return status;
    }

    return EFI_SUCCESS;
}

EfiStatus efi_page_free(EfiPhysicalAddress buf) {
    EfiStatus status = system_table->boot_services->free_pages(buf, 1);
    if (status != EFI_SUCCESS) {
        simple_log("Error during UEFI page free. Code: %i", status);
        return status;
    }

    return EFI_SUCCESS;
}

EfiStatus efi_init(EfiSystemTable *st) {
    // set global system table
    system_table = st;

    // set efi allocator functions
    efi_allocator.alloc = &efi_alloc;
    efi_allocator.free = &efi_free;
    init_alloc(efi_allocator);

    // allocate pages for efi buffer
    return efi_page_alloc((EfiPhysicalAddress *)&efi_buffer, 8, AllocateAnyPages); // 8 pages
}

// needed for EFI application?
void __chkstk(void) {
    return;
}