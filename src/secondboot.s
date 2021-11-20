[bits 16]

	jmp secondboot

%include "src/boot_print.s"

secondboot:
	mov ax, cs
	mov ds, ax	; ds = cs

	mov ax, 0xb800
	mov es, ax

	call clear

	mov byte [xpos], 0
	mov byte [ypos], 0
	mov si, msg
	call sprint

hang:
	jmp hang

msg db 'Hello second bootloader', 0

