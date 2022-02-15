#include <efi/efi.h>
#include <std/alloc.h>
#include <std/int.h>
#include <std/io.h>
#include <std/log.h>

// EFI alloc wrapper
bool to_efi_alloc(u8 **ptr, size_t n) {
    efi_alloc((EfiPhysicalAddress *)ptr);
    return true;
}

// EFI free wrapper
void to_efi_free(u8 *ptr) {
    efi_free((EfiPhysicalAddress)ptr);
}

// convert char8 string to char16 string
//
// returned string should be freed when not in use
EfiChar16 *char8_to_char16(const char *buf) {
    const char *tmp_buf = buf;
    EfiChar16 *buf16;
    size_t count = 0;

    // allocate char16 buffer
    alloc().alloc((u8 **)&buf16, 0);

    // count characters in string
    do {
        ++count;
    } while (*(buf++) != '\0');
    buf = tmp_buf;

    // copy characters to char16 string
    for (size_t i = 0; i < count; ++i) {
        buf16[i] = (EfiChar16)buf[i];
    }

    return buf16;
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
    Allocator allocator;
    Printer printer;

    // set alloc, free, and print function pointers
    allocator.alloc = &to_efi_alloc;
    allocator.free = &to_efi_free;
    printer.output_string = &output_string;

    // init allocator and printer
    init_alloc(allocator);
    init_printer(printer);

    // init EFI system table
    efi_init(st);

    // get memory map
    EfiMemoryDescriptor memory_map[0xff];
    u64 memory_map_size = sizeof(EfiMemoryDescriptor) * 0xff;
    u64 map_key, descriptor_size;
    u32 descriptor_version;
    EfiStatus status = st->boot_services->get_memory_map(&memory_map_size, &memory_map[0], &map_key, &descriptor_size, &descriptor_version);

    // test log function
    st->console_out->clear_screen(st->console_out);
    log("Hello World!!! :D");
    log("Hello zero: %i", 0);
    log("Hello negative: %i", -500);
    log("Hello space padding:\r\n%5i", 5);
    log("Hello zero padding:\r\n%05i", 5);
    log("Hello zero precision padding:\r\n%.5i", 5);
    log("Hello space and zero precision padding:\r\n%5.3i", 5);
    log("Hello negative space and zero precision padding:\r\n%5.3i", -5);
    log("Hello negative zero precision padding:\r\n%.5i", -5);
    log("Hello negative space and zero padding:\r\n%05i", -5);
    log("Hello left justify negative zero padding:\r\n%-05idone", -5);
    log("Hello left justify zero padding:\r\n%-05idone", 5);
    log("Hello left justify:\r\n%-5idone", 5);
    log("Hello left justify negative:\r\n%-5idone", -5);
    log("Hello left justify force positive:\r\n%-+5idone", 5);
    log("Hello hex 0: %x", 0x0);
    log("Hello hex ca: %x", 0xca);
    log("Hello hex 4: %x", 0x4);
    log("Hello hex deadbeef: %x", 0xdeadbeef);
    log("Hello hex 0xdeadbeef: %#x", 0xdeadbeef);
    log("Hello hex 0XDEADBEEF: %#X", 0xdeadbeef);
    log("Hello hex 0x00000001: %#.32x", 0x1);

    log("Hello number: %i", 649);

    while (true) {}

    return EFI_SUCCESS;

}
