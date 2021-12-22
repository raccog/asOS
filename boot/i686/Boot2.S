/*
 *  Second BIOS bootloader.
 *
 *  Processor starts running at _boot_entry2 after control is given by the first
 *  BIOS bootloader. When finished with all tasks, control is handed off to the
 *  kernel bootloader.
 */
.intel_syntax noprefix

.section .text
.code16

# Includes
.include "arch/i686/Locations.inc.S"

# jump past data
_boot_entry2:
    # clear direction flag
    cld

    # print init message
    mov esi, offset MSG_BOOT2
    call sprint_real

    # enable a20 line
    call enable_a20
    
    # clear GDT
    mov ecx, GDT_SIZE
    mov eax, 0
    mov edi, GDT
    rep stosd   # set gdt descriptors to all zeros

    # set minimal GDT
    mov edi, GDT
    mov ax, 0xffff      # set limit bits 0-15
    mov [edi + 8], ax
    mov [edi + 16], ax
    mov al, 0b11001111  # set flags and limit bits 16-19
    mov [edi + 14], al
    mov [edi + 22], al
    mov al, 0b10010010  # set data descriptor access byte
    mov [edi + 21], al
    or al, 0b00001000   # set code descriptor access byte
    mov [edi + 13], al

    lgdt [gdtr]

    # load blank IDT
    lidt [idtr]

    # enter protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    # clear instruction pipeline for protected mode
    jmp 0x8:begin_pmode

    hlt

# global table descriptor register
gdtr:
    .word GDT_SIZE      # size = 256 entries# 8 bytes
    .long GDT           # offset = address of GDT

# interrupt table descriptor register
idtr:
    .word IDT_SIZE      # size = 256 entries# 8 bytes
    .long IDT           # offset = address of IDT

# string constants
MSG_BOOT2:      .asciz "Running second bootloader."
a20_MSG_ERROR:  .asciz "a20 line could not be enabled. Halting."

# call to enable a20 line
enable_a20:
    # check if a20 line is enabled
    call a20_check  # test a20 line
    or ax, ax
    jnz a20_enabled # if ax!=0 a20 enabled

    # enable a20 line through BIOS
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

    # enable a20 line through keyboard controller
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

    # display error and halt, a20 line could not be enabled
    mov si, offset a20_MSG_ERROR
    call sprint_real

    hlt

    # a20 line is enabled
a20_enabled:

    ret

# runs a loop until the input buffer is clear
a20_wait:
    in al, 0x64     # read status register
    test al, 2      # if input buffer is full
    jnz a20_wait    # check again
    ret

# runs a loop until the output buffer is full
a20_wait2:
    in al, 0x64     # read status register
    test al, 1      # if output buffer is empty
    jz a20_wait2    # check again
    ret

# Checks if the a20 line is enabled by checking if two different
# memory addresses point to the same data
#
# Outputs: 
#   ax = 0 if disabled, 1 if enabled
#
# modified from https://wiki.osdev.org/A20_Line
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
