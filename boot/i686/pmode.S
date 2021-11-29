/*
 * Second bootloader in protected mode for always sunny OS (asOS)
 */
.intel_syntax noprefix

.equ stack, 0x6e00

.section .text

.code32

# string constants
MSG_PMODE:		.asciz "Running second bootloader in protected mode."

# include procedures
.include "boot/i686/vga_print_pmode.S"

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

	/* print init message */
	mov esi, offset MSG_PMODE
	call sprint

	hlt

