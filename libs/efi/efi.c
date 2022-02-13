#include <efi/efi.h>

// global system table
EfiSystemTable *system_table;

void efi_init(EfiSystemTable *st) {
    system_table = st;
}

EfiStatus efi_alloc(EfiPhysicalAddress *buf) {
    EfiStatus status = system_table->boot_services->allocate_pages(AllocateAnyPages, EfiLoaderData, 1, buf);
    if (status != EFI_SUCCESS) {
        system_table->console_out->output_string(system_table->console_out, L"Error durring memory allocation\r\n");
        return status;
    }

    return EFI_SUCCESS;
}

EfiStatus efi_free(EfiPhysicalAddress buf) {
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
