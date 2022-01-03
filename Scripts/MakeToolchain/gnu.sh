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
    TARGET=i686-elf     # default is i686-elf
fi

# Path variables
BUILD_DIR="$PREFIX/build"
SRC_DIR="$PREFIX/src"
TARBALL_DIR="$PREFIX/tarballs"
TARGET_DIR="target-$TARGET"

# Flags
BINUTILS_FLAGS="--target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror"
GCC_FLAGS="--target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers"
if [[ "`uname`" = "Darwin" ]]; then
	GCC_FLAGS="$GCC_FLAGS --with-gmp=$PREFIX --with-mpc=$PREFIX --with-mpfr=$PREFIX"
fi
GMP_FLAGS="--prefix='$PREFIX'"
MPFR_FLAGS="--prefix='$PREFIX' --with-gmp='$PREFIX'"
MPC_FLAGS="--prefix='$PREFIX' --with-gmp='$PREFIX' --with-mpfr='$PREFIX'"

# Create directories that do not exist

if [[ ! -d $PREFIX ]]; then
	mkdir $PREFIX
fi

if [[ ! -d $BUILD_DIR ]]; then
	mkdir $BUILD_DIR
fi

if [[ ! -d $BUILD_DIR/$TARGET_DIR ]]; then
    mkdir $BUILD_DIR/$TARGET_DIR
fi

if [[ ! -d $SRC_DIR ]]; then
	mkdir $SRC_DIR
fi

if [[ ! -d $TARBALL_DIR ]]; then
    mkdir $TARBALL_DIR
fi

# Download prerequisites on macos
if [[ "`uname`" = "Darwin" ]]; then
	echo "Downloading gmp, mpc, and mpfr"
	curl "https://ftp.gnu.org/gnu/gmp/$GMP_TAR" -o "$TARBALL_DIR/$GMP_TAR"
	curl "https://www.mpfr.org/mpfr-current/$MPFR_TAR" -o "$TARBALL_DIR/$MPFR_TAR"
	curl "https://ftp.gnu.org/gnu/mpc/$MPC_TAR" -o "$TARBALL_DIR/$MPC_TAR"

	echo "Extracting $GMP, $MPC, and $MPFR to $SRC_DIR."
	tar xf "$TARBALL_DIR/$GMP_TAR" -C $SRC_DIR
	tar xf "$TARBALL_DIR/$MPFR_TAR" -C $SRC_DIR
	tar xf "$TARBALL_DIR/$MPC_TAR" -C $SRC_DIR
fi

# Download binutils
echo "Downloading binutils."
curl "https://ftp.gnu.org/gnu/binutils/$BINUTILS_TAR" -o "$TARBALL_DIR/$BINUTILS_TAR"

# Extract binutils
echo "Extracting $BINUTILS to $SRC_DIR."
tar xf "$TARBALL_DIR/$BINUTILS_TAR" -C "$SRC_DIR"

# Download GCC
echo "Downloading $GCC."
if [[ "`uname`" = "Darwin" && "`uname -p`" = "arm" ]]; then
	git clone https://github.com/iains/gcc-darwin-arm64.git "$SRC_DIR/$GCC"
else
	curl "https://ftp.gnu.org/gnu/gcc/$GCC/$GCC_TAR" -o "$TARBALL_DIR/$GCC_TAR"
	echo "Extracting gcc to directory $SRC_DIR."
	tar xf "$TARBALL_DIR/$GCC_TAR" -C "$SRC_DIR"
fi

# Configure and build macos prerequisites
if [[ "`uname`" = "Darwin" ]]; then
	echo "Building $GMP"
	mkdir -p $BUILD_DIR/$GMP
    cd $BUILD_DIR/$GMP
	$SRC_DIR/$GMP/configure $GMP_FLAGS
	make -j$PCOUNT
	make install

	echo "Building $MPFR"
	mkdir -p $BUILD_DIR/$MPFR
    cd $BUILD_DIR/$MPFR
	$SRC_DIR/$MPFR/configure $MPFR_FLAGS
	make -j$PCOUNT
	make install

	echo "Building $MPC"
	mkdir -p $BUILD_DIR/$MPC
    cd $BUILD_DIR/$MPC
	$SRC_DIR/$MPC/configure $MPC_FLAGS
	make -j$PCOUNT
	make install
fi

# Configure and build binutils
echo "Building $BINUTILS."
mkdir $BUILD_DIR/$BINUTILS
cd $BUILD_DIR/$BINUTILS
$SRC_DIR/$BINUTILS/configure $BINUTILS_FLAGS
make -j$PCOUNT
make install

# Configure and build gcc
echo "Building $GCC."
mkdir $BUILD_DIR/$GCC
cd $BUILD_DIR/$GCC
$SRC_DIR/$GCC/configure $GCC_FLAGS
make all-gcc -j$PCOUNT
make all-target-libgcc -j$PCOUNT
make install-gcc
make install-target-libgcc
