/*
 * Kernel bootstrap for always sunny OS (asOS)
 */

.intel_syntax noprefix

.section .bss
.align 16
stack_bottom:
.skip 16384     # 16KB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    mov esp, offset stack_top

    call kmain

    cli
    hlt
L1: jmp L1
