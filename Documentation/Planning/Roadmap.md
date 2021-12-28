# asOS Roadmap

## bootloader

The bootloader does only two things:
* Enables system features
* Gathers system information to pass to the kernel

### major

- [x] start i686 BIOS
- [ ] start i686 UEFI
- [ ] start x86_64 BIOS
- [ ] start x86_64 UEFI
- [ ] start arm
- [ ] start riscv

### minor

- [x] load and run second bootloader
- [x] enable a20 line
- [x] load gdtr
- [x] setup gdt
- [x] load idtr
- [x] switch to protected mode

## kernel

The first step I want to take in building the kernel is a command line with a few commands:
* output register/memory
* query system features
* system dump over serial/VGA

The step after this will likely be implementing basic processes, but that could change.

### major

- [x] setup stack
- [x] kernel main in C
- [ ] setup interrupt handlers
- [ ] paging
- [ ] command line
- [ ] keyboard input
- [ ] output register command
- [ ] output memory command
- [ ] query system features command
- [ ] system dump over serial command
- [ ] system dump over VGA command

### minor

- [ ] abstract terminal interface
- [ ] implement serial output
- [ ] move cursor when printing string