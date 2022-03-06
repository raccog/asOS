# Bootloader

The bootloader is described in this document, both in its current form, along with any planned features.

## Implementations

Currently, the bootloader is only implemented for x86_64 systems running UEFI firmware.

In the future, I want to support other architectures such as arm and riscv.
The focus will be on modern hardware and firmware, so firmware such as BIOS is not planned on being supported.
However, modern 32bit systems may be supported.

## Entry Point

The UEFI bootloader's entry point is contained in `boot/efi/entry.c`.

The entry point's signature is:
```c
EfiStatus EFIAPI efi_main(EfiHandle handle, EfiSystemTable *st);
```

When new architectures/firmware are supported, their entry points will also be contained in subdirectories under the `boot` directory.