#include <efi/efi.h>

void __chkstk(void) {
    return;
}

EfiStatus efi_main(EfiHandle handle, EfiSystemTable *system_table) {
    EfiMemoryDescriptor memory_map[0xff];
    uint64_t memory_map_size = sizeof(EfiMemoryDescriptor) * 0xff;
    uint64_t map_key, descriptor_size;
    uint32_t descriptor_version;
    system_table->boot_services->get_memory_map(&memory_map_size, &memory_map[0], &map_key, &descriptor_size, &descriptor_version);

    system_table->console_out->clear_screen(system_table->console_out);
    system_table->console_out->output_string(system_table->console_out, L"Hello World!!! :D\r\n");
    return EFI_SUCCESS;
}
