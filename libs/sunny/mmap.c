#include <sunny/mmap.h>

#include <std/alloc.h>

void to_sunny_mmap(const EfiMemoryMap *efi_mmap, SunnyMemoryMap *sunny_mmap) {
    sunny_mmap->entries = 0;

    // allocate sunny memory map descriptors
    alloc().alloc((u8 **)&sunny_mmap->descriptors, efi_mmap->entries * sizeof(SunnyMemoryMapDescriptor));

    // convert efi descriptors to sunny descriptors
    for (int i = 0; i < efi_mmap->entries; ++i) {
        EfiMemoryDescriptor *efi_descriptor = (EfiMemoryDescriptor *)((void *)efi_mmap->descriptors + efi_mmap->descriptor_size * i);
        SunnyMemoryMapDescriptor *sunny_descriptor = sunny_mmap->descriptors + sunny_mmap->entries * sizeof(SunnyMemoryMapDescriptor);
        SunnyMemoryMapDescriptor *sunny_prev_descriptor = sunny_descriptor - sizeof(SunnyMemoryMapDescriptor);
        SunnyMemoryMapKind kind;

        switch (efi_descriptor->type) {
            case EfiLoaderCode:
            case EfiLoaderData:
                kind = SunnyLoader;
                break;
            case EfiBootServicesCode:
            case EfiBootServicesData:
                kind = SunnyReclaimable;
                break;
            case EfiRuntimeServicesCode:
            case EfiRuntimeServicesData:
                kind = SunnyReserved;
                break;
            case EfiConventionalMemory:
                kind = SunnyFree;
                break;
            case EfiReservedMemoryType:
            case EfiUnusableMemory:
            case EfiACPIReclaimMemory:
            case EfiACPIMemoryNVS:
            case EfiMemoryMappedIO:
            case EfiMemoryMappedIOPortSpace:
            case EfiPalCode:
            case EfiPersistentMemory:
            default:
                kind = SunnyReserved;
        }

        if (i == 0 || kind != sunny_prev_descriptor->kind) {
            sunny_descriptor->start = efi_descriptor->physical_start;
            sunny_descriptor->size = efi_descriptor->num_pages << 12;
            sunny_descriptor->kind = kind;
            ++sunny_mmap->entries;
        } else {
            sunny_prev_descriptor->size += efi_descriptor->num_pages << 12;
        }
    }
}
