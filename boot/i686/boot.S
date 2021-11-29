/*
 * Bootloader for always sunny OS (asOS)
 */

.intel_syntax noprefix

# special locations
.equ first_bootloader, 0x7c00
.equ second_bootloader, 0x7000
.equ stack, 0x6e00

# constants
.equ sector_size, 0x200

.section .text
.global _boot_entry1	# export entry point to linker

/* jump past data */
_boot_entry1:
	jmp _boot1_code

# string buffers
outstr16:	.asciz "0x0000"
reg16:		.word 0

# string constants
hexstr:		.ascii "0123456789ABCDEF"
MSG_BOOT1:			.asciz "Running first bootloader"
MSG_BOOT2_ERROR:	.asciz "Error while reading second bootloader from disk"

.code16		# 16-bit code 
_boot1_code:
	/* set segment locations */
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	/* set stack segment and pointer */
	cli		# disable interrupts
	mov ss, ax	
	mov sp, stack	# set stack

	/* save boot disk number for later */
	push dx

	/* clear the screen and set VGA video mode to 25x80 tty */
	mov ah, 0x00
	mov al, 0x03
	int 0x10

	/* print init message */
	mov si, offset MSG_BOOT1
	call sprint

	/* reset disk */
	mov ah, 0x00	# reset disk
	int 0x13

	/* load second bootloader */
	mov bx, second_bootloader
	mov ah, 0x02	# read disk sectors
	mov al, 0x06	# read 6 sectors (3K)
	mov ch, 0x00	# cylinder 0
	mov cl, 0x02	# sector 2
	pop dx			# restore drive number
	mov dh, 0x00	# drive head 0
	int 0x13
	jb bootloader_read_error	# error if CL==1
	or ah, ah
	jnz bootloader_read_error	# error if ah!=0
	or al, al
	jz bootloader_read_error	# error if al==0

	/* jump to second bootloader */
	jmp 0x0000:second_bootloader

	hlt

bootloader_read_error:
	/* print bootloader read error message */
	mov si, offset MSG_BOOT2_ERROR
	call sprint

	hlt

/* prints a character to tty
 * inputs: al = character 
 */
cprint:
	mov ah, 0x0e	# write character to teletype
	mov bh, 0x0		# page 0
	mov bl, 0x0f	# white on black attribute
	int 0x10
	ret

/* prints a string located at ds:si to tty
 * inputs: si = string location
 */
sprint:
	lodsb			# load byte from ds:si
	cmp al, 0		# if (al == 0)
	je sprint_done	# return
	call cprint		# print character in al
	jmp sprint		# restart loop
sprint_done:
	call nlprint	# print new line
	ret

/* prints a new line to tty */
nlprint:
	mov al, 13 
	call cprint
	mov al, 10
	call cprint
	ret

/* fill with zeros until MBR signature */
.fill 510 - (. - _boot_entry1), 1, 0
.word 0xaa55	# MBR signature
