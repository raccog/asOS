/*
 * Bootloader for always sunny OS (asOS)
 */
.code16
.intel_syntax noprefix

# special locations
.equ first_bootloader, 0x7c00
.equ second_bootloader, 0x7000
.equ stack, 0x6e00

# constants
.equ sector_size, 0x200

.section .text
.global _boot_entry1
_boot_entry1:
	jmp _boot1_code

hexstr:		.ascii "0123456789ABCDEF"
outstr16:	.asciz "0x0000"
reg16:		.word 0

MSG_BOOT1:			.asciz "Running first bootloader"
MSG_BOOT2_READ:		.asciz "Second bootloader read from disk"
MSG_BOOT2_ERROR:	.asciz "Error while reading second bootloader from disk"

_boot1_code:
	/* set data and extra segment locations */
	xor ax, ax
	mov ds, ax
	mov es, ax

	/* set stack segment and pointer */
	cli		# disable interrupts
	xor ax, ax
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

	mov ah, 0x00
	int 0x13

	/* load second bootloader */
	mov bx, second_bootloader
	mov ah, 0x02	# read disk sectors
	mov al, 0x02	# read 2 sectors
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

	mov si, offset MSG_BOOT2_READ
	call sprint

	jmp 0x0000:second_bootloader

	hlt

bootloader_read_error:
	mov si, offset MSG_BOOT2_ERROR
	call sprint

	hlt

/* prints a character to tty
 * inputs: al = character 
 */
cprint:
	mov ah, 0x0e
	mov bh, 0x0
	mov bl, 0x0f
	int 0x10
	ret

/* prints a string located at si to tty
 * inputs: si = string location
 */
sprint:
	lodsb			# al = *si
	cmp al, 0		# if (al == 0)
	je sprint_done	# return
	call cprint		# print character in al
	jmp sprint		# restart loop
sprint_done:
	call nlprint
	ret

/* prints a new line to tty */
nlprint:
	mov al, 13
	call cprint
	mov al, 10
	call cprint
	ret
	

/* prints the value of reg16 to tty as a 16-bit hex value
 * inputs: reg16 = value
 *
 * modified from https://wiki.osdev.org/Babystep5
 */
printreg16:
	mov di, offset outstr16 + 2
	mov ax, [reg16]
	mov si, offset hexstr
	mov cx, 4
printreg16_loop:
	rol ax, 4
	mov bx, ax
	and bx, 0x0f
	mov bl, [si + bx]
	mov [di], bl
	inc di
	dec cx
	jnz printreg16_loop

	mov si, offset outstr16
	call sprint
	
	ret

.fill 510 - (. - _boot_entry1), 1, 0
.word 0xaa55
