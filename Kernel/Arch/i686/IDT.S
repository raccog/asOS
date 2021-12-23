.intel_syntax noprefix

.section .text
.code32

.include "ArchLib/i686/Locations.inc.s"

.global setupIDT
.type setupIDT, @function

setupIDT:
    # the start of IDT
    mov edi, IDT

    # divide by zero interrupt handler
    mov esi, offset divide_by_zero

    # offset
    mov [edi], si       # offset 0-15
    rol esi, 16
    mov [edi + 6], si   # offset 16-31

    # segment selector
    xor ax, ax
    mov ax, 1         # segment descriptor index
    shl ax, 3
    mov [edi + 2], ax

    # gate type
    xor ax, ax
    mov ax, 0xe00
    # present bit
    or ax, 0x8000
    mov [edi + 4], ax

    mov esi, offset SET_IDT_MSG
    call sprint

    ret

divide_by_zero:
    mov esi, offset MSG
    call sprint
    iret

MSG:            .asciz "Divide by zero error"
SET_IDT_MSG:    .asciz "Set IDT"
