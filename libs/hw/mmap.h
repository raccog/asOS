#ifndef SUNNY_MMAP_H
#define SUNNY_MMAP_H

#include <hw/efi/efi.h>
#include <std/std.h>

typedef u64 SunnyMemoryAddress;

typedef enum {
    SunnyFree,
    SunnyReclaimable,
    SunnyReserved,
    SunnyLoader,
    SunnyKernel,
} SunnyMemoryMapKind;

typedef struct {
    SunnyMemoryMapKind kind;
    SunnyMemoryAddress start;
    size_t size;
} SunnyMemoryMapDescriptor;

typedef struct {
    SunnyMemoryMapDescriptor *descriptors;
    size_t entries;
} SunnyMemoryMap;

void to_sunny_mmap(const EfiMemoryMap *efi_mmap, SunnyMemoryMap *sunny_mmap);

#endif
