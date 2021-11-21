#
#	Makefile for always sunny OS
#
#

boot.bin: boot.o
	i686-elf-ld --oformat binary -o boot.bin -Ttext 0x7c00 -e _start boot.o

boot.o: src/boot.S
	i686-elf-as -o boot.o src/boot.S
