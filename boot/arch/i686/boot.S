.intel_syntax noprefix
.code16

.section .text
.global _start
.type _start, @function

_start:
    mov ax, 0x07c0
    mov ds, ax

    mov si, msg
    cld

chloop:
    lodsb
    or al, al
    jz hang
    mov ah, 0x0e
    mov bh, 0
    int 0x10
    jmp chloop

hang:
    jmp hang

msg:    .asciz "Hello World"

.fill 510-(. - _start), 1, 0
.word 0xaa55
