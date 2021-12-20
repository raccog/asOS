/*
 * Second bootloader in protected mode for always sunny OS (asOS)
 */
.intel_syntax noprefix

.equ stack, 0x6e00

.section .text

.code32

# string constants
MSG_PMODE:  .asciz "Running second bootloader in protected mode."
hexstr:     .ascii "0123456789ABCDEF"

# data
outstr16:   .asciz "0x0000"
reg16:      .word 0
xpos:       .byte 0
ypos:       .byte 0

cprint:
    push esi    # preserve esi

    mov ah, 0x07    # attribute = white on black
    mov cx, ax      # save word/attribute
    movzx eax, byte ptr [ypos]
    mov edx, 160    # 2 bytes (char/attribute)
    mul edx         # for 80 columns
    movzx ebx, byte ptr [xpos]
    shl ebx, 1      # times 2 to skip attribute

    mov esi, 0xb8000    # start of VGA text memory
    add esi, eax        # add y offset
    add esi, ebx        # add x offset

    mov [esi], cx           # write char/attribute
    add byte ptr [xpos], 1  # move to next column

    pop esi     # restore esi

    ret

sprint_dochar:
    call cprint
    inc esi                 # move to next character in string
sprint:
    mov al, [esi]           # load byte from esi
    or al, al
    jnz sprint_dochar       # if al!=0 print next character
    add byte ptr [ypos], 1  # move to next row
    mov byte ptr [xpos], 0  # reset column to 0

    ret

printreg16:
    mov edi, offset outstr16 + 2    # get offset of output buffer
    xor eax, eax                    # eax = 0
    mov ax, [reg16]                 # move value into ax
    mov esi, offset hexstr          # get offset of character string
    mov cx, 4                       # 4 hex characters
printreg16_loop:
    rol ax, 4       # rotate left-most 4-bits to the right-most 4-bits
    mov ebx, eax    # copy eax to ebx
    and bx, 0x0f    # extract right-most 4-bits
    mov bl, [esi + ebx]
    mov [edi], bl   # copy hex character to output buffer
    inc edi         # move to next output character
    loop printreg16_loop

    mov esi, offset outstr16
    call sprint     # print output buffer

    ret

.code32
.global begin_pmode
begin_pmode:
    /* set segment registers */
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    /* set stack */
    mov esp, stack

    /* print init message */
    mov esi, offset MSG_PMODE
    call sprint

    jmp 0x8000

    hlt

