/*
 * Second bootloader for always sunny OS (asOS)
 */
.intel_syntax noprefix


.section .text

.code16     # 16-bit code

/* jump past data */
_second_boot_entry:
    jmp _boot2_code

/* interrupt table descriptor register */
idtr:
    .word idt_end - idt_start - 1   # size = 256 entries * 8 bytes each
    .long idt_start                 # offset = address of IDT

.align 8    # align IDT to 8 byte boundary
idt_start:
    .fill 256, 8, 0     # fill 256 entries with address 0x00000000
idt_end:

/* global table descriptor register */
gdtr:
    .word gdt_end - gdt_start - 1   # size = 4 entries * 8 bytes each
    .long gdt_start                 # offset = address of GDT

/* global descriptor table */
.align 8    # align GDT to 8 byte boundary
gdt_start:
# null descriptor
    .long 0
    .long 0
# code descriptor
    .word 0xffff        # limit 0-15
    .word 0             # base 0-15
    .byte 0             # base 16-23
    .byte 0b10011010    # access byte
    .byte 0b11001111    # flags and limit
    .byte 0             # base 24-31
# data descriptor
    .word 0xffff        # limit 0-15
    .word 0             # base 0-15
    .byte 0             # base 16-23
    .byte 0b10010010    # access byte
    .byte 0b11001111    # flags and limit
    .byte 0             # base 24-31
# empty TSS descriptor
    .long 0
    .long 0
gdt_end:

# string constants
MSG_BOOT2:      .asciz "Running second bootloader."
a20_MSG_ERROR:  .asciz "a20 line could not be enabled. Halting."
hexstr:         .ascii "0123456789ABCDEF"

/* call to enable a20 line */
enable_a20:
    /* check if a20 line is enabled */
    call a20_check  # test a20 line
    or ax, ax
    jnz a20_enabled # if ax!=0 a20 enabled

    /* enable a20 line through BIOS */
    mov ax, 0x2403  # check for BIOS a20 support
    int 0x15
    jb a20_keycontroller    # if CF==1 no support
    or ah, ah
    jnz a20_keycontroller   # if ah!=0 no support

    mov ax, 0x2402  # check a20 gate status
    int 0x15
    jb a20_keycontroller    # if CF==1 BIOS error
    or ah, ah
    jnz a20_keycontroller   # if ah!=0 BIOS error
    cmp al, 1
    je a20_enabled          # if al==1 a20 enabled

    mov ax, 0x2401  # enable a20 gate
    int 0x15
    jb a20_keycontroller    # if CF==1 BIOS error
    or ah, ah
    jnz a20_keycontroller   # if ah!=0 BIOS error

    call a20_check  # test a20 gate without BIOS
    or ax, ax
    jnz a20_enabled # if ax!=0 a20 enabled

    /* enable a20 line through keyboard controller */
a20_keycontroller:
    call a20_wait   # test a20 line
    mov al, 0xad    # ad = disable first PS/2 port
    out 0x64, al    # write ps2 command

    call a20_wait   # test a20 line
    mov al, 0xd0    # d0 = read controller output port
    out 0x64, al    # write ps2 command

    call a20_wait2  # test a20 line

    in al, 0x60     # read a20 status to ax
    push ax         # push status to stack

    call a20_wait   # test a20 line
    mov al, 0xd1    # d1 = write to controller output port
    out 0x64, al    # write ps2 command
    
    call a20_wait   # test a20 line
    pop ax          # pop a20 status
    or al, 2        # set bit to enable a20 line
    out 0x60, al    # write ps2 data

    call a20_wait   # test a20 line
    mov al, 0xae    # ae = enable first PS/2 port
    out 0x64, al    # write ps2 command

    call a20_wait   # test a20 line

    mov cx, 0xff
a20_check_loop1:
    call a20_check  # test a20 line 256 times
    or ax, ax
    jnz a20_enabled # if ax!=0 a20 enabled
    loop a20_check_loop1

a20_fast:
    in al, 0x92     # read fast a20 gate register
    cmp al, 2       # check if a20 bit is set
    jne a20_enabled

    or al, 2        # set a20 enable bit
    and al, 0xfe    # ensure bit 0 is not set (used for fast reset)
    out 0x92, al    # write to fast a20 gate register

a20_fast_done:
    mov cx, 0xff
a20_check_loop2:
    call a20_check  # check a20 line 256 times
    or ax, ax
    jnz a20_enabled # if ax!=0 a20 enabled
    loop a20_check_loop2

    /* display error and halt, a20 line could not be enabled */
    mov si, offset a20_MSG_ERROR
    call sprint

    hlt

    /* a20 line is enabled */
a20_enabled:

    ret

/* runs a loop until the input buffer is clear */
a20_wait:
    in al, 0x64     # read status register
    test al, 2      # if input buffer is full
    jnz a20_wait    # check again
    ret

/* runs a loop until the output buffer is full */
a20_wait2:
    in al, 0x64     # read status register
    test al, 1      # if output buffer is empty
    jz a20_wait2    # check again
    ret

/* checks if the a20 line is enabled by checking if two different
 * memory addresses point to the same data
 *
 * outputs: ax = 0 if disabled, 1 if enabled
 *
 * modified from https://wiki.osdev.org/A20_Line
 */
a20_check:
    push ds     # preserve ds

    xor ax, ax
    not ax
    mov ds, ax  # ds = 0xffff

    mov di, 0x500
    mov si, 0x510   # si offset by 64 bytes

    mov byte ptr es:[di], 0x00  # es:di = 0
    mov byte ptr ds:[si], 0xff  # ds:si = ff

    cmp byte ptr es:[di], 0xff

    mov ax, 0
    je a20_check_exit   # if es:di==ff return 0

    mov ax, 1           # if es:di!=ff return 1
a20_check_exit:
    pop ds      # restore ds
    ret

# data
outstr16:   .asciz "0x0000"
reg16:      .word 0
xpos:       .byte 0
ypos:       .byte 0

cprint:
    push es # preserve es

    mov dx, 0xb800
    mov es, dx      # es = start of VGA memory

    mov ah, 0x07    # attribute = white on black
    mov cx, ax      # save word/attribute
    movzx ax, byte ptr [ypos]
    mov dx, 160     # 2 bytes (char/attribute)
    mul dx          # for 80 columns
    movzx bx, byte ptr [xpos]
    shl bx, 1       # times 2 to skip attribute

    mov di, 0   # start of VGA text memory
    add di, ax  # add y offset
    add di, bx  # add x offset

    mov ax, cx              # restore char/attribute
    stosw                   # write char/attribute
    add byte ptr [xpos], 1  # move to next column

    pop es  # restore es

    ret

sprint_dochar:
    call cprint
sprint:
    lodsb                   # load byte from ds:si
    or al, al
    jnz sprint_dochar       # if al!=0 print next character
    add byte ptr [ypos], 1  # move to next row
    mov byte ptr [xpos], 0  # reset column to 0

    ret

printreg16:
    mov di, offset outstr16 + 2 # get offset of output buffer
    mov ax, [reg16]             # move value into ax
    mov si, offset hexstr       # get offset of character string
    mov cx, 4                   # 4 hex characters
printreg16_loop:
    rol ax, 4       # rotate left-most 4-bits to the right-most 4-bits
    mov bx, ax      # copy ax to bx
    and bx, 0x0f    # extract right-most 4-bits
    mov bl, [si + bx]
    mov [di], bl    # copy hex character to output buffer
    inc di          # move to next output character
    loop printreg16_loop

    mov si, outstr16
    call sprint     # print output buffer

    ret

/* entry point */
_boot2_code:
    cld # clear direction flag

    /* print init message */
    mov esi, offset MSG_BOOT2
    call sprint

    /* enable a20 line */
    call enable_a20         # enable a20 line
    
    /* load GDT */
    lgdt [gdtr]

    /* load IDT */
    lidt [idtr]

    /* enter protected mode */
    mov eax, cr0
    or al, 1
    mov cr0, eax

    /* clear instruction pipeline */
    jmp 0x8:begin_pmode

    hlt
