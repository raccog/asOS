ENTRY(_boot_entry1)

MEMORY {

	boot1 : o = 0x7c00, l = 0x200
	boot2 : o = 0x7000, l = 3k
	
}

SECTIONS {

	.boot1 0x7c00 : AT(0) {
		boot.o(.text)
	} > boot1

	.boot2 0x7000 : AT(SIZEOF(.boot1))  {
		boot2.o(.text)
	} > boot2

}
