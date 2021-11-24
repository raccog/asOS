/*
 * Second bootloader for always sunny OS (asOS)
 */
.code16
.intel_syntax noprefix

.section .text

_second_boot_entry:
	jmp _boot2_code

idtr:
	.word 8

gdtr:
	.word gdt_start - gdt_end	# size = 4 entries * 8 bytes each
	.int gdt_start				# offset = address of GDT

gdt_start:
# null descriptor
	.long 0
# code descriptor
	.word 0xffff		# limit 0-15
	.word 0				# base 0-15
	.byte 0				# base 16-23
	.byte 0b10011010	# access byte
	.byte 0b11001111	# flags and limit
	.byte 0				# base 24-31
# data descriptor
	.word 0xffff		# limit 0-15
	.word 0				# base 0-15
	.byte 0				# base 16-23
	.byte 0b10010010	# access byte
	.byte 0b11001111	# flags and limit
	.byte 0				# base 24-31
# empty TSS descriptor
	.long 0
gdt_end:

MSG_SET_GDT:	.asciz "Set the GDT."

.include "src/a20.S"
.include "src/vga_print.S"

_boot2_code:
	cld	# clear direction flag

	mov byte ptr [ypos], 2	# manually set row
	call enable_a20			# enable a20 line
	
	/* load GDT */
	lgdt [gdtr]

	mov si, offset MSG_SET_GDT
	call sprint

	hlt
