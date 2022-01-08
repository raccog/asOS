# asOS

asOS (always sunny Operating System) is a hobby OS project with a goal of creating a minimal command line userland for arm systems.

My underlying goals for this project are to:
* Learn about hardware features, standards, and concepts for modern architectures (aarch64, aarch32, riscv)
* Learn a multitude of design techniques for a simple OS kernel with command line userspace.
* Learn technicques on how to design an OS for use with various architectures and hardware.
* Learn about tools such as compilers, linkers, emulators, debuggers, etc.
Also learn more about tools I already use such as make and git.
* Learn about formatting standards such as file systems, executable formats (elf), etc.
* Have fun.

## Plan

Currently the only supported architecture is aarch64.
When building the operating system, a board (raspberry pi 3, 4) should be chosen to include certain features at compile time.

The focus of the first major version of asOS is to include an extensive library for debugging the kernel.
Text output can be sent through various protocols (uart, i2c, display, hdmi) so devices can be debugged easily.

The kernel is written in C++ and its design will consistently change as I read more about design techniques.
For now, only a single core will run with no multithreading, but multithreading and multiple cores will be implemented in the future.

## Current Major Goal

The current major goal of this OS is to get a command line running that has a few commands for viewing registers/memory/system info.

There will be minimal features alongside this command line.
Interrupts are needed to interect with a keyboard.
Memory management and paging are not needed, but I may want to implement them soon.

## Directory Structure
### TODO: Update for aarch64

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
