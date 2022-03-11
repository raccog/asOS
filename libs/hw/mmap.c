#include <hw/mmap.h>

#include <std/alloc.h>

// Memory descriptor type strings
const char *MMAP_KIND[] = {
    "Free",
    "Reclaimable",
    "Reserved",
    "Loader",
    "Kernel"
};

// print each entry of the memory map
void output_mmap(OSMemoryMap *mmap) {
    OSMemoryDescriptor *largest_free_descriptor = 0;
    size_t total_free = 0;
    size_t total_reserved = 0;
    size_t total_reclaimable = 0;

    // print memory map details
    simple_log("Descriptor entries: %i", mmap->entries);
    simple_log("Physical Start - Physical End | ID | Kind");

    // print each memory map descriptor
    for (int i = 0; i < mmap->entries; ++i) {
        OSMemoryDescriptor *descriptor = mmap->descriptors + i * sizeof(OSMemoryDescriptor);
        const char *kind_buf = MMAP_KIND[descriptor->kind];
        simple_log("%x-%x: %i %s", descriptor->start, descriptor->start + descriptor->size, i, kind_buf);
    }

    // get the total amount of free, reserved, and reclaimable memory and the largest section of free memory
    for (int i = 0; i < mmap->entries; ++i) {
        OSMemoryDescriptor *descriptor = mmap->descriptors + i * sizeof(OSMemoryDescriptor);
        switch (descriptor->kind) {
            case OSFree:
                total_free += descriptor->size;
                if (largest_free_descriptor == 0 || descriptor->size > largest_free_descriptor->size) {
                    largest_free_descriptor = descriptor;
                }
                break;
            case OSReserved:
                total_reserved += descriptor->size;
                break;
            case OSReclaimable:
                total_reclaimable += descriptor->size;
                break;
            default:
                break;
        }
    }

    // print out total free, reserved, and reclaimable memory and largest free section
    simple_log("Largest free: %x-%x, Size: %iMB", largest_free_descriptor->start, largest_free_descriptor->start + largest_free_descriptor->size, largest_free_descriptor->size >> 20);
    simple_log("Total free: %iMB", total_free >> 20);
    simple_log("Total reserved: %iMB", total_reserved >> 20);
    simple_log("Total reclaimable: %iMB", total_reclaimable >> 20);
}

void to_os_mmap(const EfiMemoryMap *efi_mmap, OSMemoryMap *os_mmap) {
    os_mmap->entries = 0;

    // allocate OS memory map descriptors
    alloc().alloc((u8 **)&os_mmap->descriptors, efi_mmap->entries * sizeof(OSMemoryDescriptor));

    // convert efi descriptors to OS descriptors
    for (int i = 0; i < efi_mmap->entries; ++i) {
        EfiMemoryDescriptor *efi_descriptor = (EfiMemoryDescriptor *)((void *)efi_mmap->descriptors + efi_mmap->descriptor_size * i);
        OSMemoryDescriptor *os_descriptor = os_mmap->descriptors + os_mmap->entries * sizeof(OSMemoryDescriptor);
        OSMemoryDescriptor *os_prev_descriptor = os_descriptor - sizeof(OSMemoryDescriptor);
        OSMemoryKind kind;

        // convert descriptor type
        switch (efi_descriptor->type) {
            case EfiLoaderCode:
            case EfiLoaderData:
                kind = OSLoader;
                break;
            case EfiBootServicesCode:
            case EfiBootServicesData:
                kind = OSReclaimable;
                break;
            case EfiConventionalMemory:
                kind = OSFree;
                break;
            case EfiRuntimeServicesCode:
            case EfiRuntimeServicesData:
            case EfiReservedMemoryType:
            case EfiUnusableMemory:
            case EfiACPIReclaimMemory:
            case EfiACPIMemoryNVS:
            case EfiMemoryMappedIO:
            case EfiMemoryMappedIOPortSpace:
            case EfiPalCode:
            case EfiPersistentMemory:
            default:
                kind = OSReserved;
        }

        // combine with previous descriptor if both are the same kind
        if (i == 0 || kind != os_prev_descriptor->kind) {
            os_descriptor->start = efi_descriptor->physical_start;
            os_descriptor->size = efi_descriptor->num_pages << 12;
            os_descriptor->kind = kind;
            ++os_mmap->entries;
        } else {
            os_prev_descriptor->size += efi_descriptor->num_pages << 12;
        }
    }
}
