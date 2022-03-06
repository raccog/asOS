TARGET = x86_64-pc-unknown-windows

include $(MAKE_DIR)/*.mk

# List of variables to pass to sub-makefiles
MAKE_VARS = ROOT_DIR='$(ROOT_DIR)' \
			MAKE_DIR='$(MAKE_DIR)'

CFLAGS = -ffreestanding -mno-red-zone

BOOT_X64_SRC_DIR = $(ROOT_DIR)/boot/arch/x86_64
BOOT_X64_BIN_DIR = $(BIN_DIR)/$(TARGET)/boot

BOOT_X64_SRC = 	$(BOOT_X64_SRC_DIR)/main.c
BOOT_X64_OBJ = $(patsubst $(BOOT_X64_SRC_DIR)/%, $(BOOT_X64_BIN_DIR)/%.o, $(BOOT_X64_SRC))
OVMF_SRC = $(CACHE_DIR)/OVMF.fd

BOOT_X64_CC = clang
BOOT_X64_LD = lld-link
BOOT_X64_CFLAGS = $(CFLAGS) -target $(TARGET) -std=c11 -I$(ROOT_DIR)/libs
BOOT_X64_LDFLAGS = -flavor link -subsystem:efi_application -entry:efi_main

STD_LIB = $(BIN_DIR)/$(TARGET)/libstd.a
HW_LIB = $(BIN_DIR)/$(TARGET)/libhardware.a

BOOT_X64_LIB = $(BIN_DIR)/bootx64.efi
BOOT_X64_LIB_DEPS = $(STD_LIB) $(HW_LIB)

all: $(BOOT_X64_LIB)
.PHONY: all

$(OVMF_SRC):
	@$(MKCWD)
	wget https://retrage.github.io/edk2-nightly/bin/DEBUGX64_OVMF.fd
	mv DEBUGX64_OVMF.fd $@

$(BOOT_X64_LIB): $(BOOT_X64_OBJ) $(OVMF_SRC) $(BOOT_X64_LIB_DEPS)
	$(MKCWD)
	$(BOOT_X64_LD) $(BOOT_X64_LDFLAGS) -out:$@ $^

$(BOOT_X64_BIN_DIR)/%.c.o: $(BOOT_X64_SRC_DIR)/%.c
	$(MKCWD)
	$(BOOT_X64_CC) $(BOOT_X64_CFLAGS) -c -o $@ $<

$(STD_LIB):
	$(MAKE_VARS) TARGET='$(TARGET)' CFLAGS='$(CFLAGS)' $(MAKE) -C $(ROOT_DIR)/libs/std
.PHONY: $(STD_LIB)

$(HW_LIB):
	$(MAKE_VARS) TARGET='$(TARGET)' CFLAGS='$(CFLAGS)' $(MAKE) -C $(ROOT_DIR)/libs/hw
.PHONY: $(HW_LIB)
