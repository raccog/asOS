# Binaries
CURL := curl
TAR := tar

# Target defaults to i686-elf
TARGET ?= i686-elf

# Host system
HOST_OS ?= $(shell uname)
HOST_MACH ?= $(shell uname -m)
ifeq ($(HOST_OS),Darwin)
ifeq ($(HOST_MACH),arm64)
	MACOS_ARM := true
endif
endif

# Toolchain target versions
BINUTILS := binutils-2.37
GCC := gcc-11.2.0
GMP := gmp-6.2.1
MPFR := mpfr-4.1.0
MPC := mpc-1.2.1

# Toolchain tarballs
BINUTILS_TAR := $(BINUTILS).tar.gz
GCC_TAR := $(GCC).tar.gz
GMP_TAR := $(GMP).tar.xz
MPFR_TAR := $(MPFR).tar.gz
MPC_TAR := $(MPC).tar.gz

# Paths
PREFIX ?= $(shell pwd)/../../../Toolchain
SRC_PATH := $(PREFIX)/src
TARGET_DIR := target-$(TARGET)
GCC_PATH := $(SRC_PATH)/$(GCC)
ifeq ($(HOST_OS),Darwin)
	ifeq ($(HOST_MACH),arm64)
		GCC_PATH := $(SRC_PATH)/gcc-darwin-arm64
	endif
endif

# URLs
BINUTILS_URL := https://ftp.gnu.org/gnu/binutils/$(BINUTILS_TAR)
GCC_URL := https://ftp.gnu.org/gnu/gcc/$(GCC)/$(GCC_TAR)
GMP_URL := https://ftp.gnu.org/gnu/gmp/$(GMP_TAR)
MPFR_URL := https://www.mpfr.org/mpfr-current/$(MPFR_TAR)
MPC_URL := https://ftp.gnu.org/gnu/mpc/$(MPC_TAR)

# Flags
MAKE_FLAGS := -j$(shell nproc)
BINUTILS_FLAGS := --target=$(TARGET) --prefix=$(PREFIX) --with-sysroot --disable-nls --disable-werror
GCC_FLAGS := --target=$(TARGET) --prefix=$(PREFIX) --disable-nls --enable-languages=c,c++ --without-headers
ifeq ($(HOST_OS),Darwin)
	GCC_FLAGS += --with-gmp=$(PREFIX) --with-mpc=$(PREFIX) --with-mpfr=$(PREFIX)
endif

#
# Main rules
#

all: $(SRC_PATH) gcc

ifeq ($(HOST_OS),Darwin)
dependencies: gmp mpc mpfr
else
dependencies:
endif

.PHONY: all dependencies gcc binutils gmp mpc mpfr gcc-deps

#
# Directories
#

$(PREFIX):
	@mkdir -p $@

$(SRC_PATH): $(PREFIX)
	@mkdir -p $@

#
# Binutils and GCC
#

binutils: $(SRC_PATH)/$(BINUTILS)/build/binutils
	@echo "Installing binutils"
	$(MAKE) -C $(SRC_PATH)/$(BINUTILS)/build install

$(SRC_PATH)/$(BINUTILS_TAR): $(SRC_PATH) dependencies
	@echo "Downloading binutils"
	$(CURL) $(BINUTILS_URL) -o $@

$(SRC_PATH)/$(BINUTILS): $(SRC_PATH)/$(BINUTILS_TAR)
	@echo "Unpacking binutils"
	$(TAR) xf $< -C $(SRC_PATH)

$(SRC_PATH)/$(BINUTILS)/build: $(SRC_PATH)/$(BINUTILS)
	@mkdir $@

$(SRC_PATH)/$(BINUTILS)/build/Makefile: $(SRC_PATH)/$(BINUTILS)/build
	@echo "Configuring binutils"
	cd $< && ../configure $(BINUTILS_FLAGS)

$(SRC_PATH)/$(BINUTILS)/build/binutils: $(SRC_PATH)/$(BINUTILS)/build/Makefile
	@echo "Building binutils"
	$(MAKE) -C $(SRC_PATH)/$(BINUTILS)/build $(MAKE_FLAGS)

gcc: $(GCC_PATH)

ifdef MACOS_ARM
gcc-deps: binutils dependencies
	@echo "Downloading gcc"
	git clone https://github.com/iains/gcc-darwin-arm64.git $(GCC_PATH)
else
gcc-deps: $(SRC_PATH)/$(GCC_TAR) binutils dependencies
	@echo "Unpacking gcc"
	$(TAR) xf $< -C $(SRC_PATH)
endif

$(GCC_PATH): gcc-deps
	@echo "Building gcc"
	cd $@ && mkdir build && cd build && ../configure $(GCC_FLAGS)
	$(MAKE) -C $@/build $(MAKE_FLAGS) all-gcc
	$(MAKE) -C $@/build $(MAKE_FLAGS) all-target-libgcc
	@echo "Installing gcc"
	$(MAKE) -C $@/build install-gcc
	$(MAKE) -C $@/build install-target-libgcc
	

$(SRC_PATH)/$(GCC_TAR):
	@echo "Downloading gcc"
	$(CURL) $(GCC_URL) -o $@

#
# MacOS prerequisites
#

gmp: $(SRC_PATH)/$(GMP)

$(SRC_PATH)/$(GMP): $(SRC_PATH)/$(GMP_TAR)
	@echo "Unpacking gmp"
	$(TAR) xf $< -C $(SRC_PATH)
	@echo "Building gmp"
	cd $@ && ./configure --prefix=$(PREFIX)
	$(MAKE) -C $@ $(MAKE_FLAGS)
	@echo "Installing gmp"
	$(MAKE) -C $@ install

$(SRC_PATH)/$(GMP_TAR): $(SRC_PATH)
	@echo "Downloading gmp"
	$(CURL) $(GMP_URL) -o $@

mpfr: $(SRC_PATH)/$(MPFR)

$(SRC_PATH)/$(MPFR): $(SRC_PATH)/$(MPFR_TAR) gmp
	@echo "Unpacking mpfr"
	$(TAR) xf $< -C $(SRC_PATH)
	@echo "Building mpfr"
	cd $@ && ./configure --prefix=$(PREFIX) --with-gmp=$(PREFIX)
	$(MAKE) -C $@ $(MAKE_FLAGS)
	@echo "Installing mpfr"
	$(MAKE) -C $@ install

$(SRC_PATH)/$(MPFR_TAR): $(SRC_PATH)
	@echo "Downloading mpfr"
	$(CURL) $(MPFR_URL) -o $@

mpc: $(SRC_PATH)/$(MPC)

$(SRC_PATH)/$(MPC): $(SRC_PATH)/$(MPC_TAR) gmp mpfr
	@echo "Unpacking mpc"
	$(TAR) xf $< -C $(SRC_PATH)
	@echo "Building mpc"
	cd $@ && ./configure --prefix=$(PREFIX) --with-gmp=$(PREFIX) --with-mpfr=$(PREFIX)
	$(MAKE) -C $@ $(MAKE_FLAGS)
	@echo "Installing mpc"
	$(MAKE) -C $@ install

$(SRC_PATH)/$(MPC_TAR): $(SRC_PATH)
	@echo "Downloading mpc"
	$(CURL) $(MPC_URL) -o $@

