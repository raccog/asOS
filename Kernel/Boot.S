#
# Kernel boot script
#
# Taken from https://wiki.osdev.org/QEMU_AArch64_Virt_Bare_Bones
#

.section .text.boot
.global _start
_start:
    ldr x30, =stack_top
    mov sp, x30
    bl kmain
    b .
