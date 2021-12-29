.intel_syntax noprefix

.section .text
.code32

.global isr0
.type isr0, @function

isr0:
    mov esi, offset MSG
    call sprint
    iret

MSG:    .asciz "Divide by zero error"
