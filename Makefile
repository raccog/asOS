# Paths and commands used in all makefiles
ROOT_DIR = $(shell pwd)
MAKE_DIR = $(ROOT_DIR)/build-cfg

include $(MAKE_DIR)/paths.mk

# List of variables to pass to sub-makefiles
MAKE_VARS = ROOT_DIR='$(ROOT_DIR)' \
			MAKE_DIR='$(MAKE_DIR)'

all: bootloader
.PHONY: all

clean: 
	rm -rf $(BIN_DIR)/{sysroot,x86_64-pc-unknown-windows,bootx64.efi}
.PHONY: clean

nuke:
	rm -rf $(BIN_DIR)
.PHONY: nuke

bootloader:
	$(MAKE_VARS) $(MAKE) -C boot/arch/x86_64
.PHONY: bootloader

run:
	$(MAKE_VARS) $(MAKE) -C boot/arch/x86_64 run
.PHONY: run