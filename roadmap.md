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
- [ ] load idtr
- [ ] setup idt
- [x] switch to protected mode
- [ ] setup stack
- [ ] bootloader main in C

## kernel

### major

- [ ] kernel main in C
- [ ] command line
- [ ] output register command
- [ ] output memory command
- [ ] query system features command
- [ ] system dump over serial command
