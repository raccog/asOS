/*
 * Bootloader for always sunny OS (asOS)
 */
.code16
.intel_syntax noprefix

.section .text
.global _start
_start:
	/* set data and extra segment locations */
	xor ax, ax
	mov ds, ax	# ds = 0x0000
	mov es, ax	# es = 0x0000

	/* set stack segment and pointer 
	 * top of the stack is at 0x9c00 = 0x7c00 + 0x2000
	 */
	cli		# disable interrupts
	xor ax, ax
	mov ss, ax		# ss = 0x0000	
	mov sp, 0x2000	# sp = 0x2000

	/* clear the screen and set VGA video mode to 25x80 tty */
	mov ah, 0x00
	mov al, 0x03
	int 0x10

	/* print init message */
	mov si, offset msg
	call sprint

	/* print a register value */
	mov bx, offset reg16
	mov word ptr [bx], 0xffee
	call printreg16

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

msg:		.asciz "Running first bootloader"
hexstr:		.ascii "0123456789ABCDEF"
outstr16:	.asciz "0x0000"
reg16:		.word 0

.fill 510 - (. - _start), 1, 0
.word 0xaa55
