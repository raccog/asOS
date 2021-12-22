/*
 * Second BIOS bootloader in protected mode.
 *
 * Processor starts running at begin_pmode after the second bootloader has
 * entered protected mode. When finished with all tasks, control is handed
 * off to the kernel bootstrap procedure.
 */
.intel_syntax noprefix

.section .text
.code32

# Includes
.include "arch/i686/Locations.inc.S"

# string constants
MSG_PMODE:  .asciz "Running second bootloader in protected mode."

.global begin_pmode
begin_pmode:
    # set segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    # set stack
    mov esp, BOOTLOADER_STACK

    # print init message
    mov esi, offset MSG_PMODE
    call sprint

    # jump to kernel boostrap
    jmp 0x8000

    hlt

