#
#	Makefile for always sunny OS
#
#

boot.bin: boot.o boot2.o bootloader.o src/linker.ld
	i686-elf-ld --oformat binary -o boot.bin -T src/linker.ld boot.o boot2.o bootloader.o

boot.o: src/boot.S
	i686-elf-as -o boot.o src/boot.S

boot2.o: src/boot2.S src/a20.S src/vga_print.S
	i686-elf-as -o boot2.o src/boot2.S

bootloader.o: src/bootloader.cpp
	i686-elf-g++ -c src/bootloader.cpp -o bootloader.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
