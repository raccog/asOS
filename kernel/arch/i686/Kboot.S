/*
 * Kernel bootstrap for always sunny OS (asOS)
 */

.intel_syntax noprefix

.section .text
.code32
.global _start
.type _start, @function
_start:

    call kmain

    cli
    hlt
L1: jmp L1
