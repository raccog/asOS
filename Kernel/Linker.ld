/* 
 *  AArch64 linker script targeting QEMU
 *
 *  Derived from https://wiki.osdev.org/QEMU_AArch64_Virt_Bare_Bones
 *
 */

ENTRY(_start)
SECTIONS {
    . = 0x40000000;
    
    startup_begin = .;
    .startup . : {
        KEEP(*(.text.boot))
    }
    startup_end = .;

    text_begin = .;
    .text : {
        *(.text)
    }
    text_end = .;

    data_begin = .;
    .data : {
        *(.data)
    }
    data_end = .;

    rodata_begin = .;
    .rodata : {
        *(.rodata.*)
    }
    rodata_end = .;

    bss_begin = .;
    .bss : {
        *(.bss COMMON)
    }
    bss_end = .;

    . = ALIGN(8);
    stack_bottom = .;
    . += 0x1000; /* 4kB of stack memory */
    stack_top = .;
}
