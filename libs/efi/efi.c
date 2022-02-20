#include <efi/efi.h>

#include <std/alloc.h>

// global system table
EfiSystemTable *system_table;

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

EfiStatus efi_page_alloc(EfiPhysicalAddress *buf, size_t num_pages, EfiAllocateType alloc_type) {
    EfiStatus status = system_table->boot_services->allocate_pages(alloc_type, EfiLoaderData, num_pages, buf);
    if (status != EFI_SUCCESS) {
        system_table->console_out->output_string(system_table->console_out, L"Error durring memory allocation\r\n");
        return status;
    }

    return EFI_SUCCESS;
}

EfiStatus efi_page_free(EfiPhysicalAddress buf) {
    EfiStatus status = system_table->boot_services->free_pages(buf, 1);
    if (status != EFI_SUCCESS) {
        system_table->console_out->output_string(system_table->console_out, L"Error durring memory free\r\n");
        return status;
    }

    return EFI_SUCCESS;
}

EfiStatus efi_print_str16(EfiChar16 *str) {
    EfiStatus status = system_table->console_out->output_string(system_table->console_out, str);
    if (status != EFI_SUCCESS) {
        system_table->console_out->output_string(system_table->console_out, L"Error printing buffer\r\n");
        return status;
    }

    return EFI_SUCCESS;
}
