/*
 * Kernel bootstrap for always sunny OS (asOS)
 */

.intel_syntax noprefix

.equ kstack, 0x10000

.section .text
.code32
.global _start
.type _start, @function
_start:
    mov esp, kstack

    call kmain

    cli
    hlt
L1: jmp L1
