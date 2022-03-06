#ifndef HW_MMAP_H
#define HW_MMAP_H

#include <hw/efi/efi.h>
#include <std/std.h>

// TODO: Support 32bit addressing
typedef u64 OSMemoryAddress;

// Types of memory descriptors
typedef enum {
    OSFree,
    OSReclaimable,
    OSReserved,
    OSLoader,
    OSKernel,
} OSMemoryKind;

// Description of a chunk of physical memory
typedef struct {
    OSMemoryKind kind;
    OSMemoryAddress start;
    size_t size;
} OSMemoryDescriptor;

// List of physical memory descriptors
typedef struct {
    OSMemoryDescriptor *descriptors;
    size_t entries;
} OSMemoryMap;

// Prints out an os memory map
void output_mmap(OSMemoryMap *mmap);

// Converts an efi memory map to an os memory map
void to_os_mmap(const EfiMemoryMap *efi_mmap, OSMemoryMap *OS_mmap);

#endif
