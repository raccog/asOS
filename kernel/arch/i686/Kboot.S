/*
 *  Kernel i686 bootstrap procedure.
 */

.intel_syntax noprefix

.section .text
.code32

.global _start
.type _start, @function
_start:
    call kmain  # call kernel main function

    # kernel main function should never return,
    # but if it does, halt and run an endless loop
    cli
    hlt
1:  jmp 1b
