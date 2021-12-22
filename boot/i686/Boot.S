/*
 *  First BIOS bootloader.
 *
 *  Processor starts running at _boot_entry1 after control is given by BIOS. Control
 *  is then handed off to the second BIOS bootloader.
 */

.intel_syntax noprefix

.section .text
.global _boot_entry1    # export entry point to linker
.code16

# Includes
.include "arch/i686/Locations.inc.S"

.align 8
_boot_entry1:
    # clear segment locations
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    # disable interrupts
    cli

    # set stack segment and pointer
    mov ss, ax  
    mov sp, BOOTLOADER_STACK

    # save boot disk number for later
    push dx

    # clear the screen and set VGA video mode to 25x80 tty
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    # reset disk
    mov ah, 0x00    # reset disk
    int 0x13

    # load second bootloader
    mov bx, BOOTLOADER2
    mov ah, 0x02    # read disk sectors
    mov al, 0x06    # read 6 sectors (3K)
    mov ch, 0x00    # cylinder 0
    mov cl, 0x02    # sector 2
    mov dx, [esp]   # restore drive number
    mov dh, 0x00    # drive head 0
    int 0x13
    jb 1f       # error if CL==1
    or ah, ah
    jnz 1f      # error if ah!=0
    or al, al
    jz 1f       # error if al==0

    # print init message
    mov si, offset MSG_BOOT1
    call sprint_real

    # load kernel
    mov bx, KERNEL
    mov ah, 0x02    # read disk sectors
    mov al, 0x10    # read 16 sectors (8K)
    mov ch, 0x00    # cylinder 0
    mov cl, 0x08    # sector 8
    pop dx          # restore drive number
    mov dh, 0x00    # drive head 0
    int 0x13
    jb kernel_read_error    # error if CL==1
    or ah, ah
    jnz kernel_read_error   # error if ah!=0
    or al, al
    jz kernel_read_error    # error if al==0

    # jump to second bootloader
    jmp 0x0000:BOOTLOADER2

1:  hlt

# string constants
MSG_BOOT1:          .asciz "Running first bootloader"
MSG_KERNEL_ERROR:   .asciz "Error while reading kernel from disk"

kernel_read_error:
    # print bootloader read error message
    mov si, offset MSG_KERNEL_ERROR
    call sprint_real

    hlt

# fill with zeros until MBR signature
.fill 510 - (. - _boot_entry1), 1, 0
.word 0xaa55    # MBR signature
