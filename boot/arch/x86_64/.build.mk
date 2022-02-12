BOOT_x86_64_DIR = boot/arch/x86_64
BOOT_x86_64_BUILD_DIR = $(BUILD_DIR)/$(BOOT_x86_64_DIR)
BOOT_x86_64_SRC = $(BOOT_x86_64_DIR)/main.c
BOOT_x86_64_OBJ = $(patsubst $(BOOT_x86_64_DIR)/%, $(BOOT_x86_64_BUILD_DIR)/%.o, $(BOOT_x86_64_SRC))

BOOT_x86_64_CC = clang
BOOT_x86_64_LD = lld-link
BOOT_x86_64_CFLAGS = -target x86_64-pc-unknown-windows -ffreestanding -mno-red-zone -std=c11 -Icommon/arch/x86_64
BOOT_x86_64_LDFLAGS = -flavor link -subsystem:efi_application -entry:efi_main

BOOTLOADER_x86_64 = $(BOOT_x86_64_BUILD_DIR)/boot.efi

ALL += $(BOOTLOADER_x86_64)

$(BOOTLOADER_x86_64): $(BOOT_x86_64_OBJ)
	@$(MKCWD)
	$(BOOT_x86_64_LD) $(BOOT_x86_64_LDFLAGS) -out:$@ $^

$(BOOT_x86_64_BUILD_DIR)/%.c.o: $(BOOT_x86_64_DIR)/%.c
	@$(MKCWD)
	$(BOOT_x86_64_CC) $(BOOT_x86_64_CFLAGS) -c -o $@ $<

$(CACHE_DIR)/OVMF.fd:
	@$(MKCWD)
	wget https://retrage.github.io/edk2-nightly/bin/DEBUGX64_OVMF.fd
	mv DEBUGX64_OVMF.fd $@

run: $(BOOTLOADER_x86_64) $(CACHE_DIR)/OVMF.fd
	@mkdir -p $(BUILD_DIR)/sysroot/EFI/BOOT
	@cp $< $(BUILD_DIR)/sysroot/EFI/BOOT
	qemu-system-x86_64 -bios $(CACHE_DIR)/OVMF.fd -drive file=fat:rw:$(BUILD_DIR)/sysroot,media=disk,format=raw -net none

