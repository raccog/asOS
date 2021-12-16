# Path variables
SRC_DIR := $(PWD)
BUILD_DIR := $(SRC_DIR)/build
TOOLCHAIN_DIR := $(SRC_DIR)/toolchain
TOOLCHAIN_BIN_DIR := $(TOOLCHAIN_DIR)/bin

BOOT_DIR := $(SRC_DIR)/boot
KERNEL_DIR := $(SRC_DIR)/kernel
KERNEL_BUILD_DIR := $(BUILD_DIR)/kernel

KERNEL_i686_DIR := $(KERNEL_DIR)/arch/i686
KERNEL_i686_BUILD_DIR := $(KERNEL_BUILD_DIR)/arch/i686

# Executable paths
i686_AS := $(TOOLCHAIN_BIN_DIR)/i686-elf-as
i686_CC := $(TOOLCHAIN_BIN_DIR)/i686-elf-gcc
i686_CXX := $(TOOLCHAIN_BIN_DIR)/i686-elf-g++
i686_LD := $(TOOLCHAIN_BIN_DIR)/i686-elf-ld
i686_OBJCOPY := $(TOOLCHAIN_BIN_DIR)/i686-elf-objcopy

# Flags
CXX_FLAGS := -I$(SRC_DIR) -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti

# Kernel files
KERNEL_i686_LINKER_SCRIPT := $(KERNEL_i686_DIR)/Linker.ld
KERNEL_i686_CXX := $(wildcard $(KERNEL_i686_DIR)/*.cpp)
KERNEL_i686_HEADER := $(wildcard $(KERNEL_i686_DIR)/*.h)
KERNEL_i686_ASM := $(wildcard $(KERNEL_i686_DIR)/*.S)

KERNEL_i686_CXX_SRC := $(notdir $(KERNEL_i686_CXX))
KERNEL_i686_ASM_SRC := $(notdir $(KERNEL_i686_ASM))
KERNEL_i686_CXX_OBJ := $(patsubst %.cpp,$(KERNEL_i686_BUILD_DIR)/%.cpp.o,$(KERNEL_i686_CXX_SRC))
KERNEL_i686_ASM_OBJ := $(patsubst %.S,$(KERNEL_i686_BUILD_DIR)/%.S.o,$(KERNEL_i686_ASM_SRC))

all: $(BUILD_DIR) bootloader kernel-all

clean: 
	rm -rf build/

$(BUILD_DIR):
	@mkdir -p $@

bootloader:
	$(MAKE) -C boot

$(KERNEL_BUILD_DIR):
	@mkdir -p $@

$(KERNEL_i686_BUILD_DIR):
	@mkdir -p $@

kernel-all: $(KERNEL_BUILD_DIR) $(KERNEL_i686_BUILD_DIR) $(KERNEL_BUILD_DIR)/asos-kernel

$(KERNEL_BUILD_DIR)/asos-kernel: $(KERNEL_i686_CXX_OBJ) $(KERNEL_i686_ASM_OBJ)
	$(i686_LD) --oformat binary -T $(KERNEL_i686_LINKER_SCRIPT) -o $@ $^

$(KERNEL_i686_BUILD_DIR)/%.S.o: $(KERNEL_i686_DIR)/%.S
	$(i686_AS) -o $@ $^

$(KERNEL_i686_BUILD_DIR)/%.cpp.o: $(KERNEL_i686_DIR)/%.cpp
	$(i686_CXX) -o $@ -c $< $(CXX_FLAGS)

.EXPORT_ALL_VARIABLES:

.PHONY: all clean bootloader kernel-all
