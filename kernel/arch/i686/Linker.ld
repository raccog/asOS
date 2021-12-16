SECTIONS {

    .kboot : {
        **/i686/Kboot.S.o(.text)
    }

    .text BLOCK(1K) : {
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

}
