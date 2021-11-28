# asOS

asOS (always sunny Operating System) is a hobby OS project with the goal of creating a minimal kernel for desktop and mobile computers.

## Plan

asOS is going to be split into a kernel and a bootloader. Each supported architecture will have its own custom bootloader that will pass information about the system to the kernel. 

In the far future, there will be a GUI/window manager, but for now the goal for user interface is just a terminal. 

## Current Major Goal

The current major goal of this OS is to get a command line running that has a few commands for dumping system information. 

System information should be shown on the monitor when short enough, and dumped to serial output when long.

## Supported Architectures

As the kernel of asOS relies on the custom bootloader, the bootloader needs to be updated for each supported architecture. 

Currently, the only bootloader is for i386 BIOS, but others are planned to be supported in the future. 

The following list includes all architectures that I want to support:
* i386 BIOS
* i386 UEFI
* x86_64 BIOS
* x86_64 UEFI
* armv6 (32-bit raspberry pi)
* riscv (32-bit)
