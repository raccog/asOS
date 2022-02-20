# Always Sunny OS

asOS (Always Sunny Operating System) is my solo hobby OS project (though feedback is always welcome :D).

Currently, I am working on the custom UEFI bootloader.

## Build Instructions

To build all libraries and executables, run ```make all``` from the main directory.
Make sure you have all dependencies (listed below) installed before building.

To build and run the bootloader, run ```make run```.

### Dependencies

- qemu-system-x86_64
- clang
- make
- lld (llvm linker)
- llvm-ar (llvm archiver)
- wget
- essential commands (mkdir, mv, cp)

## Purpose

Starting with basic processes (after the bootloader), I am working through parts of an operating system as I learn about them.
As I am learning along the way, it will likely take years to achieve my goals; and this is not a bad thing.
This project is for me to work on for fun, and it is by design that everything will take very long to implement.

My underlying goals for this project are to:
* Learn about hardware features, standards, and concepts for multiple modern architectures (x64, aarch32, aarch64, riscv32).
* Learn a multitude of design techniques for large projects.
* Learn techniques on how to design an OS for use with various architectures and hardware.
* Learn about tools such as compilers, linkers, emulators, debuggers, etc.
Also learn more about tools I already use such as make and git.
* Learn about standards such as file systems, executable formats (elf), posix compatibility, etc.
* My main goal is to have fun.
