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
	rm -rf build/*
.PHONY: clean

nuke:
	rm -rf build
.PHONY: nuke

bootloader:
	$(MAKE_VARS) $(MAKE) -C boot/efi
.PHONY: bootloader

run:
	$(MAKE_VARS) $(MAKE) -C boot/efi run
.PHONY: run