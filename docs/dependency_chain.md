# Code Dependency Chain

This document describes each executable that is created during the build process and the dependencies for each.

## Executables and Libraries

An executable describes a binary file that will be loaded either by the OS or a part of the bootloader.

Common libraries describe object archive files that have not been linked yet.
These libraries are to be statically included in executables that depend on them.
For this reason, there is not really any dependencies between libraries; only executables have dependencies.

However, some libraries might depend on symbols in other libraries, so they will need to be linked together.
For example, the Stdlib will depend on a file system library in order to provide procedures for interacting with files.
Thus, the kernel and bootloader both need to be linked to the Stdlib and a file system library in order to read files.

## Executables

### Bootloader

Currently, the only supported option is UEFI on x86_64 systems, but other options such as raspberry pis may be supported in the future.

Every library and executable listed under the bootloader will be freed from memory when the kernel is finished loading.

Dependencies:

* Archlib
* Stdlib
* Hardwarelib
* Bootlib

### Kernel

The kernel is where all the magic happens.

There is no kernel implementation yet, so this dependency chain is to plan for the future.

Dependencies:

* Archlib
* Stdlib
* Hardwarelib
* Kernellib

### Userspace Programs

Eventually, the kernel will be able to run programs in userspace, but this won't be for a while.

Dependencies:

* Unknown for now

## Common Libraries

Common libraries can be statically compiled into the bootloader, kernel, or userspace executables.

### Archlib

Archlib contains general procedures for each architecture.

### Stdlib

The stdlib contains general procedures and data structures to be used anywhere in the codebase.

### HardwareLib

Hardwarelib contains functions and data structures to interact with hardware.
Usually used in bootloader and kernel, but not userspace programs.

### Executable-Specific Libraries

### Bootlib

Bootlib is not specific to any firmware or architecture and contains general-purpose functions and data that can be used by all firmware and architectures.

### Kernellib

Data structures and functions that are only used in the kernel.
