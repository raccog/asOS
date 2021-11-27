#
#	Makefile for always sunny OS
#
#

boot.bin: boot.o boot2.o bootloader.o pmode.o src/linker.ld
	i686-elf-ld --oformat binary -o boot.bin -T src/linker.ld boot.o boot2.o pmode.o bootloader.o

boot.o: src/boot.S
	i686-elf-as -o boot.o src/boot.S

boot2.o: src/boot2.S src/a20.S src/vga_print_rmode.S
	i686-elf-as -o boot2.o src/boot2.S

pmode.o: src/pmode.S src/vga_print_pmode.S
	i686-elf-as -o pmode.o src/pmode.S

bootloader.o: src/bootloader.cpp
	i686-elf-g++ -c src/bootloader.cpp -o bootloader.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
