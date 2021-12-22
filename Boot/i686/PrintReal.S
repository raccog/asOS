/*
 *  Bootloader print functions for real mode
 *
 */

.intel_syntax noprefix

.section .data

# string constants
hexstr:         .ascii "0123456789ABCDEF"

# data
outstr16:   .asciz "0x0000"
xpos:       .byte 0
ypos:       .byte 0

.section .text
.code16

# global procedures
.global cprint_real
.global sprintln_real

# Prints an ascii character to the screen using BIOS.
#
# Inputs:
#   al = character
cprint_real:
    mov ah, 0x0e    # write character to teletype
    mov bh, 0x0     # page 0
    mov bl, 0x0f    # white on black attribute
    int 0x10
    ret

# Prints a string of ascii characters until finding a '\0' character.
#
# Inputs:
#   ds:si = address of first character in the string
sprintln_real:
    lodsb               # al = [ds:si]
    cmp al, 0
    je 1f               # if al == 0 return
    call cprint_real    # print character
    jmp sprintln_real   # restart loop

    # Print new line
1:  mov al, '\r'
    call cprint_real
    mov al, '\n'
    call cprint_real
    ret
