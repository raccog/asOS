# Paths and commands used in all makefiles
ROOT_DIR = $(shell pwd)
MAKE_DIR = $(ROOT_DIR)/build-cfg

# List of variables to pass to sub-makefiles
MAKE_VARS = ROOT_DIR='$(ROOT_DIR)' \
			MAKE_DIR='$(MAKE_DIR)'

# Libs
#include libs/std/.build.mk
#include libs/efi/.build.mk

# Kernel lib
#include libs/sunny/.build.mk

# Bootloader
#include boot/arch/x86_64/.build.mk

all: bootloader
#	$(MAKE_VARS) make -C libs
.PHONY: all

clean: 
	rm -rf $(BUILD_DIR)
.PHONY: clean

bootloader:
	$(MAKE_VARS) $(MAKE) -C boot/arch/x86_64
.PHONY: bootloader