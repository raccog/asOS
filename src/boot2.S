/*
 * Second bootloader for always sunny OS (asOS)
 */
.code16
.intel_syntax noprefix

.equ gdt_addr, 0x6f00

.section .text

_second_boot_entry:
	cld	# clear direction flag

	mov byte ptr [ypos], 2	# manually set row
	call enable_a20			# enable a20 line
	
	/* set GDT */
	lgdt [gdtr]

	mov si, offset MSG_SET_GDT
	call sprint

	hlt

gdtr:
	.word 32		# size = 4 entries * 8 bytes each
	.int gdt_addr	# offset = address of GDT

MSG_SET_GDT:	.asciz "Set the GDT."

.include "src/a20.S"
.include "src/vga_print.S"
