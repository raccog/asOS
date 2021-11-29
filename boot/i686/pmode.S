/*
 * Second bootloader in protected mode for always sunny OS (asOS)
 */
.intel_syntax noprefix

.equ stack, 0x6e00

.section .text

.code32

# string constants
MSG_PMODE:		.asciz "Entered into protected mode."

# include procedures
.include "src/vga_print_pmode.S"

.code32
.global begin_pmode
begin_pmode:
	/* set segment registers */
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax

	/* set stack */
	mov esp, stack

	mov byte ptr [ypos], 5	# manually set row to 5
	mov esi, offset MSG_PMODE
	call sprint

	/* call C hook */
	#call bootloader_main

	hlt

