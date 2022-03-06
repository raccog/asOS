#include "efi.h"

#include <std/alloc.h>
#include <std/log.h>

#define BOOTLOADER_PAGES 8

// global system table
EfiSystemTable *system_table;

// buffer to use for string conversion (from 8bit char to 16bit char) and output to efi console
EfiChar16 *char16_buf;

// buffer to use for heap allocation
u8 *efi_heap_buffer_start;
u8 *efi_heap_buffer;
u64 allocated_bytes;    // counter used to panic when heap has overflowed

// This allocator contains pointers to efi allocation functions.
// These allocation functions are to be used only in the UEFI bootloader
Allocator efi_allocator;

// The most simple heap allocation function.
//
// A chunk of memory (currently 8 pages) is allocated using the UEFI page allocator when the
// bootloader first starts. A pointer to this chunk of allocated memory is used to give out
// dynamically allocated buffers when this function is called.
//
// NOTES:
// As the pointer to this page buffer increases linearly, it will eventually overflow. Using
// this method, memory cannot be freed, so the efi_free function just simply returns.
// 
// Also, memory allocated with this function is not aligned, so using this heap memory might
// be slower than aligned memory.
bool efi_alloc(u8 **ptr, size_t bytes) {
    if (allocated_bytes + bytes > (BOOTLOADER_PAGES << 22)) {
        simple_log("CRITICAL ERROR: Heap overflow in bootloader");
        return false;
    }
    allocated_bytes += bytes;
    *ptr = efi_heap_buffer;
    efi_heap_buffer += bytes;
    return true;
}

// The most simple heap free function.
//
// Currently does nothing :)
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

// output string (with 16bit chars) to efi console
void efi_print_str16(EfiChar16 *str) {
    // EfiStatus status = 
    system_table->console_out->output_string(system_table->console_out, str);
    /* TODO: panic bootloader on failure
    if (status != EFI_SUCCESS) {
        return status;
    }
    */

    // return EFI_SUCCESS;
}

void efi_output_string(const char *str) {
    EfiChar16 *buf16 = char8_to_char16(str);

    efi_print_str16(buf16);
}

// Uses UEFI page allocator to allocate a number of pages.
EfiStatus efi_page_alloc(EfiPhysicalAddress *buf, size_t num_pages, EfiAllocateType alloc_type) {
    EfiStatus status = system_table->boot_services->allocate_pages(alloc_type, EfiLoaderData, num_pages, buf);
    if (status != EFI_SUCCESS) {
        simple_log("Error during UEFI page allocation. Code: %i", status);
        return status;
    }

    return EFI_SUCCESS;
}

// Uses UEFI page allocator free to free pages.
EfiStatus efi_page_free(EfiPhysicalAddress buf, size_t num_pages) {
    EfiStatus status = system_table->boot_services->free_pages(buf, num_pages);
    if (status != EFI_SUCCESS) {
        simple_log("Error during UEFI page free. Code: %i", status);
        return status;
    }

    return EFI_SUCCESS;
}

EfiStatus efi_init(EfiSystemTable *st) {
    EfiStatus status;

    // set global system table
    system_table = st;

    // set efi allocator functions
    efi_allocator.alloc = &efi_alloc;
    efi_allocator.free = &efi_free;
    init_alloc(efi_allocator);
    allocated_bytes = 0;

    // allocate pages for efi buffer
    if ((status = efi_page_alloc((EfiPhysicalAddress *)&efi_heap_buffer, BOOTLOADER_PAGES, AllocateAnyPages)) != EFI_SUCCESS) {
        return status;
    }

    // save page buffer starting address to free later
    efi_heap_buffer_start = efi_heap_buffer;

    return EFI_SUCCESS;
}

EfiStatus efi_exit() {
    // free page buffer
    efi_page_free(efi_heap_buffer_start, BOOTLOADER_PAGES);
}

// needed for EFI application?
void __chkstk(void) {
    return;
}