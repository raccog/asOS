#
#	Makefile for always sunny OS
#
#

boot.bin: boot.o secondboot.o
	dd if=boot.o of=boot.bin bs=512
	dd if=secondboot.o of=boot.bin bs=512 seek=1

boot.o: src/boot.s src/boot_print.s
	nasm src/boot.s -f bin -o boot.o

secondboot.o: src/secondboot.s src/boot_print.s
	nasm src/secondboot.s -f bin -o secondboot.o
