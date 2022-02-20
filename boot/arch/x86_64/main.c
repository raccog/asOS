#include <efi/efi.h>
#include <std/alloc.h>
#include <std/int.h>
#include <std/io.h>
#include <std/log.h>
#include <sunny/mmap.h>

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

// returns a string describing an efi memory type
const char *mmap_kind(SunnyMemoryMapKind kind) {
    switch (kind) {
        case SunnyReserved:
            return "Reserved";
        case SunnyFree:
            return "Free";
        case SunnyReclaimable:
            return "Reclaimable";
        case SunnyLoader:
            return "Loader";
        case SunnyKernel:
            return "Kernel";
        default:
            return "Unknown";
    }
}

void output_mmap(SunnyMemoryMap *mmap) {
    SunnyMemoryMapDescriptor *largest_free_descriptor = 0;
    size_t total_free = 0;

    // print memory map details
    simple_log("Descriptor entries: %i", mmap->entries);
    simple_log("Physical Start - Physical End | ID | Kind");

    // print each memory map descriptor
    for (int i = 0; i < mmap->entries; ++i) {
        SunnyMemoryMapDescriptor *descriptor = mmap->descriptors + i * sizeof(SunnyMemoryMapDescriptor);
        const char *kind_buf = mmap_kind(descriptor->kind);
        simple_log("%x-%x: %i %s", descriptor->start, descriptor->start + descriptor->size, i, kind_buf);
    }

    for (int i = 0; i < mmap->entries; ++i) {
        SunnyMemoryMapDescriptor *descriptor = mmap->descriptors + i * sizeof(SunnyMemoryMapDescriptor);
        if (descriptor->kind == SunnyFree) {
            total_free += descriptor->size;
            if (largest_free_descriptor == 0 || descriptor->size > largest_free_descriptor->size) {
                largest_free_descriptor = descriptor;
            }
        }
    }

    const char *kind_buf = mmap_kind(largest_free_descriptor->kind);
    simple_log("Largest free: %x-%x, Size: %iMB", largest_free_descriptor->start, largest_free_descriptor->start + largest_free_descriptor->size, largest_free_descriptor->size >> 20);
    simple_log("Total free: %iMB", total_free >> 20);
}

EfiStatus efi_main(EfiHandle handle, EfiSystemTable *st) {
    Printer printer;
    EfiStatus status;
    EfiMemoryMap efi_mmap;
    SunnyMemoryMap sunny_mmap;

    // init EFI system table
    if ((status = efi_init(st)) != EFI_SUCCESS) {
        return status;
    }

    // disable watchdog timer
    st->boot_services->set_watchdog_timer(0, 0, 0, 0);

    // init printer
    printer.output_string = &output_string;
    init_printer(printer);

    // allocate buffers
    init_print_buffer();
    alloc().alloc((u8 **)&char16_buf, 2048);
    size_t efi_mmap_size = sizeof(EfiMemoryDescriptor) * 0xff;
    alloc().alloc((u8 **)&efi_mmap.descriptors, efi_mmap_size);

    // get memory map
    status = st->boot_services->get_memory_map(&efi_mmap_size, efi_mmap.descriptors, &efi_mmap.key, &efi_mmap.descriptor_size, &efi_mmap.version);
    efi_mmap.entries = efi_mmap_size / efi_mmap.descriptor_size;
    if (status != EFI_SUCCESS) {
        return status;
    }

    // output memory map
    st->console_out->clear_screen(st->console_out);
    to_sunny_mmap(&efi_mmap, &sunny_mmap);
    output_mmap(&sunny_mmap);

    // test log function
    simple_log("Hello kernel!");

    while (true) {}

    return EFI_SUCCESS;

}
