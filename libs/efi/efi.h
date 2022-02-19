/*
 * EFI header
 *
 * Most of this is taken from the linux EFI header (https://github.com/torvalds/linux/blob/master/include/linux/efi.h)
 * Other parts are taken from the brutal project EFI loader (https://github.com/brutal-org/brutal) and the UEFI specifications 
 */

#ifndef EFI_H
#define EFI_H

#include <std/bool.h>
#include <std/int.h>

#define EFI_SUCCESS             0
#define EFI_ERROR               (1ull << 63)
#define EFI_LOAD_ERROR          (1  | EFI_ERROR)
#define EFI_INVALID_PARAMETER   (2  | EFI_ERROR)
#define EFI_UNSUPPORTED         (3  | EFI_ERROR)
#define EFI_BAD_BUFFER_SIZE     (4  | EFI_ERROR)
#define EFI_BUFFER_TOO_SMALL    (5  | EFI_ERROR)
#define EFI_NOT_READY           (6  | EFI_ERROR)
#define EFI_DEVICE_ERROR        (7  | EFI_ERROR)
#define EFI_WRITE_PROTECTED     (8  | EFI_ERROR)
#define EFI_OUT_OF_RESOURCES    (9  | EFI_ERROR)
#define EFI_NOT_FOUND           (14 | EFI_ERROR)
#define EFI_TIMEOUT             (18 | EFI_ERROR)
#define EFI_ABORTED             (21 | EFI_ERROR)
#define EFI_SECURITY_VIOLATION  (26 | EFI_ERROR)

typedef u64 EfiStatus;
typedef u8 EfiBool;
typedef u16 EfiChar16;
typedef u64 EfiPhysicalAddr;
typedef void *EfiHandle;
typedef u64 EfiPhysicalAddress;
typedef u64 EfiVirtualAddress;

typedef enum {
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
} EfiAllocateType;

typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiMaxMemoryType
} EfiMemoryType;

typedef struct {
    u64 signature;
    u32 revision;
    u32 header_size;
    u32 crc32;
    u32 _reserved;
} EfiTableHeader;

struct _EfiSimpleTextOutput;

typedef EfiStatus (*EfiTextReset)(struct _EfiSimpleTextOutput *self, bool extended_verification);
typedef EfiStatus (*EfiTextString)(struct _EfiSimpleTextOutput *self, EfiChar16 *string);
typedef EfiStatus (*EfiTextTestString)(struct _EfiSimpleTextOutput *self, EfiChar16 *string);
typedef EfiStatus (*EfiTextQueryMode)(struct _EfiSimpleTextOutput *self, u32 mode_number, u32 *columns, u32 *rows);
typedef EfiStatus (*EfiTextSetMode)(struct _EfiSimpleTextOutput *self, u32 mode_number);
typedef EfiStatus (*EfiTextSetAttribute)(struct _EfiSimpleTextOutput *self, u32 attribute);
typedef EfiStatus (*EfiTextClearScreen)(struct _EfiSimpleTextOutput *self);
typedef EfiStatus (*EfiTextSetCursorPosition)(struct _EfiSimpleTextOutput *self, u32 column, u32 row);
typedef EfiStatus (*EfiTextEnableCursor)(struct _EfiSimpleTextOutput *self, bool visible);

typedef struct {
    i32 max_mode;
    i32 mode;
    i32 attribute;
    i32 cursor_column;
    i32 cursor_row;
    bool cursor_visible;
} EfiSimpleTextOutputMode;

typedef struct _EfiSimpleTextOutput {
    EfiTextReset reset;
    EfiTextString output_string;
    EfiTextTestString test_string;
    EfiTextQueryMode query_mode;
    EfiTextSetMode set_mode;
    EfiTextSetAttribute set_attribute;
    EfiTextClearScreen clear_screen;
    EfiTextSetCursorPosition set_cursor_position;
    EfiTextEnableCursor enable_cursor;
    EfiSimpleTextOutputMode *mode;
} EfiSimpleTextOutput;

typedef struct _EfiMemoryDescriptor {
    u32 type;
    EfiPhysicalAddress physical_start;
    EfiVirtualAddress virtual_start;
    u64 num_pages;
    u64 attribute;
} EfiMemoryDescriptor;

typedef EfiStatus (*EfiAllocatePages)(EfiAllocateType type, EfiMemoryType memory_type, u64 pages, EfiPhysicalAddress *memory);
typedef EfiStatus (*EfiFreePages)(EfiPhysicalAddress memory, u64 pages);
typedef EfiStatus (*EfiGetMemoryMap)(u64 *memory_map_size, struct _EfiMemoryDescriptor *memory_map, u64 *map_key, u64 *descriptor_size, u32 *descriptor_version);

typedef struct {
    EfiTableHeader header;
    void *raise_tpl;
    void *restore_tpl;
    EfiAllocatePages allocate_pages;
    EfiFreePages free_pages;
    EfiGetMemoryMap get_memory_map;
    void *allocate_pool;
    void *free_pool;
    void *create_event;
    void *set_timer;
    void *wait_for_event;
    void *signal_event;
    void *close_event;
    void *check_event;
    void *install_protocol_interface;
    void *reinstall_protocol_interface;
    void *uninstall_protocol_interface;
    void *handle_protocol;
    void *_reserved;
    void *register_protocol_notify;
    void *locate_handle;
    void *locate_device_path;
    void *install_configuration_table;
    void *load_image;
    void *start_image;
    void *exit;
    void *unload_image;
    void *exit_boot_services;
    void *get_next_monotonic_count;
    void *stall;
    void *set_watchdog_timer;
    void *connect_controller;
    void *disconnect_controller;
    void *open_protocol;
    void *close_protocol;
    void *open_protocol_information;
    void *protocols_per_handle;
    void *locate_handle_buffer;
    void *locate_protocol;
    void *install_multiple_protocol_interfaces;
    void *uninstall_multiple_protocol_interfaces;
    void *calculate_crc32;
    void *copy_mem;
    void *set_mem;
    void *create_event_ex;
} EfiBootServices;

typedef struct {
	EfiTableHeader header;
	EfiChar16 *firmware_vendor;
	u32 firmware_revision;
	EfiHandle console_in_handle;
	u64 console_in;// simple text input pointer
	EfiHandle console_out_handle;
	EfiSimpleTextOutput *console_out;
	EfiHandle std_error_handle;
	EfiSimpleTextOutput *std_error;
	u64 runtime_services;   // runtime services pointer
	EfiBootServices *boot_services;
	u32 num_table_entries;
	u64 config_table;  // config table pointer
} EfiSystemTable;

// init global system table
EfiStatus efi_init(EfiSystemTable *st);

// efi page allocation function
EfiStatus efi_page_alloc(EfiPhysicalAddress *buf, size_t num_pages);

// efi page free function
EfiStatus efi_page_free(EfiPhysicalAddress buf);

// efi print function
EfiStatus efi_print_str16(EfiChar16 *str);

#endif
