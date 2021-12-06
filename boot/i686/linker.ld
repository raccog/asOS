ENTRY(_boot_entry1)

MEMORY {

	boot1 : o = 0x7c00, l = 0x200
	boot2 : o = 0x7000, l = 3K
    kernel : o = 0x8000, l = 16K
	
}

SECTIONS {

	.boot1 0x7c00 : AT(0) {
		boot.o(.text)
	} > boot1

	.boot2 0x7000 : AT(SIZEOF(.boot1))  {
		boot2.o(.text)
		pmode.o(.text)
	} > boot2

    . = 0xe00;
    .kernel 0x8000 (NOLOAD): AT(0xe00) {
        kboot.o(.bss .text)
    } > kernel

}
