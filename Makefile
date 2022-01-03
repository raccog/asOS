# Path variables
SRC_DIR := $(PWD)
BUILD_DIR := $(SRC_DIR)/Build
TOOLCHAIN_DIR := $(SRC_DIR)/Toolchain
TOOLCHAIN_BIN_DIR := $(TOOLCHAIN_DIR)/bin
PREFIX=$(TOOLCHAIN_DIR)
export PREFIX

ARCHLIB_DIR := $(SRC_DIR)/ArchLib
ARCHLIB_BUILD_DIR := $(BUILD_DIR)/ArchLib
ARCHLIB_i686_DIR := $(ARCHLIB_DIR)/i686
ARCHLIB_i686_BUILD_DIR := $(ARCHLIB_BUILD_DIR)/i686

KERNEL_DIR := $(SRC_DIR)/Kernel
KERNEL_BUILD_DIR := $(BUILD_DIR)/Kernel

STDLIB_DIR := $(SRC_DIR)/StdLib
STDLIB_BUILD_DIR := $(BUILD_DIR)/StdLib

BOOT_DIR := $(SRC_DIR)/Boot
BOOT_BUILD_DIR := $(BUILD_DIR)/Boot
BOOT_i686_DIR := $(BOOT_DIR)/i686
BOOT_i686_BUILD_DIR := $(BOOT_BUILD_DIR)/i686

KERNEL_i686_DIR := $(KERNEL_DIR)/Arch/i686
KERNEL_i686_BUILD_DIR := $(KERNEL_BUILD_DIR)/Arch/i686

# Executable paths
i686_AS := $(TOOLCHAIN_BIN_DIR)/i686-elf-as
i686_AR := $(TOOLCHAIN_BIN_DIR)/i686-elf-ar
i686_CC := $(TOOLCHAIN_BIN_DIR)/i686-elf-gcc
i686_CXX := $(TOOLCHAIN_BIN_DIR)/i686-elf-g++
i686_LD := $(TOOLCHAIN_BIN_DIR)/i686-elf-ld
i686_OBJCOPY := $(TOOLCHAIN_BIN_DIR)/i686-elf-objcopy

# Flags
CXX_FLAGS := -I$(SRC_DIR) -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti

# ArchLib files
ARCHLIB_i686_LINKER_SCRIPT := $(ARCHLIB_i686_DIR)/Linker.ld
ARCHLIB_i686_ASM := $(wildcard $(ARCHLIB_i686_DIR)/*.S)
ARCHLIB_i686_ASM_SRC := $(notdir $(ARCHLIB_i686_ASM))
ARCHLIB_i686_ASM_OBJ := $(patsubst %.S,$(ARCHLIB_i686_BUILD_DIR)/%.S.o,$(ARCHLIB_i686_ASM_SRC))

# Kernel files
KERNEL_i686_LINKER_SCRIPT := $(KERNEL_i686_DIR)/Linker.ld
KERNEL_i686_CXX := $(wildcard $(KERNEL_i686_DIR)/*.cpp)
KERNEL_i686_HEADER := $(wildcard $(KERNEL_i686_DIR)/*.h)
KERNEL_i686_ASM := $(wildcard $(KERNEL_i686_DIR)/*.S)

KERNEL_i686_CXX_SRC := $(notdir $(KERNEL_i686_CXX))
KERNEL_i686_ASM_SRC := $(notdir $(KERNEL_i686_ASM))
KERNEL_i686_CXX_OBJ := $(patsubst %.cpp,$(KERNEL_i686_BUILD_DIR)/%.cpp.o,$(KERNEL_i686_CXX_SRC))
KERNEL_i686_ASM_OBJ := $(patsubst %.S,$(KERNEL_i686_BUILD_DIR)/%.S.o,$(KERNEL_i686_ASM_SRC))

# Bootloader files
BOOT_i686_LINKER_SCRIPT := $(BOOT_i686_DIR)/Linker.ld
BOOT_i686_ASM := $(wildcard $(BOOT_i686_DIR)/*.S)
BOOT_i686_ASM_SRC := $(notdir $(BOOT_i686_ASM))
BOOT_i686_ASM_OBJ := $(patsubst %.S,$(BOOT_i686_BUILD_DIR)/%.S.o,$(BOOT_i686_ASM_SRC))

# StdLib files
STDLIB_CXX := $(wildcard $(STDLIB_DIR)/*.cpp)
STDLIB_CXX_SRC := $(notdir $(STDLIB_CXX))
STDLIB_CXX_OBJ := $(patsubst %.cpp,$(STDLIB_BUILD_DIR)/%.cpp.o,$(STDLIB_CXX_SRC))

#
# Main rules
#

all: asos-i686

run: asos-i686
	qemu-system-i386 -fda $(BUILD_DIR)/asos-i686.img

clean: 
	rm -rf $(BUILD_DIR)

#
# Toolchain rules
#

toolchain-gnu:
	cd Scripts/MakeToolchain/gnu && $(MAKE)

toolchain-llvm:
	cd Scripts/MakeToolchain/llvm && $(MAKE) 

clean-toolchain:
	rm -rf Toolchain

.EXPORT_ALL_VARIABLES:

.PHONY: all clean asos-i686 i686-bootloader-all bootloader-all kernel-all

#
# Image rules
#

asos-i686: $(BUILD_DIR)/asos-i686.img

$(BUILD_DIR)/asos-i686.img: $(BUILD_DIR) i686-bootloader-all kernel-all
	$(i686_OBJCOPY) -I binary -O binary --pad-to 0xe00 $(BOOT_BUILD_DIR)/i686-bootloader $@
	cat $(KERNEL_BUILD_DIR)/asos-kernel >> $@

#
# Directory rules
#

$(BUILD_DIR):
	@mkdir -p $@

$(ARCHLIB_BUILD_DIR): $(BUILD_DIR)
	@mkdir -p $@

$(ARCHLIB_i686_BUILD_DIR): $(ARCHLIB_BUILD_DIR)
	@mkdir -p $@

$(BOOT_BUILD_DIR): $(BUILD_DIR)
	@mkdir -p $@

$(STDLIB_BUILD_DIR): $(BUILD_DIR)
	@mkdir -p $@

$(BOOT_i686_BUILD_DIR): $(BOOT_BUILD_DIR)
	@mkdir -p $@

$(KERNEL_BUILD_DIR):
	@mkdir -p $@

$(KERNEL_i686_BUILD_DIR):
	@mkdir -p $@

#
# ArchLib rules
#

archlib-all: i686-archlib-all

i686-archlib-all: $(ARCHLIB_i686_BUILD_DIR) $(ARCHLIB_BUILD_DIR)/i686-archlib.a

$(ARCHLIB_BUILD_DIR)/i686-archlib.a: $(ARCHLIB_i686_ASM_OBJ)
	$(i686_AR) rcs $@ $^

$(ARCHLIB_i686_BUILD_DIR)/%.S.o: $(ARCHLIB_i686_DIR)/%.S
	$(i686_AS) -o $@ $^

#
# Bootloader rules
#

bootloader-all: i686-bootloader-all

i686-bootloader-all: i686-archlib-all $(BOOT_i686_BUILD_DIR) $(BOOT_BUILD_DIR)/i686-bootloader

$(BOOT_BUILD_DIR)/i686-bootloader: $(BOOT_i686_ASM_OBJ) $(ARCHLIB_BUILD_DIR)/i686-archlib.a
	$(i686_LD) --oformat binary -T $(BOOT_i686_LINKER_SCRIPT) -o $@ $^

$(BOOT_i686_BUILD_DIR)/%.S.o: $(BOOT_i686_DIR)/%.S
	$(i686_AS) -o $@ $^

#
# Kernel rules
#

kernel-all: archlib-all stdlib-all $(KERNEL_BUILD_DIR) $(KERNEL_i686_BUILD_DIR) $(KERNEL_BUILD_DIR)/asos-kernel

$(KERNEL_BUILD_DIR)/asos-kernel: $(KERNEL_i686_CXX_OBJ) $(KERNEL_i686_ASM_OBJ) $(ARCHLIB_BUILD_DIR)/i686-archlib.a $(STDLIB_BUILD_DIR)/stdlib.a
	$(i686_LD) --oformat binary -T $(KERNEL_i686_LINKER_SCRIPT) -o $@ $^

$(KERNEL_i686_BUILD_DIR)/%.S.o: $(KERNEL_i686_DIR)/%.S
	$(i686_AS) -o $@ $^

$(KERNEL_i686_BUILD_DIR)/%.cpp.o: $(KERNEL_i686_DIR)/%.cpp
	$(i686_CXX) -o $@ -c $< $(CXX_FLAGS)

#
# StdLib rules
#

stdlib-all: $(STDLIB_BUILD_DIR) $(STDLIB_BUILD_DIR)/stdlib.a

$(STDLIB_BUILD_DIR)/stdlib.a: $(STDLIB_CXX_OBJ)
	$(i686_AR) rcs $@ $^

$(STDLIB_BUILD_DIR)/%.cpp.o: $(STDLIB_DIR)/%.cpp
	$(i686_CXX) -o $@ -c $< $(CXX_FLAGS)
