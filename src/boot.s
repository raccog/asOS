;
; Boot program for always sunny OS (asOS)
;

[ORG 0x7c00]    ; offset all memory addresses

; parts of code taken from https://wiki.osdev.org/Babystep4

	xor ax, ax
	mov ds, ax      ; ds = 0
    mov ss, ax      ; ss = 0
    mov sp, 0x9c00  ; 0x2000 past code start

    cld

; clears the screen and then prints a string and a register

	mov ax, 0xb800  ; text video memory location
    mov es, ax

    call clear

    mov byte [ypos], 0
    mov byte [xpos], 0
    mov si, msg
    call sprint

    mov ax, 0xb800
    mov gs, ax
    mov bx, 0x0000
    mov ax, [gs:bx]

    mov word [reg16], 0x5555
    call printreg16

; sets cursor position
; taken from https://wiki.osdev.org/Text_Mode_Cursor

    mov bx, 160
    mov dx, 0x3d4
    mov al, 0x0f
    out dx, al

    inc dl
    mov al, bl
    out dx, al

    dec dl
    mov al, 0x0e
    out dx, al

    inc dl
    mov al, bh
    out dx, al

hang:
    jmp hang

dochar:
    call cprint
sprint:
    lodsb
    cmp al, 0
    jne dochar
    add byte [ypos], 1  ; down one row
    mov byte [xpos], 0  ; to front of the row
    ret

clear:
    mov al, ' '
    mov byte [ypos], 0
.yloop:
    mov byte [xpos], 0
.xloop:
    call cprint
    cmp byte [xpos], 79
    jne .xloop
    add byte [ypos], 1
    cmp byte [ypos], 20
    jne .yloop
    ret

cprint:
    mov ah, 0x0f    ; attribute: white on black text
    mov cx, ax      ; save character and attribute
    movzx ax, byte [ypos]
    mov dx, 160     ; 2 bytes for 80 columns
    mul dx
    movzx bx, byte [xpos]
    shl bx, 1       ; skip attribute

    mov di, 0   ; start of video memory
    add di, ax  ; add y offset
    add di, bx  ; add x offset

    mov ax, cx          ; restore character and attribute
    stosw               ; write character and attribute
    add byte [xpos], 1  ; move 1 column to the right

    ret

printreg16:
    mov di, outstr16
    mov ax, [reg16]
    mov si, hexstr
    mov cx, 4   ; four places
hexloop:
    rol ax, 4
    mov bx, ax
    and bx, 0x0f
    mov bl, [si + bx]
    mov [di], bl
    inc di
    dec cx
    jnz hexloop

    mov si, outstr16
    call sprint

    ret

xpos db 0
ypos db 0
hexstr db '0123456789ABCDEF'
outstr16 db '0000',0
reg16 dw 0
msg db 'Hello asOS', 0

	times 510 - ($-$$) db 0
	db 0x55
	db 0xaa

