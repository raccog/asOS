MEMORY {

	ram : o = 0x8000, l = 16K
	
}

SECTIONS {

    .kboot : {
        **/i686/Kboot.S.o(.text)
    } > ram

    .text BLOCK(1K) : {
        *(.text)
    } > ram

    .rodata : {
        *(.rodata)
    } > ram

    .data : {
        *(.data)
    } > ram

    .bss : {
        *(.bss)
    } > ram

}
