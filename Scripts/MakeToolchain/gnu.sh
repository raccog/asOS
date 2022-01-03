#!/bin/bash

# Toolchain target versions
BINUTILS="binutils-2.37"
GCC="gcc-11.2.0"
GMP="gmp-6.2.1"
MPFR="mpfr-4.1.0"
MPC="mpc-1.2.1"

# Toolchain tarballs
BINUTILS_TAR="$BINUTILS.tar.gz"
GCC_TAR="$GCC.tar.gz"
GMP_TAR="$GMP.tar.xz"
MPFR_TAR="$MPFR.tar.gz"
MPC_TAR="$MPC.tar.gz"

# Extra
PCOUNT=`nproc`

# Ensure toolchain dir is set
if [[ -z $PREFIX ]]; then
	echo "Set PREFIX to the toolchain directory path."
	exit 1
fi

# Ensure a target is set
if [[ -z $TARGET ]]; then
	echo "Set TARGET to the target architecture."
	exit 2
fi

# Path variables
TARGET_DIR="target-$TARGET"
if [[ "`uname`" = "Darwin" && "`uname -p`" = "arm" ]]; then
	GCC_PATH="$PREFIX/src/gcc-darwin-arm64"
else
	GCC_PATH="$PREFIX/src/$GCC"
fi

# Flags
BINUTILS_FLAGS="--target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror"
GCC_FLAGS="--target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers"
if [[ "`uname`" = "Darwin" ]]; then
	GCC_FLAGS="$GCC_FLAGS --with-gmp=$PREFIX --with-mpc=$PREFIX --with-mpfr=$PREFIX"
fi

# Move to toolchain dir
if [[ ! -d $PREFIX ]]; then
	mkdir $PREFIX
fi
cd $PREFIX

# Move to src dir
if [[ ! -d src ]]; then
	mkdir src
fi
cd src

# Download prerequisites on macos
if [[ "`uname`" = "Darwin" ]]; then
	echo "Downloading gmp, mpc, and mpfr"
	curl -O "https://ftp.gnu.org/gnu/gmp/$GMP_TAR"
	curl -O "https://www.mpfr.org/mpfr-current/$MPFR_TAR"
	curl -O "https://ftp.gnu.org/gnu/mpc/$MPC_TAR"

	echo "Extracting $GMP, $MPC, and $MPFR to directory $PREFIX/src."
	tar xf "$GMP_TAR"
	tar xf "$MPFR_TAR"
	tar xf "$MPC_TAR"
fi

# Download binutils
echo "Downloading binutils."
curl -O "https://ftp.gnu.org/gnu/binutils/$BINUTILS_TAR"

# Extract binutils
echo "Extracting $BINUTILS to directory $PREFIX/src."
tar xf "$BINUTILS_TAR"

# Download GCC
echo "Downloading $GCC."
if [[ "`uname`" = "Darwin" && "`uname -p`" = "arm" ]]; then
	git clone https://github.com/iains/gcc-darwin-arm64.git
else
	echo "Extracting gcc to directory $PREFIX/src."
	curl -O "https://ftp.gnu.org/gnu/gcc/$GCC/$GCC_TAR"
	tar xf "$GCC_TAR"
fi

# Move to target build dir
mkdir build
cd build

# Build gnu toolchain
mkdir $TARGET_DIR
cd $TARGET_DIR

# Configure and build macos prerequisites
if [[ "`uname`" = "Darwin" ]]; then
	echo "Building $GMP"
	mkdir $GMP
	cd $GMP
	$PREFIX/src/$GMP/configure --prefix="$PREFIX"
	make -j4
	make install
	cd ..

	echo "Building $MPFR"
	mkdir $MPFR
	cd $MPFR
	$PREFIX/src/$MPFR/configure --prefix="$PREFIX" --with-gmp="$PREFIX"
	make -j4
	make install
	cd ..

	echo "Building $MPC"
	mkdir $MPC
	cd $MPC
	$PREFIX/src/$MPC/configure --prefix="$PREFIX" --with-gmp="$PREFIX" --with-mpfr="$PREFIX"
	make -j4
	make install
	cd ..
fi

# Configure and build binutils
echo "Building $BINUTILS."
mkdir $BINUTILS
cd $BINUTILS
$PREFIX/src/$BINUTILS/configure $BINUTILS_FLAGS
make -j$PCOUNT
make install
cd ..

# Configure and build gcc
echo "Building $GCC."
mkdir $GCC
cd $GCC
$GCC_PATH/configure $GCC_FLAGS
make all-gcc -j$PCOUNT
make all-target-libgcc -j$PCOUNT
make install-gcc
make install-target-libgcc
