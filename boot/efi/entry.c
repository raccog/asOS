#include "efi.h"

#include <hw/mmap.h>
#include <std/alloc.h>
#include <std/std.h>
#include <std/log.h>
#include <std/printer.h>

EfiStatus status;
EfiMemoryMap efi_mmap;
size_t efi_mmap_size = sizeof(EfiMemoryDescriptor) * 0xff;

// Initialize efi system table and allocate print buffers
EfiStatus init(EfiSystemTable *st) {
    // printer contains a pointer to the efi string output function
    Printer printer;

    // buffer pointers
    char *print_buf;
    EfiChar16 *efi_print_buf;

    // stack allocated print buffers
    char stack_print_buf[256];
    EfiChar16 efi_stack_print_buf[512];

    // init printer
    printer.output_string = &efi_output_string;
    init_printer(printer);

    // temporarily allocate print buffers using the stack
    set_print_buffer(&stack_print_buf[0]);
    efi_set_char16_buf(&efi_stack_print_buf[0]);

    // init EFI system table
    if ((status = efi_init(st)) != EfiSuccess) {
        return status;
    }

    // allocate print buffer
    alloc().alloc((u8 **)&print_buf, 1024);
    set_print_buffer(print_buf);

    // allocate efi print buffer
    alloc().alloc((u8 **)&efi_print_buf, 2048);
    efi_set_char16_buf(efi_print_buf);

    // allocate buffer for memory map
    alloc().alloc((u8 **)&efi_mmap.descriptors, efi_mmap_size);

    simple_log("Bootloader init successful");

    return EfiSuccess;
}

// The main entry point for the UEFI bootloader.
EfiStatus EFIAPI efi_main(EfiHandle handle, EfiSystemTable *st) {
    // OS memory map
    OSMemoryMap os_mmap;

    // init efi and allocate printer buffers
    if ((status = init(st)) != EfiSuccess) {
        return status;
    }

    // disable watchdog timer
    // this prevents the system from resetting after a certain time interval
    st->boot_services->set_watchdog_timer(0, 0, 0, 0);

    // get efi memory map
    status = st->boot_services->get_memory_map(&efi_mmap_size, efi_mmap.descriptors, &efi_mmap.key, &efi_mmap.descriptor_size, &efi_mmap.version);
    efi_mmap.entries = efi_mmap_size / efi_mmap.descriptor_size;
    if (status != EfiSuccess) {
        return status;
    }

    // convert efi mmap to OS mmap and print it
    to_os_mmap(&efi_mmap, &os_mmap);
    output_mmap(&os_mmap);

    // test log function
    simple_log("Hello kernel!");

    // infinite loop will be replaced with calling the OS entry point
    while (true) {}

    // free chunk of pages used for heap allocation
    efi_exit();

    // bootloader should never return
    return EfiLoadError;

}
