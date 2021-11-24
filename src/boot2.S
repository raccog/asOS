/*
 * Second bootloader for always sunny OS (asOS)
 */
.code16
.intel_syntax noprefix

.section .text

_second_boot_entry:
	cld	# clear direction flag

	mov byte ptr [ypos], 2	# manually set row
	call enable_a20			# enable a20 line

	hlt

.include "src/a20.S"
.include "src/vga_print.S"
