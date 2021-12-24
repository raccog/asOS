# asOS

asOS (always sunny Operating System) is a hobby OS project with a goal of creating a minimal command line userland for desktop systems.

My underlying goals for this project are to:
* Learn about hardware features, standards, and concepts for a variety of architectures (x86, arm, and riscv).
* Learn a multitude of design techniques for a simple OS kernel with command line userspace.
* Learn technicques on how to design an OS for use with various architectures and hardware.
* Learn about tools such as compilers, linkers, emulators, debuggers, etc.
Also learn more about tools I already use such as make and git.
* Learn about formatting standards such as file systems, executable formats (elf), etc.
* Have fun.

## Plan

asOS is going to contain a kernel and a bootloader.
Each supported architecture will have its own custom bootloader that will pass information about the system to the kernel. 
There is also a common library called ArchLib that is linked to both the kernel and bootloader.

In the far future, there may be a GUI/window manager, but for now the goal for user interface is just a terminal. 

## Current Major Goal

The current major goal of this OS is to get a command line running that has a few commands for viewing registers/memory/system info.

There will be minimal OS features implemented before this command line can be implemented.
Interrupts are needed to interect with the keyboard.
Memory management and paging are not needed, but I may want to implement them soon.

This command line will run in the kernel and will read system information passed by the bootloader.

## Directory Structure

Each directory in the main project directory contains a seprarte sub-project.
Some examples of these sub-projects are:
* Bootloader
* Kernel
* ArchLib
* Documentaion
* Scripts
* Userland

Each sub-project is important enough to be contained separately from each other sub-project.
Sub-project directories each contain a Readme.md file that describes its purpose and structure.

## Supported Architectures

As the kernel of asOS relies on the custom bootloader, the bootloader needs to be implemented separetely for each supported architecture. 

Currently, the only bootloader is for i686 BIOS, but others are planned to be supported in the future. 
In the current phase of this project, I am only working on i686 bios.
Once I get a minimal command line running, I am going to work on other architectures.

The following list includes all architectures that I want to support:
* i686 BIOS
* i686 UEFI
* x86_64 UEFI
* arm (32-bit raspberry pi)
* riscv (32-bit)
