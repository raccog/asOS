TARGET = x86_64-pc-unknown-windows
CFLAGS = -ffreestanding -fno-stack-protector -fshort-wchar -mno-red-zone -Wall -include $(ROOT_DIR)/libs/std/prelude.h

include $(MAKE_DIR)/*.mk

# List of variables to pass to sub-makefiles
MAKE_VARS = ROOT_DIR='$(ROOT_DIR)' \
			MAKE_DIR='$(MAKE_DIR)' \
			TARGET='$(TARGET)' \
			CFLAGS='$(CFLAGS)'

BOOT_X64_SRC_DIR = $(ROOT_DIR)/boot/efi
BOOT_X64_BIN_DIR = $(BIN_DIR)/$(TARGET)/boot

BOOT_X64_SRC = 	entry.c \
				efi.c
BOOT_X64_OBJ = $(patsubst %, $(BOOT_X64_BIN_DIR)/%.o, $(BOOT_X64_SRC))
OVMF_SRC = $(CACHE_DIR)/OVMF.fd

BOOT_X64_CC = clang
BOOT_X64_LD = lld-link
BOOT_X64_CFLAGS = $(CFLAGS) -target $(TARGET) -std=c11 -I$(ROOT_DIR)/libs -D__LIB__='"UEFI_BOOT"'
BOOT_X64_LDFLAGS = -flavor link -subsystem:efi_application -entry:efi_main

STD_LIB = $(BIN_DIR)/$(TARGET)/libstd.a
HW_LIB = $(BIN_DIR)/$(TARGET)/libhardware.a

BOOT_X64_LIB = $(BIN_DIR)/bootx64.efi
BOOT_X64_LIB_DEPS = $(STD_LIB) $(HW_LIB)

all: $(BOOT_X64_LIB)
.PHONY: all

run: $(BOOT_X64_LIB) $(OVMF_SRC)
	echo "UEFI bootloader running..."
	mkdir -p $(BIN_DIR)/sysroot/EFI/BOOT
	cp $< $(BIN_DIR)/sysroot/EFI/BOOT
	qemu-system-x86_64 \
		-nographic \
		-bios $(OVMF_SRC) \
		-net none \
		-drive file=fat:rw:$(BIN_DIR)/sysroot,media=disk,format=raw \
		-m 128M
.PHONY: run

$(OVMF_SRC):
	$(MKCWD)
	wget https://retrage.github.io/edk2-nightly/bin/DEBUGX64_OVMF.fd
	mv DEBUGX64_OVMF.fd $@

$(BOOT_X64_LIB): $(BOOT_X64_OBJ) $(BOOT_X64_LIB_DEPS)
	$(MKCWD)
	$(BOOT_X64_LD) $(BOOT_X64_LDFLAGS) -out:$@ $^

$(BOOT_X64_BIN_DIR)/%.c.o: %.c $(BOOT_X64_LIB_DEPS)
	$(MKCWD)
	$(BOOT_X64_CC) $(BOOT_X64_CFLAGS) -c -o $@ $<

$(STD_LIB):
	echo "Stdlib building for target: $(TARGET)..."
	$(MAKE_VARS) $(MAKE) -s -C $(ROOT_DIR)/libs/std
	echo "Stdlib build!"
.PHONY: $(STD_LIB)

$(HW_LIB):
	echo "Hardware lib building for target: $(TARGET)..."
	$(MAKE_VARS) $(MAKE) -s -C $(ROOT_DIR)/libs/hw
	echo "Hardware lib built!"
.PHONY: $(HW_LIB)
