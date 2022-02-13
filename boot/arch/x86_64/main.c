#include <efi/efi.h>
#include <std/alloc.h>
#include <std/io.h>
#include <std/log.h>

bool to_efi_alloc(uint8_t **ptr, size_t n) {
    efi_alloc((EfiPhysicalAddress *)ptr);
    return true;
}

void to_efi_free(uint8_t *ptr) {
    efi_free((EfiPhysicalAddress)ptr);
}

EfiChar16 *char8_to_char16(const char *buf) {
    const char *tmp_buf = buf;
    EfiChar16 *buf16;
    size_t count = 0;
    alloc().alloc((uint8_t **)&buf16, 0);

    do {
        ++count;
    } while (*(buf++) != '\0');
    buf = tmp_buf;

    for (size_t i = 0; i < count; ++i) {
        buf16[i] = (EfiChar16)buf[i];
    }

    return buf16;
}

void output_string(const char *str) {
    EfiChar16 *buf16 = char8_to_char16(str);

    efi_print_str16(buf16);
    alloc().free((uint8_t *)buf16);
}

void __chkstk(void) {
    return;
}

EfiStatus efi_main(EfiHandle handle, EfiSystemTable *st) {
    Allocator allocator;
    Printer printer;

    allocator.alloc = &to_efi_alloc;
    allocator.free = &to_efi_free;
    printer.output_string = &output_string;

    init_alloc(allocator);
    init_printer(printer);

    efi_init(st);


    EfiMemoryDescriptor memory_map[0xff];
    uint64_t memory_map_size = sizeof(EfiMemoryDescriptor) * 0xff;
    uint64_t map_key, descriptor_size;
    uint32_t descriptor_version;
    EfiStatus status = st->boot_services->get_memory_map(&memory_map_size, &memory_map[0], &map_key, &descriptor_size, &descriptor_version);

    st->console_out->clear_screen(st->console_out);
    log("Hello World!!! :D");
    log("Hello zero: %i", 0);
    log("Hello negative: %i", -500);
    log("Hello space padding:\r\n%5i", 5);
    log("Hello zero padding:\r\n%05i", 5);
    log("Hello zero precision padding:\r\n%.5i", 5);
    log("Hello space and zero padding:\r\n%5.3i", 5);

    return log("Hello number: %i", 649);

}
