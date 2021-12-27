ENTRY(_boot_entry1)

MEMORY {
	boot1 : o = 0x7c00, l = 0x200
	boot2 : o = 0x7000, l = 3K
}

SECTIONS {

	.boot1 ORIGIN(boot1) : AT(0) {
		*/Build/Boot/i686/Boot.S.o(.text)
        */Build/Boot/i686/PrintReal.S.o(.text .rodata .data)

        . = 510;
        SHORT(0xaa55)
	} > boot1 =0x0000

	.boot2 ORIGIN(boot2) : AT(LENGTH(boot1)) {
		*/Build/Boot/i686/Boot2.S.o(.text)
		*/Build/Boot/i686/Pmode.S.o(.text)
        */Build/ArchLib/i686-archlib.a(.text .rodata .data)
	} > boot2

}
