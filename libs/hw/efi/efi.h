/*
 * EFI header
 *
 * Most of this is taken from the linux EFI header (https://github.com/torvalds/linux/blob/master/include/linux/efi.h)
 * Other parts are taken from the brutal project EFI loader (https://github.com/brutal-org/brutal) and the UEFI specifications 
 * 
 * Some pieces of documentation are taken from the UEFI specification document.
 */

#ifndef HW_EFI_H
#define HW_EFI_H

#include <std/std.h>

// EFIAPI is defined just to follow calling conventions,
// it is not needed for compilation.
#define EFIAPI

// EFI return codes
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

typedef EfiStatus (EFIAPI *EfiTextReset)(struct _EfiSimpleTextOutput *self, bool extended_verification);
typedef EfiStatus (EFIAPI *EfiTextString)(struct _EfiSimpleTextOutput *self, EfiChar16 *string);
typedef EfiStatus (EFIAPI *EfiTextTestString)(struct _EfiSimpleTextOutput *self, EfiChar16 *string);
typedef EfiStatus (EFIAPI *EfiTextQueryMode)(struct _EfiSimpleTextOutput *self, uptr mode_number, uptr *columns, uptr *rows);
typedef EfiStatus (EFIAPI *EfiTextSetMode)(struct _EfiSimpleTextOutput *self, uptr mode_number);
typedef EfiStatus (EFIAPI *EfiTextSetAttribute)(struct _EfiSimpleTextOutput *self, uptr attribute);
typedef EfiStatus (EFIAPI *EfiTextClearScreen)(struct _EfiSimpleTextOutput *self);
typedef EfiStatus (EFIAPI *EfiTextSetCursorPosition)(struct _EfiSimpleTextOutput *self, uptr column, uptr row);
typedef EfiStatus (EFIAPI *EfiTextEnableCursor)(struct _EfiSimpleTextOutput *self, bool visible);

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

typedef struct _EfiDevicePathProtocol {
    u8 type;
    u8 sub_type;
    u8 length[2];
} EfiDevicePathProtocol;

typedef uptr (EFIAPI *EfiDevicePathUtilsGetDevicePathSize)(const EfiDevicePathProtocol *device_path);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsDupDevicePath)(const EfiDevicePathProtocol *device_path);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsAppendPath)(const EfiDevicePathProtocol *src1, const EfiDevicePathProtocol *src2);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsAppendNode)(const EfiDevicePathProtocol *device_path, const EfiDevicePathProtocol *device_node);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsAppendInstance)(const EfiDevicePathProtocol *device_path, 
    const EfiDevicePathProtocol *device_path_instance);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsGetNextInstance)(EfiDevicePathProtocol **device_path_instance, uptr *device_path_instance_size);
typedef bool (EFIAPI *EfiDevicePathUtilsIsMultiInstance)(const EfiDevicePathProtocol *device_path);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsCreateNode)(u8 node_type, u8 node_sub_type, u16 node_length);

typedef struct _EfiDevicePathUtilitiesProtocol {
    // Returns the size of the specified device path, in bytes.
    EfiDevicePathUtilsGetDevicePathSize get_device_path_size;
    // Duplicates a device path structure.
    EfiDevicePathUtilsDupDevicePath duplicate_device_path;
    // Appends the device node to the specified device path.
    EfiDevicePathUtilsAppendPath append_device_path;
    // Appends the device path to the specified device path.
    EfiDevicePathUtilsAppendNode append_device_node;
    // Appends a device path instance to another device path.
    EfiDevicePathUtilsAppendInstance append_device_path_instance;
    // Retrieves the next device path instance from a device path data structure.
    EfiDevicePathUtilsGetNextInstance get_next_device_path_instance;
    // Returns true if this is a multi-instance device path.
    EfiDevicePathUtilsIsMultiInstance is_device_path_multi_instance;
    // Allocates memory for a device node with the specified type and sub type.
    EfiDevicePathUtilsCreateNode create_device_node;
} EfiDevicePathUtilitiesProtocol;

typedef struct _EfiMemoryDescriptor {
    u32 type;
    u32 _pad1;
    EfiPhysicalAddress physical_start;
    EfiVirtualAddress virtual_start;
    u64 num_pages;
    u64 attribute;
} EfiMemoryDescriptor;

typedef struct {
    EfiMemoryDescriptor *descriptors;
    uptr entries;
    uptr descriptor_size;
    uptr key;
    u32 version;
} EfiMemoryMap;

typedef EfiStatus (*EfiAllocatePages)(EfiAllocateType type, EfiMemoryType memory_type, uptr pages, EfiPhysicalAddress *memory);
typedef EfiStatus (*EfiFreePages)(EfiPhysicalAddress memory, uptr pages);
typedef EfiStatus (*EfiGetMemoryMap)(uptr *memory_map_size, struct _EfiMemoryDescriptor *memory_map, uptr *map_key, uptr *descriptor_size, u32 *descriptor_version);
typedef EfiStatus (*EfiSetWatchdogTimer)(uptr timeout, u64 code, uptr data_size, EfiChar16 *data);

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
    EfiSetWatchdogTimer set_watchdog_timer;
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
	void *console_in;// simple text input pointer
	EfiHandle console_out_handle;
	EfiSimpleTextOutput *console_out;
	EfiHandle std_error_handle;
	EfiSimpleTextOutput *std_error;
	void *runtime_services;   // runtime services pointer
	EfiBootServices *boot_services;
	uptr num_table_entries;
	void *config_table;  // config table pointer
} EfiSystemTable;

typedef EfiStatus (EFIAPI *EfiImageEntryPoint)(EfiHandle handle, EfiSystemTable *st);

#endif
