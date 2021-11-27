/*
 * Second bootloader for always sunny OS (asOS)
 */
.intel_syntax noprefix

.section .text

.code16		# 16-bit code

/* jump past data */
_second_boot_entry:
	jmp _boot2_code

/* interrupt table descriptor register */
#idtr:
	#.word 8

/* global table descriptor register */
gdtr:
	.word gdt_start - gdt_end	# size = 4 entries * 8 bytes each
	.int gdt_start				# offset = address of GDT

/* global descriptor table */
.align 8	# align GDT to 8 byte boundary
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

# string constants
MSG_SET_GDT:	.asciz "Set the GDT."

# include procedures
.include "src/a20.S"
.include "src/vga_print.S"

/* entry point */
_boot2_code:
	cld	# clear direction flag

	/* enable a20 line */
	mov byte ptr [ypos], 2	# manually set row
	call enable_a20			# enable a20 line
	
	/* load GDT */
	lgdt [gdtr]

	/* print GDT load success message */
	mov si, offset MSG_SET_GDT
	call sprint

	/* run C hook */
	call bootloader_main

	hlt
