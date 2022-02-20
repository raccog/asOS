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

// returns a string describing an efi memory type
const char *mmap_type(EfiMemoryType type) {
    switch (type) {
        case EfiReservedMemoryType:
            return "Reserved";
        case EfiLoaderCode:
            return "Loader Code";
        case EfiLoaderData:
            return "Loader Data";
        case EfiBootServicesCode:
            return "Boot Services Code";
        case EfiBootServicesData:
            return "Boot Services Data";
        case EfiRuntimeServicesCode:
            return "Runtime Services Code";
        case EfiRuntimeServicesData:
            return "Runtime Services Data";
        case EfiConventionalMemory:
            return "Conventional";
        case EfiUnusableMemory:
            return "Unusable";
        case EfiACPIReclaimMemory:
            return "ACPI Reclaim";
        case EfiACPIMemoryNVS:
            return "ACPI NVS";
        case EfiMemoryMappedIO:
            return "Memory Mapped IO";
        case EfiMemoryMappedIOPortSpace:
            return "Memory Mapped IO Port Space";
        case EfiPalCode:
            return "Pal Code";
        case EfiPersistentMemory:
            return "Persistent";
        default:
            return "Unknown";
    }
}

void output_mmap(EfiMemoryDescriptor *mmap, u64 mmap_key, u64 mmap_size, u64 descriptor_size, u32 descriptor_version) {
    // print memory map details
    simple_log("Memory map key: %i", mmap_key);
    simple_log("Memory map descriptor version: %i", descriptor_version);
    simple_log("Num descriptors: %i", mmap_size / descriptor_size);
    simple_log("Descriptor size: %i", descriptor_size);
    simple_log("Physical Start - Physical End | ID | Type");

    // print each memory map descriptor
    for (int i = 0; i < mmap_size / descriptor_size; ++i) {
        // Conversion to void pointer is necessary to bypass struct alignment.
        // This is because the size of an EfiMemoryDescriptor struct might not be the same as the actual
        // descriptor size.
        EfiMemoryDescriptor *descriptor = (EfiMemoryDescriptor *)((void *)mmap + descriptor_size * i);
        const char *type_buf = mmap_type((EfiMemoryType)descriptor->type);
        simple_log("%x-%x: %i %s", descriptor->physical_start, descriptor->physical_start + (descriptor->num_pages << 12), i, type_buf);
    }
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
    EfiMemoryDescriptor mmap[0xff];
    u64 mmap_size = sizeof(EfiMemoryDescriptor) * 0xff;
    u64 mmap_key, descriptor_size;
    u32 descriptor_version;
    status = st->boot_services->get_memory_map(&mmap_size, &mmap[0], &mmap_key, &descriptor_size, &descriptor_version);
    if (status != EFI_SUCCESS) {
        return status;
    }

    // output memory map
    st->console_out->clear_screen(st->console_out);
    output_mmap(mmap, mmap_key, mmap_size, descriptor_size, descriptor_version);

    // test log function
    simple_log("Hello kernel!");

    while (true) {}

    return EFI_SUCCESS;

}
