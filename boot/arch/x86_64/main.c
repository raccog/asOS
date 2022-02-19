#include <efi/efi.h>
#include <std/alloc.h>
#include <std/int.h>
#include <std/io.h>
#include <std/log.h>

EfiChar16 *char16_buf;

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

// EFI output string wrapper
void output_string(const char *str) {
    EfiChar16 *buf16 = char8_to_char16(str);

    efi_print_str16(buf16);
    alloc().free((u8 *)buf16);
}

// needed for EFI application?
void __chkstk(void) {
    return;
}

EfiStatus efi_main(EfiHandle handle, EfiSystemTable *st) {
    Printer printer;
    EfiStatus status;

    // init EFI system table
    if ((status = efi_init(st)) != EFI_SUCCESS) {
        return status;
    }

    // init printer
    printer.output_string = &output_string;
    init_printer(printer);

    // allocate output buffers
    init_print_buffer();
    alloc().alloc((u8 **)&char16_buf, 2048);

    // get memory map
    EfiMemoryDescriptor memory_map[0xff];
    u64 memory_map_size = sizeof(EfiMemoryDescriptor) * 0xff;
    u64 map_key, descriptor_size;
    u32 descriptor_version;
    status = st->boot_services->get_memory_map(&memory_map_size, &memory_map[0], &map_key, &descriptor_size, &descriptor_version);
    if (status != EFI_SUCCESS) {
        return status;
    }

    // test log function
    st->console_out->clear_screen(st->console_out);
    simple_log("Hello kernel!");
    simple_log("Hello hex number 1: %x!", 0x1);
    simple_log("Hello hex number deadbeef: %x!", 0xdeadbeef);
    simple_log("Hello integer number 1: %i!", 1);
    simple_log("Hello integer number 1234: %i!", 1234);
    simple_log("Hello integer number -1234: %i!", -1234);
    simple_log("Hello string kernel: %s!", "kernel");
    simple_log("Hello char a: %c!", 'a');
    simple_log("Hello ptr deadbeefdeadbeef: %p!", 0xdeadbeefdeadbeef);
    simple_log("Hello bits 0000 0000 0000 1111: %b!", 15);
    simple_log("Hello bits 0000 0000 0001 0000: %b!", 16);


    while (true) {}

    return EFI_SUCCESS;

}
