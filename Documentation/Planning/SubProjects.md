# Sub-Projects

asOS is composed of a set of sub-projects that are all contained in this repository.
These sub-projects are described in this document.

Each header in this document describes a directory in this repository.

## Descriptions

### Boot

The special bootloader for this operating system.

#### Dependencies
* ArchLib

### Kernel

The main kernel of this operating system.

#### Dependencies
* ArchLib
* StdLib

### ArchLib

Common assembly procedures that are used in multiple other sub-projects.

#### Dependencies
NONE

### StdLib

The standard library.

#### Dependencies
NONE

## Final Image Layout

The memory layout of the final operating system image for each supported architecture.

### i686

```
-----------------------------------------
|                           0x00000000  |
|                                       |
|   Bootloader                          |
|                                       |
|                           0x00000dff  |
-----------------------------------------
|                           0x00000e00  |
|                                       |
|   Kernel                              |
|                                       |
|                           0x00004dff  |
-----------------------------------------
```

