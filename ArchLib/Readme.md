# ArchLib

ArchLib is a static library that is linked to the bootloader and kernel executables.

## Purpose

ArchLib generally contains assembly procedures that are useful for both the bootloader and kernel.
Any shared assembly procedures should be moved here so that they are not duplicated among the bootloader and kernel.

As this library is linked in the 3k bootloader, only procedures that are absolutely necessary for the bootloader to have should be included.
