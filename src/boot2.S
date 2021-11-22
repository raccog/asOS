/*
 * Second bootloader for always sunny OS (asOS)
 */
.code16
.intel_syntax noprefix

.section .text

_second_boot_entry:
	mov al, '!'
	mov ah, 0x0e
	mov bh, 0x0
	mov bl, 0x0f
	int 0x10
	hlt

