#
#	Makefile for asOS
#
#

# Directories
PWD := $(shell pwd)
BUILD_DIR := Build
KERNEL_DIR := Kernel

# Executables
AS := clang
CXX := clang++
LD := ld.lld
QEMU := qemu-system-aarch64

# Files
KERNEL_CXX := $(wildcard Kernel/*.cpp)
KERNEL_AS := $(wildcard Kernel/*.S)
KERNEL_OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(KERNEL_CXX))
KERNEL_OBJ += $(patsubst %.S,$(BUILD_DIR)/%.o,$(KERNEL_AS))
LINKER_FILE := Kernel/Linker.ld

# Flags and extra
TARGET := aarch64-none-none-elf
AS_FLAGS := -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti
CXX_FLAGS := -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti
LD_FLAGS := -nostdlib 

#
# Main targets
#

.PHONY: all clean run

all: $(BUILD_DIR)/kernel.elf

clean:
	rm -rf $(BUILD_DIR)

run: $(BUILD_DIR)/kernel.elf
	@$(QEMU) -machine virt -cpu cortex-a57 -kernel $< -nographic

#
# Kernel targets
#

$(BUILD_DIR)/kernel.elf: $(KERNEL_OBJ)
	@$(LD) $(LD_FLAGS) -T $(LINKER_FILE) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CXX) -c $< -o $@ -target $(TARGET) $(CXX_FLAGS)

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(@D)
	@$(AS) -c $< -o $@ -target $(TARGET) $(AS_FLAGS)
