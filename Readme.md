# asOS

asOS (always sunny Operating System) is a hobby OS project with a goal of creating a command line for running processes

My underlying goals for this project are to:
* Learn about hardware features, standards, and concepts for multiple architectures.
* Learn a multitude of design techniques for a simple kernel.
* Learn techniques on how to design an OS for use with various architectures and hardware.
* Learn about tools such as compilers, linkers, emulators, debuggers, etc.
Also learn more about tools I already use such as make and git.
* Learn about formatting standards such as file systems, executable formats (elf), etc.
* Have fun.

## Plan

The focus of the first major version of asOS is to include a library for debugging the kernel.
Text output can be sent through various protocols (uart, i2c, vga, hdmi) so devices can be debugged easily.

The kernel is written in C and its design will consistently change as I read more about design techniques.
For now, only a single core will run with no multithreading, but multithreading and multiple cores will be implemented in the future.

## Current Major Goal

The current major goal of this OS is to get a command line running that has a few builtin commands, mostly for debugging purposes.

There will be minimal features alongside this command line (no filesystem, no multithreading, etc.).
Interrupts are needed to interect with a keyboard.
Memory management and paging are not needed, but I may want to implement them soon.
