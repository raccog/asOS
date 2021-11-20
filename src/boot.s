;
;   Bootloader for always sunny OS (asOS)
;
;   Currently prints a message to VGA text memory and moves the cursor before hanging.
;
; parts of code taken from https://wiki.osdev.org/Babystep4

[ORG 0x7c00]    ; offset all memory addresses
[bits 16]

    jmp _start

%include "src/boot_print.s"

global _start
_start:

    ; set segment locations
	xor ax, ax
	mov ds, ax      ; ds = 0
    mov ss, ax      ; ss = 0
    mov sp, 0x9c00  ; 0x2000 past code start

    ; clear direction and interrupt flags
    cld
    cli


; clears the screen and then prints a string and a register value

	mov ax, 0xb800  ; text video memory location
    mov es, ax

    call clear      ; clear the VGA screen

    mov byte [ypos], 0  ; y = 0
    mov byte [xpos], 0  ; x = 0
    mov si, msg
    call sprint         ; print string

    mov ax, 0xb800  ; text video memory location
    mov gs, ax      
    mov bx, 0x0000  ; bx = 0
    mov ax, [gs:bx] ; read 2 bytes from VGA text memory

    mov word [reg16], ax    ; reg16 = ax
    call printreg16         ; print reg16

; sets the cursor position
    mov ax, 2
    mov bx, 0
    call movecursor

hang:
    jmp hang


msg db 'Hello asOS', 0

	times 510 - ($-$$) db 0
	dw 0xaa55

