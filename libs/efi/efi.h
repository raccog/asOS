/*
 * EFI header
 *
 * Most of this is taken from the linux EFI header (https://github.com/torvalds/linux/blob/master/include/linux/efi.h)
 * Other parts are taken from the brutal project EFI loader (https://github.com/brutal-org/brutal) and the UEFI specifications 
 */

#ifndef EFI_H
#define EFI_H

#include <stdbool.h>
#include <stdint.h>

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

typedef uint64_t EfiStatus;
typedef uint8_t EfiBool;
typedef uint16_t EfiChar16;
typedef uint64_t EfiPhysicalAddr;
typedef void *EfiHandle;
typedef uint64_t EfiPhysicalAddress;
typedef uint64_t EfiVirtualAddress;

typedef struct {
    uint64_t signature;
    uint32_t revision;
    uint32_t header_size;
    uint32_t crc32;
    uint32_t _reserved;
} EfiTableHeader;

struct _EfiSimpleTextOutput;

typedef EfiStatus (*EfiTextReset)(struct _EfiSimpleTextOutput *self, bool extended_verification);
typedef EfiStatus (*EfiTextString)(struct _EfiSimpleTextOutput *self, EfiChar16 *string);
typedef EfiStatus (*EfiTextTestString)(struct _EfiSimpleTextOutput *self, EfiChar16 *string);
typedef EfiStatus (*EfiTextQueryMode)(struct _EfiSimpleTextOutput *self, uint32_t mode_number, uint32_t *columns, uint32_t *rows);
typedef EfiStatus (*EfiTextSetMode)(struct _EfiSimpleTextOutput *self, uint32_t mode_number);
typedef EfiStatus (*EfiTextSetAttribute)(struct _EfiSimpleTextOutput *self, uint32_t attribute);
typedef EfiStatus (*EfiTextClearScreen)(struct _EfiSimpleTextOutput *self);
typedef EfiStatus (*EfiTextSetCursorPosition)(struct _EfiSimpleTextOutput *self, uint32_t column, uint32_t row);
typedef EfiStatus (*EfiTextEnableCursor)(struct _EfiSimpleTextOutput *self, bool visible);

typedef struct {
    int32_t max_mode;
    int32_t mode;
    int32_t attribute;
    int32_t cursor_column;
    int32_t cursor_row;
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
    uint32_t type;
    EfiPhysicalAddress physical_start;
    EfiVirtualAddress virtual_start;
    uint64_t num_pages;
    uint64_t attribute;
} EfiMemoryDescriptor;

typedef EfiStatus (*EfiGetMemoryMap)(uint64_t *memory_map_size, struct _EfiMemoryDescriptor *memory_map, uint64_t *map_key, uint64_t *descriptor_size, uint32_t *descriptor_version);

typedef struct {
    EfiTableHeader header;
    void *raise_tpl;
    void *restore_tpl;
    void *allocate_pages;
    void *free_pages;
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
	uint32_t firmware_revision;
	EfiHandle console_in_handle;
	uint64_t console_in;// simple text input pointer
	EfiHandle console_out_handle;
	EfiSimpleTextOutput *console_out;
	EfiHandle std_error_handle;
	EfiSimpleTextOutput *std_error;
	uint64_t runtime_services;   // runtime services pointer
	EfiBootServices *boot_services;
	uint32_t num_table_entries;
	uint64_t config_table;  // config table pointer
} EfiSystemTable;

#endif