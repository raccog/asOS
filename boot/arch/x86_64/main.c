#include "efi.h"

#include <hw/mmap.h>
#include <std/alloc.h>
#include <std/std.h>
#include <std/log.h>
#include <std/printer.h>

EfiStatus status;
EfiMemoryMap efi_mmap;
size_t efi_mmap_size = sizeof(EfiMemoryDescriptor) * 0xff;

EfiStatus init(EfiSystemTable *st) {
    Printer printer;
    EfiChar16 *char16_buf;
    char *print_buffer;

    // stack allocated print buffers
    char stack_print_buffer[256];
    EfiChar16 stack_char16_buf[512];

    // init printer
    printer.output_string = &efi_output_string;
    init_printer(printer);

    // temporarily allocate print buffers using the stack
    set_print_buffer(&stack_print_buffer[0]);
    efi_set_char16_buf(&stack_char16_buf[0]);

    // init EFI system table
    if ((status = efi_init(st)) != EFI_SUCCESS) {
        return status;
    }

    // allocate buffers
    alloc().alloc((u8 **)&print_buffer, 1024);
    set_print_buffer(print_buffer);
    alloc().alloc((u8 **)&char16_buf, 2048);
    efi_set_char16_buf(char16_buf);
    alloc().alloc((u8 **)&efi_mmap.descriptors, efi_mmap_size);

    simple_log("Bootloader init successful");

    return EFI_SUCCESS;
}

EfiStatus efi_main(EfiHandle handle, EfiSystemTable *st) {
    OSMemoryMap os_mmap;

    // init efi and allocate printer buffer
    if ((status = init(st)) != EFI_SUCCESS) {
        return status;
    }

    // disable watchdog timer
    st->boot_services->set_watchdog_timer(0, 0, 0, 0);

    // get memory map
    status = st->boot_services->get_memory_map(&efi_mmap_size, efi_mmap.descriptors, &efi_mmap.key, &efi_mmap.descriptor_size, &efi_mmap.version);
    efi_mmap.entries = efi_mmap_size / efi_mmap.descriptor_size;
    if (status != EFI_SUCCESS) {
        return status;
    }

    // output memory map
    to_os_mmap(&efi_mmap, &os_mmap);
    output_mmap(&os_mmap);

    // test log function
    simple_log("Hello kernel!");

    while (true) {}

    return EFI_SUCCESS;

}
