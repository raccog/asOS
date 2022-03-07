/*
 * EFI header
 *
 * Most of this is taken from the linux EFI header (https://github.com/torvalds/linux/blob/master/include/linux/efi.h)
 * Other parts are taken from the brutal project EFI loader (https://github.com/brutal-org/brutal) and the UEFI specifications 
 * 
 * Some pieces of documentation are taken from the UEFI specification document.
 * The section of the UEFI specification that the documentation and definitions were taken from will be listed as well.
 */

#ifndef HW_EFI_H
#define HW_EFI_H

#include <std/std.h>

// EFIAPI is defined just to follow calling conventions,
// it is not needed for compilation.
#define EFIAPI

// EFI return codes
// Defined in Appendix D - Status Codes
// Success Codes
#define EfiSuccess              0
// Error codes
#define EfiError                (1ull << 63)
#define EfiLoadError            (1  | EfiError)
#define EfiInvalidParameter     (2  | EfiError)
#define EfiUnsupported          (3  | EfiError)
#define EfiBadBufferSize        (4  | EfiError)
#define EfiNotReady             (6  | EfiError)
#define EfiDeviceError          (7  | EfiError)
#define EfiWriteProtected       (8  | EfiError)
#define EfiOutOfResources       (9  | EfiError)
#define EfiNotFound             (14 | EfiError)
#define EfiTimeout              (18 | EfiError)
#define EfiAborted              (21 | EfiError)
#define EfiSecurityViolation    (26 | EfiError)
// Warning codes
#define EfiBufferTooSmall       (5  | EfiError)

// Common UEFI Data Types
// Defined in 2.3.1 Data Types
typedef uptr EfiStatus;
typedef u8 EfiBool;
typedef u16 EfiChar16;
typedef void *EfiHandle;
typedef u64 EfiPhysicalAddress;
typedef u64 EfiVirtualAddress;
typedef void *EfiEvent;
typedef u64 EfiLba;
typedef uptr EfiTpl;

// PE32+ Subsystem type for EFI images
// Defined in 2.1.1 UEFI Images
#define EfiImageSubsystemEfiApplication         10
#define EfiImageSubsystemEfiBootServiceDriver   11
#define EfiImageSubsystemEfiRuntimeDriver       12

// PE32+ Machine type for EFI images
// Defined in 2.1.1 UEFI Images
#define EfiImageMachineIA32             0x014c
#define EfiImageMachineIA64             0x0200
#define EfiImageMachineEBC              0x0ebc
#define EfiImageMachinex64              0x8664
#define EfiImageMachineArmThumbMixed    0x01c2
#define EfiImageMachineAarch64          0xaa64
#define EfiImageMachineRiscv32          0x5032
#define EfiImageMachineRiscv64          0x5064
#define EfiImageMachineRiscv128         0x5128

// Event Types
// These types can be ORed together as needed
// Defined in 7.1 Event, Timer, and Task Priority Services - EFI_BOOT_SERVICES.CreateEvent()
#define EventTimer                      0x80000000
#define EventRuntime                    0x40000000
#define EventNotifyWait                 0x00000100
#define EventNotifySignal               0x00000200
#define EventSignalExitBootServices     0x00000201
#define EventSignalVirtualAddressChange 0x60000202

// EFI GUID
// Defined in 7.3 Protocol Handler Services - EFI_BOOT_SERVICES.InstallProtocolInterface()
typedef struct {
    u32 data1;
    u16 data2;
    u16 data3;
    u8 data4[8];
} EfiGuid;

// EFI Interface Type
// Defined in 7.3 Protocol Handler Services - EFI_BOOT_SERVICES.InstallProtocolInterface()
typedef enum {
    EfiNativeInterface
} EfiInterfaceType;

// EFI Allocate Type
// Defined in 7.2 Memory Allocation Services - EFI_BOOT_SERVICES.AllocatePages()
typedef enum {
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
} EfiAllocateType;

// EFI Memory Type
// Defined in 7.2 Memory Allocation Services - EFI_BOOT_SERVICES.AllocatePages()
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

// Data structure that precedes all of the standard EFI table types.
// Defined in 4.2 EFI Table Header
typedef struct {
    u64 signature;
    u32 revision;
    u32 header_size;
    u32 crc32;
    u32 _reserved;
} EfiTableHeader;

struct _EfiSimpleTextOutputProtocol;

// Simple Text Output Protocol Function Signatures
// Defined in 12.4 Simple Text Output Protocol
typedef EfiStatus (EFIAPI *EfiTextReset)(struct _EfiSimpleTextOutputProtocol *self, bool extended_verification);
typedef EfiStatus (EFIAPI *EfiTextString)(struct _EfiSimpleTextOutputProtocol *self, EfiChar16 *string);
typedef EfiStatus (EFIAPI *EfiTextTestString)(struct _EfiSimpleTextOutputProtocol *self, EfiChar16 *string);
typedef EfiStatus (EFIAPI *EfiTextQueryMode)(struct _EfiSimpleTextOutputProtocol *self, uptr mode_number, uptr *columns, uptr *rows);
typedef EfiStatus (EFIAPI *EfiTextSetMode)(struct _EfiSimpleTextOutputProtocol *self, uptr mode_number);
typedef EfiStatus (EFIAPI *EfiTextSetAttribute)(struct _EfiSimpleTextOutputProtocol *self, uptr attribute);
typedef EfiStatus (EFIAPI *EfiTextClearScreen)(struct _EfiSimpleTextOutputProtocol *self);
typedef EfiStatus (EFIAPI *EfiTextSetCursorPosition)(struct _EfiSimpleTextOutputProtocol *self, uptr column, uptr row);
typedef EfiStatus (EFIAPI *EfiTextEnableCursor)(struct _EfiSimpleTextOutputProtocol *self, bool visible);


// Simple Text Output Mode
// Defined in 12.4 Simple Text Output Protocol
typedef struct {
    i32 max_mode;
    i32 mode;
    i32 attribute;
    i32 cursor_column;
    i32 cursor_row;
    bool cursor_visible;
} EfiSimpleTextOutputMode;

// This protocol is used to control text-based output devices.
// Defined in 12.4 Simple Text Output Protocol
typedef struct _EfiSimpleTextOutputProtocol {
    // Resets the text output device hardware.
    EfiTextReset reset;
    // Writes a string to the output device.
    EfiTextString output_string;
    // Verifies that all characters in a string can be output to the target device.
    EfiTextTestString test_string;
    // Returns information for an available text mode that the output device(s) supports.
    EfiTextQueryMode query_mode;
    // Sets the output device(s) to a specified mode.
    EfiTextSetMode set_mode;
    // Sets the background and foreground colors for the output_string() and clear_screen() functions.
    EfiTextSetAttribute set_attribute;
    // Clears the output device(s) display to the currently selected background color.
    EfiTextClearScreen clear_screen;
    // Sets the current coordinates of the cursor position.
    EfiTextSetCursorPosition set_cursor_position;
    // Makes the cursor visible or invisible.
    EfiTextEnableCursor enable_cursor;
    EfiSimpleTextOutputMode *mode;
} EfiSimpleTextOutputProtocol;

// Can be used on any device handle to obtain generic path/location information concerning the physical device or logical device.
// Defined in 10.2 EFI Device Path Protocol
typedef struct _EfiDevicePathProtocol {
    u8 type;
    u8 sub_type;
    u8 length[2];
} EfiDevicePathProtocol;

// Device Path Utilities Protocol Function Signatures
// Defined in 10.5 Device Path Utilities Protocol
typedef uptr (EFIAPI *EfiDevicePathUtilsGetDevicePathSize)(const EfiDevicePathProtocol *device_path);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsDupDevicePath)(const EfiDevicePathProtocol *device_path);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsAppendPath)(const EfiDevicePathProtocol *src1, const EfiDevicePathProtocol *src2);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsAppendNode)(const EfiDevicePathProtocol *device_path, const EfiDevicePathProtocol *device_node);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsAppendInstance)(const EfiDevicePathProtocol *device_path, 
    const EfiDevicePathProtocol *device_path_instance);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsGetNextInstance)(EfiDevicePathProtocol **device_path_instance, uptr *device_path_instance_size);
typedef bool (EFIAPI *EfiDevicePathUtilsIsMultiInstance)(const EfiDevicePathProtocol *device_path);
typedef EfiDevicePathProtocol *(EFIAPI *EfiDevicePathUtilsCreateNode)(u8 node_type, u8 node_sub_type, u16 node_length);

// Creates and manipulates device paths and device nodes.
// Defined in 10.5 Device Path Utilities Protocol
typedef struct _EfiDevicePathUtilitiesProtocol {
    // Returns the size of the specified device path, in bytes.
    EfiDevicePathUtilsGetDevicePathSize get_device_path_size;
    // Duplicates a device path structure.
    EfiDevicePathUtilsDupDevicePath duplicate_device_path;
    // Appends the device path to the specified device path.
    EfiDevicePathUtilsAppendPath append_device_path;
    // Appends the device node to the specified device path.
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

struct _EfiLoadFileProtocol;

// Load File Protocol Function Signatures
// Defined in 13.1 Load File Protocol
typedef EfiStatus (EFIAPI *EfiLoadFile)(struct _EfiLoadFileProtocol *this, EfiDevicePathProtocol *file_path, bool boot_policy, uptr *buffer_size, 
    void *buffer);

// Used to obtain files, that are primarily boot options, from arbitrary devices.
// Defined in 13.1 Load File Protocol
typedef struct _EfiLoadFileProtocol {
    // Causes the driver to load a specified file.
    EfiLoadFile load_file;
} EfiLoadFileProtocol;

// Used to obtain files from arbitrary devices but are not used as boot options.
// Defined in 13.2 Load File 2 Protocol
typedef EfiLoadFileProtocol EfiLoadFile2Protocol;

// Load File 2 Protocol Interface Structure
// Defined in 13.2 Load File 2 Protocol
#define EfiSimpleFileSystemProtocolRevision 0x00010000

struct _EfiSimpleFileSystemProtocol;
struct _EfiFileProtocol;

// Simple File System Protocol Function Signatures
// Defined in 13.4 Simple File System Protocol
typedef EfiStatus (EFIAPI *EfiSimpleFileSystemProtocolOpenVolume)(struct _EfiSimpleFileSystemProtocol *this, struct _EfiFileProtocol **root);

// Provides a minimal interface for file-type access to a device.
// Defined in 13.4 Simple File System Protocol
typedef struct _EfiSimpleFileSystemProtocol {
    u64 revision;
    // Opens the root directory on a volume.
    EfiSimpleFileSystemProtocolOpenVolume open_volume;
} EfiSimpleFileSystemProtocol;

// File Protocol Revision Numbers
// Defined in 13.5 File Protocol
#define EfiFileProtocolRevision         0x00010000
#define EfiFileProtocolRevision2        0x00020000
#define EfiFileProtocolLatestRevision   EfiFileProtocolRevision2

// File Open Modes
// Defined in 13.5 File Protocol - EFI_FILE_PROTOCOL.Open()
#define EfiFileModeRead     0x0000000000000001
#define EfiFileModeWrite    0x0000000000000002
#define EfiFileModeCreate   0x0000000000000003

// File Attributes
// Defined in 13.5 File Protocol - EFI_FILE_PROTOCOL.Open()
#define EfiFileReadOnly     0x0000000000000001
#define EfiFileHidden       0x0000000000000002
#define EfiFileSystem       0x0000000000000004
#define EfiFileReserved     0x0000000000000008
#define EfiFileDirectory    0x0000000000000010
#define EfiFileArchive      0x0000000000000020
#define EfiFileValidAttr    0x0000000000000037

// File IO Token
// Defined in 13.5 File Protocol - EFI_FILE_PROTOCOL.OpenEx()
typedef struct {
    EfiEvent event;
    EfiStatus status;
    uptr buffer_size;
    void *buffer;
} EfiFileIoToken;

// This represents the current time information
// Defined in 8.3 Time Services - GetTime()
typedef struct {
    u16 year;           // 1900 - 9999
    u8 month;           // 1 - 12
    u8 day;             // 1 - 31
    u8 hour;            // 0 - 23
    u8 minute;          // 0 - 59
    u8 second;          // 0 - 59
    u8 _pad1;
    u32 nanosecond;     // 0 - 999,999,999
    u16 time_zone;      // -1440 to 1440 or 2047
    u8 daylight;
    u8 _pad2;
} EfiTime;

// File Protocol Function Signatures
// Defined in 13.5 File Protocol
typedef EfiStatus (EFIAPI *EfiFileOpen)(struct _EfiFileProtocol *this, struct _EfiFileProtocol **new_handle, const EfiChar16 *file_name, u64 open_mode, 
    u64 attributes);
typedef EfiStatus (EFIAPI *EfiFileClose)(struct _EfiFileProtocol *this);
typedef EfiStatus (EFIAPI *EfiFileDelete)(struct _EfiFileProtocol *this);
typedef EfiStatus (EFIAPI *EfiFileRead)(struct _EfiFileProtocol *this, uptr *buffer_size, void *buffer);
typedef EfiStatus (EFIAPI *EfiFileWrite)(struct _EfiFileProtocol *this, uptr *buffer_size, void *buffer);
typedef EfiStatus (EFIAPI *EfiFileOpenEx)(struct _EfiFileProtocol *this, struct _EfiFileProtocol **new_handle, const EfiChar16 *file_name, u64 open_mode, 
    u64 attributes, EfiFileIoToken *token);
typedef EfiStatus (EFIAPI *EfiFileReadEx)(struct _EfiFileProtocol *this, EfiFileIoToken *token);
typedef EfiStatus (EFIAPI *EfiFileWriteEx)(struct _EfiFileProtocol *this, EfiFileIoToken *token);
typedef EfiStatus (EFIAPI *EfiFileFlushEx)(struct _EfiFileProtocol *this, EfiFileIoToken *token);
typedef EfiStatus (EFIAPI *EfiFileSetPosition)(struct _EfiFileProtocol *this, u64 position);
typedef EfiStatus (EFIAPI *EfiFileGetPosition)(struct _EfiFileProtocol *this, u64 *position);
typedef EfiStatus (EFIAPI *EfiFileGetInfo)(struct _EfiFileProtocol *this, EfiGuid *information_type, uptr *buffer_size, void *buffer);
typedef EfiStatus (EFIAPI *EfiFileSetInfo)(struct _EfiFileProtocol *this, EfiGuid *information_type, uptr buffer_size, void *buffer);
typedef EfiStatus (EFIAPI *EfiFileFlush)(struct _EfiFileProtocol *this);

// Provides a GUID and a data structure that can be used with EfiFileProtocol.set_info() and EfiFileProtocol.get_info() to set or get generic file information.
// Defined in 13.5 File Protocol - EFI_FILE_INFO
typedef struct {
    u64 size;
    u64 file_size;
    u64 physical_size;
    EfiTime create_time;
    EfiTime last_access_time;
    EfiTime modification_time;
    u64 attribute;
    EfiChar16 *filename;
} EfiFileInfo;

// Provides a GUID and a data structure that can be used with EfiFileProtocol.get_info() to get information about the system volume, and
// EfiFileProtocol.set_info() to set the system volume’s volume label.
// Defined in 13.5 File Protocol - EFI_FILE_SYSTEM_INFO
typedef struct {
    u64 size;
    bool read_only;
    u64 volume_size;
    u64 free_space;
    u32 block_size;
    EfiChar16 *volume_label;
} EfiFileSystemInfo;

// Provides a GUID and a data structure that can be used with EfiFileProtocol.get_info() or EfiFileProtocol.set_info() to get or set information about the
// system’s volume label.
// Defined in 13.5 File Protocol - EFI_FILE_SYSTEM_VOLUME_LABEL
typedef struct {
    EfiChar16 *volume_label;
} EfiFileSystemVolumeLabel;

// File Protocol revisions numbers
// Defined in 13.5 File Protocol
#define EfiFileProtocolRevision         0x00010000
#define EfiFileProtocolRevision2        0x00020000
#define EfiFileProtocolLatestRevision   EfiFileProtocolRevision2

// Provides file based access to supported file systems.
// Defined in 13.5 File Protocol
typedef struct _EfiFileProtocol {
    u64 revision;
    // Opens a new file relative to the source file’s location.
    EfiFileOpen open;
    // Closes a specified file handle.
    EfiFileClose close;
    // Closes and deletes a file.
    EfiFileDelete delete;
    // Reads data from a file.
    EfiFileRead read;
    // Writes data to a file.
    EfiFileWrite write;
    // Returns a file’s current position.
    EfiFileGetPosition get_position;
    // Sets a file’s current position.
    EfiFileSetPosition set_position;
    // Returns information about a file.
    EfiFileGetInfo get_info;
    // Sets information about a file.
    EfiFileSetInfo set_info;
    // Flushes all modified data associated with a file to a device.
    EfiFileFlush flush;
    // Opens a new file relative to the source directory’s location.
    EfiFileOpenEx open_ex;      // added in revision 2
    // Reads data from a file.
    EfiFileReadEx read_ex;      // added in revision 2
    // Writes data to a file.
    EfiFileWriteEx write_ex;    // added in revision 2
    // Flushes all modified data associated with a file to a device.
    EfiFileFlushEx flush_ex;    // added in revision 2
} EfiFileProtocol;

struct _EfiDiskIoProtocol;

// Disk I/O Protocol Function Signatures
// Defined in 13.7 Disk I/O Protocol
typedef EfiStatus (EFIAPI *EfiDiskRead)(struct _EfiDiskIoProtocol *this, u32 media_id, u64 offset, uptr buffer_size, void *buffer);
typedef EfiStatus (EFIAPI *EfiDiskWrite)(struct _EfiDiskIoProtocol *this, u32 media_id, u64 offset, uptr buffer_size, void *buffer);

// Disk I/O Protocol Revision Number
// Defined in 13.7 Disk I/O Protocol
#define EfiDiskIoProtocolRevision   0x00010000

// This protocol is used to abstract Block I/O interfaces.
// Defined in 13.7 Disk I/O Protocol
typedef struct _EfiDiskIoProtocol {
    u64 revision;
    // Reads a specified number of bytes from a device.
    EfiDiskRead read_disk;
    // Writes a specified number of bytes to a device.
    EfiDiskWrite write_disk;
} EfiDiskIoProtocol;

// Disk IO 2 Token
// Defined in 13.8 Disk I/O 2 Protocol
typedef struct {
    EfiEvent event;
    EfiStatus status;
} EfiDiskIo2Token;

struct _EfiDiskIo2Protocol;

// Disk I/O 2 Protocol Function Signatures
// Defined in 13.8 Disk I/O 2 Protocol
typedef EfiStatus (EFIAPI *EfiDiskCancelEx)(struct _EfiDiskIo2Protocol *this);
typedef EfiStatus (EFIAPI *EfiDiskReadEx)(struct _EfiDiskIo2Protocol *this, u32 media_id, u64 offset, EfiDiskIo2Token *token, uptr buffer_size, void *buffer);
typedef EfiStatus (EFIAPI *EfiDiskWriteEx)(struct _EfiDiskIo2Protocol *this, u32 media_id, u64 offset, EfiDiskIo2Token *token, uptr buffer_size, void *buffer);
typedef EfiStatus (EFIAPI *EfiDiskFlushEx)(struct _EfiDiskIo2Protocol *this, EfiDiskIo2Token *token);

// Disk I/O 2 Protocol Revision Number
// Defined in 13.8 Disk I/O 2 Protocol
#define EfiDiskIo2ProtocolRevision  0x00020000

// This protocol is used to abstract Block I/O interfaces in a non-blocking manner.
// Defined in 13.8 Disk I/O 2 Protocol
typedef struct _EfiDiskIo2Protocol {
    u64 revision;
    // Terminate outstanding asynchronous requests to a device.
    EfiDiskCancelEx cancel;
    // Reads a specified number of bytes from a device.
    EfiDiskReadEx read_disk_ex;
    // Writes a specified number of bytes to a device.
    EfiDiskWriteEx write_disk_ex;
    // Flushes all modified data to the physical device.
    EfiDiskFlushEx flush_disk_ex;
} EfiDiskIo2Protocol;

// Block IO Media
// Defined in 13.9 Block I/O Protocol
typedef struct {
    u32 media_id;
    bool removable_media;
    bool media_present;
    bool logical_partition;
    bool read_only;
    bool write_caching;
    u32 block_size;
    u32 io_align;
    EfiLba last_block;
    EfiLba lowest_aligned_lba;  // added in revision 2
    u32 logical_blocks_per_physical_block;  // added in revision 2
    u32 optimal_transfer_length_granularity; // added in revision 3
} EfiBlockIoMedia;

struct _EfiBlockIoProtocol;

// Block I/O Protocol Function Signatures
// Defined in 13.9 Block I/O Protocol
typedef EfiStatus (EFIAPI *EfiBlockReset)(struct _EfiBlockIoProtocol *this, bool extended_verification);
typedef EfiStatus (EFIAPI *EfiBlockRead)(struct _EfiBlockIoProtocol *this, u32 media_id, EfiLba lba, uptr buffer_size, void *buffer);
typedef EfiStatus (EFIAPI *EfiBlockWrite)(struct _EfiBlockIoProtocol *this, u32 media_id, EfiLba lba, uptr buffer_size, void *buffer);
typedef EfiStatus (EFIAPI *EfiBlockFlush)(struct _EfiBlockIoProtocol *this);

// Block I/O Protocol Revision Numbers
// Defined in 13.9 Block I/O Protocol
#define EfiBlockIoProtocolRevision2 0x00020001
#define EfiBlockIoProtocolRevision3 ((2 << 16) | (31))

// This protocol provides control over block devices.
// Defined in 13.9 Block I/O Protocol
typedef struct _EfiBlockIoProtocol {
    u64 revision;
    EfiBlockIoMedia *media;
    // Resets the block device hardware.
    EfiBlockReset reset;
    // Reads the requested number of blocks from the device.
    EfiBlockRead read_blocks;
    // Writes a specified number of blocks to the device.
    EfiBlockWrite write_blocks;
    // Flushes all modified data to a physical block device.
    EfiBlockFlush flush_blocks;
} EfiBlockIoProtocol;

// Block IO Token 2
// Defined in 13.10 Block I/O 2 Protocol
typedef struct {
    EfiEvent event;
    EfiStatus transaction_status;
} EfiBlockIo2Token;

struct _EfiBlockIo2Protocol;

// Block I/O 2 Protocol Function Signatures
// Defined in 13.10 Block I/O 2 Protocol
typedef EfiStatus (EFIAPI *EfiBlockResetEx)(struct _EfiBlockIo2Protocol *this, bool extended_verification);
typedef EfiStatus (EFIAPI *EfiBlockReadEx)(struct _EfiBlockIo2Protocol *this, u32 media_id, EfiLba lba, EfiBlockIo2Token *token, uptr buffer_size, 
    void *buffer);
typedef EfiStatus (EFIAPI *EfiBlockWriteEx)(struct _EfiBlockIo2Protocol *this, u32 media_id, EfiLba lba, EfiBlockIo2Token *token, uptr buffer_size, 
    void *buffer);
typedef EfiStatus (EFIAPI *EfiBlockFlushEx)(struct _EfiBlockIo2Protocol *this, EfiBlockIo2Token *token);

// This protocol provides control over block devices.
// Defined in 13.10 Block I/O 2 Protocol
typedef struct _EfiBlockIo2Protocol {
    EfiBlockIoMedia *media;
    // Resets the block device hardware.
    EfiBlockResetEx reset;
    // Reads the requested number of blocks from the device.
    EfiBlockReadEx read_blocks_ex;
    // Writes a specified number of blocks to the device.
    EfiBlockWriteEx write_blocks_ex;
    // Flushes all modified data to a physical block device.
    EfiBlockFlushEx flush_blocks_ex;
} EfiBlockIo2Protocol;

// Erase Block Token
// Defined in 13.12 Erase Block Protocol
typedef struct {
    EfiEvent event;
    EfiStatus transaction_status;
} EfiEraseBlockToken;

struct _EfiEraseBlockProtocol;

// Erase Block Protocol Function Signatures
// Defined in 13.12 Erase Block Protocol
typedef EfiStatus (EFIAPI *EfiBlockErase)(struct _EfiEraseBlockProtocol *this, u32 media_id, EfiLba lba, EfiEraseBlockToken *token, uptr size);

// Erase Block Protocol Revision Number
// Defined in 13.12 Erase Block Protocol
#define EfiEraseBlockProtocolRevision   ((2 << 16) | (60))

// This protocol provides the ability for a device to expose erase functionality. This optional protocol is installed on the same handle as the 
// EfiBlockIoProtocol or EfiBlockIo2Protocol.
// Defined in 13.12 Erase Block Protocol
typedef struct _EfiEraseBlockProtocol {
    u64 revision;
    u32 erase_length_granularity;
    // Erase a specified number of device blocks.
    EfiBlockErase erase_blocks;
} EfiEraseBlockProtocol;

// ATA Pass Thru Protocol Attributes
// Defined in 13.13 ATA Pass Thru Protocol
#define EfiAtaPassThruAttributesPhysical    0x0001
#define EfiAtaPassThruAttributesLogical     0x0002
#define EfiAtaPassThruAttributeNonBlockIo   0x0002

// ATA Pass Thru Mode
// Defined in 13.13 ATA Pass Thru Protocol
typedef struct {
    u32 attributes;
    u32 io_align;
} EfiAtaPassThruMode;

// ATA Command Block
// Defined in 13.13 ATA Pass Thru Protocol - EFI_ATA_PASS_THRU_PROTOCOL.PassThru()
typedef struct {
    u8 _reserved1[2];
    u8 command;
    u8 features;
    u8 sector_number;
    u8 cylinder_low;
    u8 cylinder_high;
    u8 device_head;
    u8 sector_number_exp;
    u8 cylinder_low_exp;
    u8 cylinder_high_exp;
    u8 features_exp;
    u8 sector_count;
    u8 sector_count_exp;
    u8 _reserved2[6];
} EfiAtaCommandBlock;

// ATA Status Block
// Defined in 13.13 ATA Pass Thru Protocol - EFI_ATA_PASS_THRU_PROTOCOL.PassThru()
typedef struct {
    u8 _reserved1[2];
    u8 status;
    u8 error;
    u8 sector_number;
    u8 cylinder_low;
    u8 cylinder_high;
    u8 device_head;
    u8 sector_number_exp;
    u8 cylinder_low_exp;
    u8 cylinder_high_exp;
    u8 _reserved2;
    u8 sector_count;
    u8 sector_count_exp;
    u8 _reserved3[6];
} EfiAtaStatusBlock;

// Defined in 13.13 ATA Pass Thru Protocol - EFI_ATA_PASS_THRU_PROTOCOL.PassThru()
typedef u8 EfiAtaPassThruCmdProtocol;

// Defined in 13.13 ATA Pass Thru Protocol - EFI_ATA_PASS_THRU_PROTOCOL.PassThru()
#define EfiAtaPassThruProtocolAtaHardwareReset  0x00
#define EfiAtaPassThruProtocolAtaSoftwareReset  0x01
#define EfiAtaPassThruProtocolAtaNonData        0x02
#define EfiAtaPassThruProtocolPioDataIn         0x04
#define EfiAtaPassThruProtocolPioDataOut        0x05
#define EfiAtaPassThruProtocolDma               0x06
#define EfiAtaPassThruProtocolDmaQueued         0x07
#define EfiAtaPassThruProtocolDeviceDiagnostic  0x08
#define EfiAtaPassThruProtocolDeviceReset       0x09
#define EfiAtaPassThruProtocolUdmaDataIn        0x0a
#define EfiAtaPassThruProtocolUdmaDataOut       0x0b
#define EfiAtaPassThruProtocolFpdma             0x0c
#define EfiAtaPassThruProtocolReturnResponse    0xff

// Defined in 13.13 ATA Pass Thru Protocol - EFI_ATA_PASS_THRU_PROTOCOL.PassThru()
typedef u8 EfiAtaPassThruLength;

// Defined in 13.13 ATA Pass Thru Protocol - EFI_ATA_PASS_THRU_PROTOCOL.PassThru()
#define EfiAtaPassThruLengthBytes   0x80

// Defined in 13.13 ATA Pass Thru Protocol - EFI_ATA_PASS_THRU_PROTOCOL.PassThru()
#define EfiAtaPassThruLengthMask            0x70
#define EfiAtaPassThruLengthNoDataTransfer  0x00
#define EfiAtaPassThruLengthFeatures        0x10
#define EfiAtaPassThruLengthSectorCount     0x20
#define EfiAtaPassThruLengthTpsiu           0x30
#define EfiAtaPassThruLengthCount           0x0f

// ATA Pass Thru Command Packet
// Defined in 13.13 ATA Pass Thru Protocol - EFI_ATA_PASS_THRU_PROTOCOL.PassThru()
typedef struct {
    EfiAtaStatusBlock *status_block;
    EfiAtaCommandBlock *command_block;
    u64 timeout;
    void *in_data_buffer;
    void *out_data_buffer;
    u32 in_transfer_length;
    u32 out_transfer_length;
    EfiAtaPassThruCmdProtocol protocol;
    EfiAtaPassThruLength length;
} EfiAtaPassThruCommandPacket;

struct _EfiAtaPassThruProtocol;

// ATA Pass Thru Protocol Function Signatures
// Defined in 13.13 ATA Pass Thru Protocol
typedef EfiStatus (EFIAPI *EfiAtaPassThruPassThru)(struct _EfiAtaPassThruProtocol *this, u16 port, u16 port_multiplier_port,
    EfiAtaPassThruCommandPacket *packet, EfiEvent event);
typedef EfiStatus (EFIAPI *EfiAtaPassThruGetNextPort)(struct _EfiAtaPassThruProtocol *this, u16 *port);
typedef EfiStatus (EFIAPI *EfiAtaPassThruGetNextDevice)(struct _EfiAtaPassThruProtocol *this, u16 port, u16 *port_multipler_port);
typedef EfiStatus (EFIAPI *EfiAtaPassThruBuildDevicePath)(struct _EfiAtaPassThruProtocol *this, u16 port, u16 port_multipler_port,
    EfiDevicePathProtocol **device_path);
typedef EfiStatus (EFIAPI *EfiAtaPassThruGetDevice)(struct _EfiAtaPassThruProtocol *this, EfiDevicePathProtocol *device_path, u16 *port,
    u16 *port_multiplier_port);
typedef EfiStatus (EFIAPI *EfiAtaPassThruResetPort)(struct _EfiAtaPassThruProtocol *this, u16 port);
typedef EfiStatus (EFIAPI *EfiAtaPassThruResetDevice)(struct _EfiAtaPassThruProtocol *this, u16 port, u16 port_multiplier_port);

// Provides services that allow ATA commands to be sent to ATA Devices attached to an ATA controller.
// Defined in 13.13 ATA Pass Thru Protocol
typedef struct _EfiAtaPassThruProtocol {
    EfiAtaPassThruMode *mode;
    // Sends an ATA command to an ATA device that is attached to the ATA controller. This function supports both blocking I/O and non-blocking I/O. The
    // blocking I/O functionality is required, and the non-blocking I/O functionality is optional.
    EfiAtaPassThruPassThru pass_thru;
    // Used to retrieve the list of legal port numbers for ATA devices on an ATA controller. These can either be the list of ports where ATA devices are
    // actually present or the list of legal port numbers for the ATA controller. Regardless, the caller of this function must probe the port number returned
    // to see if an ATA device is actually present at that location on the ATA controller.
    EfiAtaPassThruGetNextPort get_next_port;
    // Used to retrieve the list of legal port multiplier port numbers for ATA devices on a port of an ATA controller. These can either be the list of port
    // multiplier ports where ATA devices are actually present on port or the list of legal port multiplier ports on that port. Regardless, the caller of
    // this function must probe the port number and port multiplier port number returned to see if an ATA device is actually present.
    EfiAtaPassThruGetNextDevice get_next_device;
    // Used to allocate and build a device path node for an ATA device on an ATA controller.
    EfiAtaPassThruBuildDevicePath build_device_path;
    // Used to translate a device path node to a port number and port multiplier port number.
    EfiAtaPassThruGetDevice get_device;
    // Resets a specific port on the ATA controller. This operation also resets all the ATA devices connected to the port.
    EfiAtaPassThruResetPort reset_port;
    // Resets an ATA device that is connected to an ATA controller.
    EfiAtaPassThruResetDevice reset_device;
} EfiAtaPassThruProtocol;

// NVM Express Pass Through Protocol Attributes
// Defined in 13.15 NVM Express Pass Through Protocol
#define EfiNvmExpressPassThruAttributesPhysical     0x0001
#define EfiNvmExpressPassThruAttributesLogical      0x0002
#define EfiNvmExpressPassThruAttributesNonBlockIo   0x0004
#define EfiNvmExpressPassThruAttributesCmdSetNvm    0x0008

// NVM Express Pass Through Mode
// Defined in 13.15 NVM Express Pass Through Protocol
typedef struct {
    u32 attributes;
    u32 io_align;
    u32 nvme_version;
} EfiNvmExpressPassThruMode;

// Fused Operation
// Defined in 13.15 NVM Express Pass Through Protocol
#define NormalCmd       0x00
#define FusedFirstCmd   0x01
#define FusedSecondCmd  0x02

// CDW Flags
// Defined in 13.15 NVM Express Pass Through Protocol
#define Cdw2Valid   0x01
#define Cdw3Valid   0x02
#define Cdw10Valid  0x04
#define Cdw11Valid  0x08
#define Cdw12Valid  0x10
#define Cdw13Valid  0x20
#define Cdw14Valid  0x40
#define Cdw15Valid  0x80

// NVME CDW0
// Defined in 13.15 NVM Express Pass Through Protocol
typedef struct {
    u32 opcode;
    u32 fused_operation;
    u32 _reserved;
} NvmeCdw0;

// NVM Express Command
// Defined in 13.15 NVM Express Pass Through Protocol
typedef struct {
    NvmeCdw0 cdw0;
    u8 flags;
    u32 nsid;
    u32 cdw2;
    u32 cdw3;
    u32 cdw10;
    u32 cdw11;
    u32 cdw12;
    u32 cdw13;
    u32 cdw14;
    u32 cdw15;
} EfiNvmExpressCommand;

// NVM Express Completion
// Defined in 13.15 NVM Express Pass Through Protocol
typedef struct {
    u32 dw0;
    u32 dw1;
    u32 dw2;
    u32 dw3;
} EfiNvmExpressCompletion;

// NVM Express Pass Thru Command Packet
// Defined in 13.15 NVM Express Pass Through Protocol
typedef struct {
    u64 command_timeout;
    void *transfer_buffer;
    u32 transfer_length;
    void *metadata_buffer;
    u32 metadata_length;
    u8 queue_type;
    EfiNvmExpressCommand *nvme_cmd;
    EfiNvmExpressCompletion *nvmd_completion;
} EfiNvmExpressPassThruCommandPacket;

struct _EfiNvmExpressPassThruProtocol;

// NVM Express Pass Through Protocol Function Signatures
// Defined in 13.15 NVM Express Pass Through Protocol
typedef EfiStatus (EFIAPI *EfiNvmExpressPassThruPassThru)(struct _EfiNvmExpressPassThruProtocol *this, u32 namespace_id,
    EfiNvmExpressPassThruCommandPacket *packet, EfiEvent event);
typedef EfiStatus (EFIAPI *EfiNvmExpressPassThruGetNextNamespace)(struct _EfiNvmExpressPassThruProtocol *this, u32 *namespace_id);
typedef EfiStatus (EFIAPI *EfiNvmExpressPassThruBuildDevicePath)(struct _EfiNvmExpressPassThruProtocol *this, u32 namespace_id,
    EfiDevicePathProtocol **device_path);
typedef EfiStatus (EFIAPI *EfiNvmExpressPassThruGetNamespace)(struct _EfiNvmExpressPassThruProtocol *this, EfiDevicePathProtocol *device_path, u32 *namespace_id);

// his protocol provides services that allow NVM Express commands to be sent to an NVM Express controller or to a specific namespace in a NVM Express
// controller. This protocol interface is optimized for storage.
// Defined in 13.15 NVM Express Pass Through Protocol
typedef struct _EfiNvmExpressPassThruProtocol {
    EfiNvmExpressPassThruMode *mode;
    // Sends an NVM Express Command Packet to an NVM Express controller or namespace. This function supports both blocking I/O and non-blocking I/O. The
    // blocking I/O functionality is required, and the non- blocking I/O functionality is optional.
    EfiNvmExpressPassThruPassThru pass_thru;
    // Used to retrieve the next namespace ID for this NVM Express controller.
    EfiNvmExpressPassThruGetNextNamespace get_next_namespace;
    // Used to allocate and build a device path node for an NVM Express namespace on an NVM Express controller.
    EfiNvmExpressPassThruBuildDevicePath build_device_path;
    // Used to translate a device path node to a namespace ID.
    EfiNvmExpressPassThruGetNamespace get_namespace;
} EfiNvmExpressPassThruProtocol;

// Memory Descriptor
// Defined in 7.2 Memory Allocation Services - EFI_BOOT_SERVICES.GetMemoryMap()
typedef struct _EfiMemoryDescriptor {
    u32 type;
    u32 _pad1;
    EfiPhysicalAddress physical_start;
    EfiVirtualAddress virtual_start;
    u64 num_pages;
    u64 attribute;
} EfiMemoryDescriptor;

// Custom type for containing EFI memory map information
typedef struct {
    EfiMemoryDescriptor *descriptors;
    uptr entries;
    uptr descriptor_size;
    uptr key;
    u32 version;
} EfiMemoryMap;

// Memory Allocation Service Function Signatures
// Defined in 7.2 Memory Allocation Services
typedef EfiStatus (*EfiAllocatePages)(EfiAllocateType type, EfiMemoryType memory_type, uptr pages, EfiPhysicalAddress *memory);
typedef EfiStatus (*EfiFreePages)(EfiPhysicalAddress memory, uptr pages);
typedef EfiStatus (*EfiGetMemoryMap)(uptr *memory_map_size, struct _EfiMemoryDescriptor *memory_map, uptr *map_key, uptr *descriptor_size, u32 *descriptor_version);
typedef EfiStatus (*EfiSetWatchdogTimer)(uptr timeout, u64 code, uptr data_size, EfiChar16 *data);

// Boot Services Definitions
// Defined in 4.4 EFI Boot Services Table
#define EfiBootServicesSignature    0x56524553544f4f42
#define EfiBootServicesRevision     EfiSpecificationVersion

// Contains a table header and pointers to all of the boot services.
// Defined in 4.4 EFI Boot Services Table
typedef struct {
    EfiTableHeader header;

    /* 
     * ##########################
     * # Task Priority Services #
     * ##########################
     */

    // Raises a task’s priority level and returns its previous level.
    void *raise_tpl;
    // Restores a task’s priority level to its previous value.
    void *restore_tpl;

    /*
     * ###################
     * # Memory Services #
     * ###################
     */
    
    // Allocates memory pages from the system.
    EfiAllocatePages allocate_pages;
    // Frees memory pages.
    EfiFreePages free_pages;
    // Returns the current memory map.
    EfiGetMemoryMap get_memory_map;
    // Allocates pool memory.
    void *allocate_pool;
    // Returns pool memory to the system.
    void *free_pool;

    /* 
     * ############################
     * # Event and Timer Services #
     * ############################
     */

    // Creates an event.
    void *create_event;
    // Sets the type of timer and the trigger time for a timer event.
    void *set_timer;
    // Stops execution until an event is signaled.
    void *wait_for_event;
    // Signals an event.
    void *signal_event;
    // Closes an event.
    void *close_event;
    // Checks whether an event is in the signaled state.
    void *check_event;

    /*
     * #############################
     * # Protocol Handler Services #
     * #############################
     */

    // Installs a protocol interface on a device handle. If the handle does not exist, it is created and added to the list of handles in the system.
    // install_multiple_protocol_interfaces() performs more error checking than install_protocol_interface(), so it is recommended that
    // install_multiple_protocol_interfaces() be used in place of install_protocol_interface()
    void *install_protocol_interface;
    // Reinstalls a protocol interface on a device handle.
    void *reinstall_protocol_interface;
    // Removes a protocol interface from a device handle. It is recommended that uninstall_multiple_protocol_interfaces() be used in place of
    // uninstall_protocol_interface().
    void *uninstall_protocol_interface;
    // Queries a handle to determine if it supports a specified protocol.
    void *handle_protocol;
    void *_reserved;
    // Creates an event that is to be signaled whenever an interface is installed for a specified protocol.
    void *register_protocol_notify;
    // Returns an array of handles that support a specified protocol.
    void *locate_handle;
    // Locates the handle to a device on the device path that supports the specified protocol.
    void *locate_device_path;
    // Adds, updates, or removes a configuration table entry from the EFI System Table.
    void *install_configuration_table;

    /*
     * ##################
     * # Image Services #
     * ##################
     */

    // Loads an EFI image into memory.
    void *load_image;
    // Transfers control to a loaded image’s entry point.
    void *start_image;
    // Terminates a loaded EFI image and returns control to boot services.
    void *exit;
    // Unloads an image.
    void *unload_image;
    // Terminates all boot services.
    void *exit_boot_services;

    /*
     * ##########################
     * # Miscellaneous Services #
     * ##########################
     */

    // Returns a monotonically increasing count for the platform.
    void *get_next_monotonic_count;
    // Induces a fine-grained stall.
    void *stall;
    // Sets the system’s watchdog timer.
    EfiSetWatchdogTimer set_watchdog_timer;

    /*
     * ###########################
     * # Driver Support Services #
     * ###########################
     */

    // Connects one or more drivers to a controller.
    void *connect_controller;
    // Disconnects one or more drivers from a controller.
    void *disconnect_controller;

    /*
     * ####################################
     * # Open and Close Protocol Services #
     * ####################################
     */

    // Queries a handle to determine if it supports a specified protocol. If the protocol is supported by the handle, it opens the protocol on behalf of the
    // calling agent. This is an extended version of the EFI boot service handle_protocol().
    void *open_protocol;
    // Closes a protocol on a handle that was opened using open_protocol().
    void *close_protocol;
    // Retrieves the list of agents that currently have a protocol interface opened.
    void *open_protocol_information;

    /*
     * ####################
     * # Library Services #
     * ####################
     */

    // Retrieves the list of protocol interface GUIDs that are installed on a handle in a buffer allocated from pool.
    void *protocols_per_handle;
    // Returns an array of handles that support the requested protocol in a buffer allocated from pool.
    void *locate_handle_buffer;
    // Returns the first protocol instance that matches the given protocol.
    void *locate_protocol;
    // Installs one or more protocol interfaces into the boot services environment.
    void *install_multiple_protocol_interfaces;
    // Removes one or more protocol interfaces into the boot services environment.
    void *uninstall_multiple_protocol_interfaces;

    /*
     * ######################
     * # 32bit CRC Services #
     * ######################
     */

    // Computes and returns a 32-bit CRC for a data buffer.
    void *calculate_crc32;

    /*
     * ##########################
     * # Miscellaneous Services #
     * ##########################
     */

    // The copy_mem() function copies the contents of one buffer to another buffer.
    void *copy_mem;
    // The set_mem() function fills a buffer with a specified value.
    void *set_mem;
    void *create_event_ex;
} EfiBootServices;

// System Table Definitions
// Defined in 4.3 EFI System Table
#define EfiSystemTableSignature     0x5453595320494249
#define Efi2_80SystemTableRevision  ((2 << 16) | (80))
#define Efi2_70SystemTableRevision  ((2 << 16) | (70))
#define Efi2_60SystemTableRevision  ((2 << 16) | (60))
#define Efi2_50SystemTableRevision  ((2 << 16) | (50))
#define Efi2_40SystemTableRevision  ((2 << 16) | (40))
#define Efi2_31SystemTableRevision  ((2 << 16) | (31))
#define Efi2_30SystemTableRevision  ((2 << 16) | (30))
#define Efi2_20SystemTableRevision  ((2 << 16) | (20))
#define Efi2_10SystemTableRevision  ((2 << 16) | (10))
#define Efi2_00SystemTableRevision  ((2 << 16) | (00))
#define Efi1_10SystemTableRevision  ((1 << 16) | (10))
#define Efi1_02SystemTableRevision  ((1 << 16) | (02))
#define EfiSpecificationVersion     EfiSystemTableRevision
#define EfiSpecificationRevision    Efi2_80SystemTableRevision

// Contains pointers to the runtime and boot services tables.
// Defined in 4.3 EFI System Table
typedef struct {
	EfiTableHeader header;
	EfiChar16 *firmware_vendor;
	u32 firmware_revision;
	EfiHandle console_in_handle;
	void *console_in;// simple text input pointer
	EfiHandle console_out_handle;
	EfiSimpleTextOutputProtocol *console_out;
	EfiHandle std_error_handle;
	EfiSimpleTextOutputProtocol *std_error;
	void *runtime_services;   // runtime services pointer
	EfiBootServices *boot_services;
	uptr num_table_entries;
	void *config_table;  // config table pointer
} EfiSystemTable;

// This is the main entry point for a UEFI Image.
// Defined in 4.1 UEFI Image Entry Point
typedef EfiStatus (EFIAPI *EfiImageEntryPoint)(EfiHandle handle, EfiSystemTable *st);

#endif
