#
#	Makefile for always sunny OS
#
#

boot.bin: boot.o boot2.o src/linker.ld
	i686-elf-ld --oformat binary -o boot.bin -T src/linker.ld boot.o boot2.o

boot.o: src/boot.S
	i686-elf-as -o boot.o src/boot.S

boot2.o: src/boot2.S
	i686-elf-as -o boot2.o src/boot2.S
