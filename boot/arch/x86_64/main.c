#include <efi.h>

EfiStatus efi_main(EfiHandle handle, EfiSystemTable *system_table) {
    system_table->console_out->clear_screen(system_table->console_out);
    system_table->console_out->output_string(system_table->console_out, L"Hello World!!! :D\r\n");
    return EFI_SUCCESS;
}
