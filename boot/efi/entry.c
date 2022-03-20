#include "efi.h"

#include <hw/mmap.h>
#include <std/alloc.h>
#include <std/printer.h>
#include <std/print_formatters.h>

#include <limits.h>

EfiStatus status;
EfiMemoryMap efi_mmap;
size_t efi_mmap_size = sizeof(EfiMemoryDescriptor) * 0xff;

// Initialize efi system table and allocate print buffers
EfiStatus init(EfiSystemTable *st) {
    // printer contains a pointer to the efi string output function
    Printer printer;

    // buffer pointers
    char *print_buf;

    // stack allocated print buffers
    char stack_print_buf[256];

    // init printer
    printer.putc = &efi_putc;
    printer.puts = &efi_puts;

    init_printer(printer);

    // temporarily allocate print buffers using the stack
    set_print_buffer(&stack_print_buf[0]);

    // init EFI system table
    if ((status = efi_init(st)) != EfiSuccess) {
        return status;
    }

    // allocate print buffer
    alloc().alloc((u8 **)&print_buf, 1024);
    set_print_buffer(print_buf);

    // allocate buffer for memory map
    alloc().alloc((u8 **)&efi_mmap.descriptors, efi_mmap_size);

    simple_log("Bootloader init successful");

    return EfiSuccess;
}

// The main entry point for the UEFI bootloader.
EfiStatus EfiApi efi_main(EfiHandle image_handle, EfiSystemTable *st) {
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
    simple_log("Hello UEFI bootloader!");

    // print address of bootloader entry
    simple_log("Bootloader entry at: %x", &efi_main);

    // print out system table header
    simple_log("Efi Table Signature: %p", st->header.signature);
    simple_log("Efi Table Revision: %x", st->header.revision);
    simple_log("Efi Table Header Size: %x", st->header.header_size);
    simple_log("Efi Table CRC32: %x", st->header.crc32);

    // setup for getting bootloader image protocol
    EfiLoadedImageProtocol *loaded_image;
    EfiGuid guid = EfiLoadedImageProtocolGuid;

    // get image protocol
    status = st->boot_services->open_protocol(image_handle, &guid, (void **)&loaded_image, image_handle, 0, EfiOpenProtocolGetProtocol);
    if (status != EfiSuccess) {
        simple_log("Failed to open loaded image protocol with error code %i", status);
        return status;
    }

    // print image information
    simple_log("Image base: %x", loaded_image->image_base);
    simple_log("Image size: %x", loaded_image->image_size);

    // test simple_printf
    simple_log("Testing simple_printf$() ...");
    simple_printf$("Should be 0: %i", 0);
    simple_printf$("Should be 10: %i", 10);
    simple_printf$("Should be 7: %d", 7);
    simple_printf$("Should be 5827462: %i", 5827462);
    simple_printf$("Should be 1234567890: %i", 1234567890);
    simple_printf$("Should be -1234567890: %i", -1234567890);
    simple_printf$("Should be 10000000000: %il", 10000000000l);
    simple_printf$("Should be -12345678901234567: %il", -12345678901234567l);
    simple_printf$("Should be 0x0123456789abcdef: %p", 0x0123456789abcdef);
    simple_printf$("Should be 0xdeadbeef: %x", 0xdeadbeef);
    simple_log("simple_printf$() test complete.");

    // infinite loop will be replaced with calling the OS entry point
    while (true) {}

    // free chunk of pages used for heap allocation
    efi_exit();

    // bootloader should never return
    return EfiLoadError;

}
