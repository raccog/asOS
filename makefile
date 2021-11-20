#
#	Makefile for always sunny OS
#
#

boot.bin: src/boot.s src/boot_print.s
	nasm src/boot.s -f bin -o boot.bin
