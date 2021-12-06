#
#	Makefile for always sunny OS
#
#

boot.bin: boot.o boot2.o pmode.o kboot.o boot/i686/linker.ld
	i686-elf-ld --oformat binary -o boot.bin -T boot/i686/linker.ld boot.o boot2.o pmode.o kboot.o

boot.o: boot/i686/boot.S
	i686-elf-as -o boot.o boot/i686/boot.S

boot2.o: boot/i686/boot2.S boot/i686/a20.S boot/i686/vga_print_rmode.S
	i686-elf-as -o boot2.o boot/i686/boot2.S

pmode.o: boot/i686/pmode.S boot/i686/vga_print_pmode.S
	i686-elf-as -o pmode.o boot/i686/pmode.S

kboot.o: kernel/arch/i686/boot.S
	i686-elf-as -o kboot.o kernel/arch/i686/boot.S
