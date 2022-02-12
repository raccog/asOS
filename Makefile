# Makefile structure is inspired by brutal
# https://github.com/brutal-org/brutal
.SUFFIXES:
.DELETE_ON_ERROR:
.DEFAULT_GOAL := all

CC := clang
LD := clang
CFLAGS := -O2

BUILD_DIR := build
CACHE_DIR := $(BUILD_DIR)/cache

MKCWD = mkdir -p $(@D)

include boot/arch/x86_64/.build.mk

.PHONY: all
all: $(ALL)

.PHONY: clean
clean: 
	rm -rf $(BUILD_DIR)
