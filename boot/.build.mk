BOOT_CFLAGS := $(CFLAGS) -ffreestanding -target i686-elf
BOOT_LDFLAGS := -nostdlib -Wl,--oformat=binary -target i686-elf

BOOT_BUILD_DIR := $(BUILD_DIR)/boot/arch/i686
BOOT_SRC := boot/arch/i686/boot.S
BOOT_OBJ := $(patsubst boot/arch/i686/%, $(BOOT_BUILD_DIR)/%.o, $(BOOT_SRC))

BOOTLOADER := $(BOOT_BUILD_DIR)/boot.bin

ALL += $(BOOTLOADER)

$(BOOTLOADER): $(BOOT_OBJ)
	@$(MKCWD)
	$(LD) -o $@ $^ $(BOOT_LDFLAGS)

$(BOOT_BUILD_DIR)/%.S.o: boot/arch/i686/%.S
	@$(MKCWD)
	$(CC) -c -o $@ $< $(BOOT_CFLAGS)
