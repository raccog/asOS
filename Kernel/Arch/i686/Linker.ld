MEMORY {
	ram : o = 0x8000, l = 16K
}

SECTIONS {

    .kboot : {
        */Build/Kernel/Arch/i686/Kboot.S.o(.kboot)
    } >ram

    .text : {
        *(.text)
    } >ram

    .rodata : {
        *(.rodata)
    } >ram

    .data : {
        *(.data)
    } >ram

    .bss : {
        *(.bss)
    } >ram

}
