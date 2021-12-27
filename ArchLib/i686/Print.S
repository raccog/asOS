/*
 *  Bootloader print functions
 *
 */

.intel_syntax noprefix

.section .rodata
# string constants
hexstr:     .ascii "0123456789ABCDEF"

.section .data

# data
outstr16:   .asciz "0x0000"
xpos:       .byte 0
ypos:       .byte 0

.section .text
.code32

# global procedures
.global cprint
.global sprint
.global printreg16

# Prints an ascii character to the current VGA text position.
#
# Inputs:
#   al = character
#
# The color attribute will always be white on black.
cprint:
    # add attribute to character
    mov ah, 0x07    # attribute = white on black
    mov cx, ax      # save word/attribute

    # get position offset
    movzx eax, byte ptr [ypos]
    mov edx, 160    # 2 bytes (char/attribute)
    mul edx         # for 80 columns
    movzx ebx, byte ptr [xpos]
    shl ebx, 1      # times 2 to skip attribute

    # get position address in VGA memory
    mov edi, 0xb8000    # start of VGA text memory
    add edi, eax        # add y offset
    add edi, ebx        # add x offset

    # write character and update position
    mov [edi], cx           # write char/attribute
    add byte ptr [xpos], 1  # move to next column

    # move to the start of the next row if the last
    # column of this row is full
    cmp byte ptr [xpos], 80
    jl 1f                   # if xpos < 80 return
    mov byte ptr [xpos], 0  # reset column
    add byte ptr [ypos], 1  # move to next row

    # move to the top row if the bottom row has been
    # full
    cmp byte ptr [ypos], 25
    jl 1f                   # if ypos < 25 return
    mov byte ptr [ypos], 0  # reset row

1:
    ret

# Prints a string of ascii characters until finding a '\0' character.
#
# Inputs:
#   esi = address of first character in the string
sprint:
    jmp 2f
1:
    call cprint             # print current character
    inc esi                 # move to next character in string
2:
    mov al, [esi]           # load character from esi
    or al, al
    jnz 1b                  # if al != 0 print next character
    add byte ptr [ypos], 1  # move to next row
    mov byte ptr [xpos], 0  # reset column to 0

    # move to the top row if the bottom row has been
    # full
    cmp byte ptr [ypos], 25
    jl 3f                   # if ypos < 25 return
    mov byte ptr [ypos], 0  # reset row

3:
    ret

# Prints a 16-bit register in hex.
#
# Inputs:
#   ax = 16-bit register to be printed
printreg16:
    mov edi, offset outstr16 + 2    # get offset of output buffer
    mov esi, offset hexstr          # get offset of character string
    mov cx, 4                       # 4 hex characters
1:
    rol ax, 4       # rotate left-most 4-bits to the right-most 4-bits
    mov ebx, eax    # copy eax to ebx
    and bx, 0x0f    # extract right-most 4-bits
    mov bl, [esi + ebx]
    mov [edi], bl   # copy hex character to output buffer
    inc edi         # move to next output character
    loop 1b

    mov esi, offset outstr16
    call sprint     # print output buffer

    ret
