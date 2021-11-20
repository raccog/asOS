;
; Bootloader for always sunny OS (asOS)
;
; Currently prints a message to VGA text memory and moves the cursor before hanging.
;
; parts of code taken from https://wiki.osdev.org/Babystep4

[ORG 0x7c00]	; offset all memory addresses
[bits 16]

	jmp _start

%include "src/boot_print.s"

global _start
_start:

	; set segment locations
	xor ax, ax
	mov ds, ax	; ds = 0

	cli			; disable interrupts
	mov ss, ax	; ss = 0
	mov sp, 0x9c00; 0x2000 past code start

	cld			; set direction flag to be positive

	push dx		; save floppy drive number


; clears the screen and then prints a string and a register value

	mov ax, 0xb800	; text video memory location
	mov es, ax

	call clear		; clear the VGA screen

	mov byte [ypos], 0	; y = 0
	mov byte [xpos], 0	; x = 0
	mov si, msg
	call sprint			; print string

	mov ax, 0xb800		; text video memory location
	mov gs, ax	
	mov bx, 0x0000; bx = 0
	mov ax, [gs:bx]		; read 2 bytes from VGA text memory

	mov word [reg16], ax	; reg16 = ax
	call printreg16			; print reg16

; sets the cursor position
	mov ax, 2
	mov bx, 0
	call movecursor

resetfloppy:
	sti			; enable interrupts
	pop dx		; restore floppy drive number
	xor ax, ax	; 0 = reset floppy
	int 0x13
	jc resetfloppy	; try reset again if failed

	push dx		; preserve floppy drive number
	mov si, resetmsg
	call sprint

	mov ax, 0x07e0	; read to address 0x07e0:0x0000
	mov es, ax

readfloppy:
	xor bx, bx	; set buffer offset to 0
	mov ah, 0x2	; 2 = read from floppy
	mov al, 0x1	; read 1 sector
	mov ch, 0x0	; track 1
	mov cl, 0x2	; sector 2
	mov dh, 0x0	; head 1
	pop dx		; restore floppy drive number
	int 0x13
	jc readfloppy	; try read again if failed

	mov ax, 0xb800
	mov es, ax
	mov si, readmsg
	call sprint

	jmp 0x07e0:0x0000	; far jump to second bootloader


msg db 'Hello first bootloader', 0
resetmsg db 'Floppy reset', 0
readmsg db 'Second bootloader read', 0

	times 510 - ($-$$) db 0
	dw 0xaa55

