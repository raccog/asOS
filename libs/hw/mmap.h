#ifndef HW_MMAP_H
#define HW_MMAP_H

#include <hw/efi/efi.h>
#include <std/std.h>

typedef u64 OSMemoryAddress;

typedef enum {
    OSFree,
    OSReclaimable,
    OSReserved,
    OSLoader,
    OSKernel,
} OSMemoryKind;

typedef struct {
    OSMemoryKind kind;
    OSMemoryAddress start;
    size_t size;
} OSMemoryDescriptor;

typedef struct {
    OSMemoryDescriptor *descriptors;
    size_t entries;
} OSMemoryMap;

void output_mmap(OSMemoryMap *mmap);
void to_os_mmap(const EfiMemoryMap *efi_mmap, OSMemoryMap *OS_mmap);

#endif
