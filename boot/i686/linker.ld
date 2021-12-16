ENTRY(_boot_entry1)

MEMORY {

	boot1 : o = 0x7c00, l = 0x200
	boot2 : o = 0x7000, l = 3K
	
}

SECTIONS {

	.boot1 0x7c00 : AT(0) {
		**/i686/boot.S.o(.text)
	} > boot1

	.boot2 0x7000 : AT(SIZEOF(.boot1))  {
		**/i686/boot2.S.o(.text)
		**/i686/pmode.S.o(.text)
	} > boot2

}
