#
#	Makefile for always sunny OS
#
#

boot.bin: src/boot.s
	nasm src/boot.s -f bin -o boot.bin
