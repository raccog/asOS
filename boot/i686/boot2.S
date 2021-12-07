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
idtr:
	.word idt_end - idt_start - 1   # size = 256 entries * 8 bytes each
    .long idt_start                 # offset = address of IDT

.align 8    # align IDT to 8 byte boundary
idt_start:
    .fill 256, 8, 0     # fill 256 entries with address 0x00000000
idt_end:

/* global table descriptor register */
gdtr:
	.word gdt_end - gdt_start - 1	# size = 4 entries * 8 bytes each
	.long gdt_start					# offset = address of GDT

/* global descriptor table */
.align 8	# align GDT to 8 byte boundary
gdt_start:
# null descriptor
	.long 0
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
	.long 0
gdt_end:

# string constants
MSG_BOOT2:	.asciz "Running second bootloader."

# include procedures
.include "boot/i686/a20.S"
.include "boot/i686/vga_print_rmode.S"

/* entry point */
_boot2_code:
	cld	# clear direction flag
	cli	# clear interrupt flag

	/* print init message */
	mov esi, offset MSG_BOOT2
	call sprint

	/* enable a20 line */
	call enable_a20			# enable a20 line
	
	/* load GDT */
	lgdt [gdtr]

    /* load IDT */
    lidt [idtr]

	/* enter protected mode */
	mov eax, cr0
	or al, 1
	mov cr0, eax

	/* clear instruction pipeline */
	jmp 0x8:begin_pmode

	hlt
