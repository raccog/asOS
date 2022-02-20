BOOT_X64_DIR = boot/arch/x86_64
BOOT_X64_BUILD_DIR = $(BUILD_DIR)/$(BOOT_X64_DIR)
BOOT_X64_SRC = $(BOOT_X64_DIR)/main.c
BOOT_X64_OBJ = $(patsubst $(BOOT_X64_DIR)/%, $(BOOT_X64_BUILD_DIR)/%.o, $(BOOT_X64_SRC))

BOOT_X64_CC = clang
BOOT_X64_LD = lld-link
BOOT_X64_CFLAGS = -target x86_64-pc-unknown-windows -ffreestanding -mno-red-zone -std=c11 -Ilibs
BOOT_X64_LDFLAGS = -flavor link -subsystem:efi_application -entry:efi_main

BOOT_X64 = $(BOOT_X64_BUILD_DIR)/BOOTX64.EFI
BOOT_X64_H_DEPS = $(STD_H) $(EFI_H) $(SUNNY_H)
BOOT_X64_LIB_DEPS = $(STD_LIB) $(EFI_LIB) $(SUNNY_LIB)

ALL += $(BOOT_X64)

$(BOOT_X64): $(BOOT_X64_OBJ) $(BOOT_X64_LIB_DEPS)
	@$(MKCWD)
	$(BOOT_X64_LD) $(BOOT_X64_LDFLAGS) -out:$@ $^

$(BOOT_X64_BUILD_DIR)/%.c.o: $(BOOT_X64_DIR)/%.c $(BOOT_X64_H_DEPS)
	@$(MKCWD)
	$(BOOT_X64_CC) $(BOOT_X64_CFLAGS) -c -o $@ $<

$(CACHE_DIR)/OVMF.fd:
	@$(MKCWD)
	wget https://retrage.github.io/edk2-nightly/bin/DEBUGX64_OVMF.fd
	mv DEBUGX64_OVMF.fd $@

run: $(BOOT_X64) $(CACHE_DIR)/OVMF.fd
	@mkdir -p $(BUILD_DIR)/sysroot/EFI/BOOT
	@cp $< $(BUILD_DIR)/sysroot/EFI/BOOT
	qemu-system-x86_64 \
		-nographic \
		-bios $(CACHE_DIR)/OVMF.fd \
		-net none \
		-drive file=fat:rw:$(BUILD_DIR)/sysroot,media=disk,format=raw \
		-m 128M

