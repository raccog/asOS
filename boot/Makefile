# Path variables
BOOT_BUILD_DIR := $(BUILD_DIR)/boot

BOOT_i686_DIR := $(BOOT_DIR)/i686
BOOT_i686_BUILD_DIR := $(BOOT_BUILD_DIR)/i686

# Bootloader files
BOOT_i686_LINKER_SCRIPT := $(BOOT_i686_DIR)/Linker.ld
BOOT_i686_ASM := $(wildcard $(BOOT_i686_DIR)/*.S)
BOOT_i686_ASM_SRC := $(notdir $(BOOT_i686_ASM))
BOOT_i686_ASM_OBJ := $(patsubst %.S,$(BOOT_i686_BUILD_DIR)/%.S.o,$(BOOT_i686_ASM_SRC))

all: i686-bootloader-all

i686-bootloader-all: $(BOOT_i686_BUILD_DIR) $(BOOT_BUILD_DIR)/i686-bootloader

$(BOOT_BUILD_DIR)/i686-bootloader: $(BOOT_i686_ASM_OBJ)
	$(i686_LD) --oformat binary -T $(BOOT_i686_LINKER_SCRIPT) -o $@ $^

$(BOOT_BUILD_DIR):
	@mkdir -p $@

$(BOOT_i686_BUILD_DIR): $(BOOT_BUILD_DIR)
	@mkdir -p $@

$(BOOT_i686_BUILD_DIR)/%.S.o: $(BOOT_i686_DIR)/%.S
	$(i686_AS) -o $@ $^

.PHONY: all i686-bootloader-all

