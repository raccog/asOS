#!/bin/sh
clang -target i686-elf -ffreestanding -nostdlib -nostdinc -O2 -o boot.S.o -c boot/arch/i686/boot.S
clang -target i686-elf -ffreestanding -nostdlib -O2 -Wl,--oformat=binary -o os.bin boot.S.o
