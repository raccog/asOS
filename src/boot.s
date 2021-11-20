;
; Boot program for always sunny OS (asOS)
;
%macro BiosPrint 1
mov si, word %1
call ch_loop
%endmacro

[ORG 0x7c00]

	xor ax, ax
	mov ds, ax
    cld

	BiosPrint msg
	BiosPrint msg

hang:
	jmp hang

ch_loop:
    lodsb
    or al, al
    jz done
    mov ah, 0x0e
    int 0x10
    jmp ch_loop
done:
    ret

msg db 'Hello asOS', 13, 10, 0

	times 510 - ($-$$) db 0
	db 0x55
	db 0xaa

