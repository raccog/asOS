#!/bin/sh

if [[ -z $TARGET ]]; then
	echo "Set TARGET to the target architecture."
	exit 1
fi

if [[ -z $PREFIX ]]; then
	echo "Set PREFIX to the toolchain directory path."
	exit 2
fi

# Move to toolchain dir
mkdir $PREFIX
cd $PREFIX

if [[ ! -d src ]]; then

# Move to src dir
	mkdir src
	cd src

# Download prerequisites on macos
if [[ "`uname`" = "Darwin" ]]; then
	echo "Downloading gmp, mpc, and mpfr"
	curl -O "https://ftp.gnu.org/gnu/gmp/gmp-6.2.1.tar.xz"
	curl -O "https://www.mpfr.org/mpfr-current/mpfr-4.1.0.tar.gz"
	curl -O "https://ftp.gnu.org/gnu/mpc/mpc-1.2.1.tar.gz"

	echo "Extracting gmp, mpc, and mpfr to directory $PREFIX/src."
	tar xf "gmp-6.2.1.tar.xz"
	tar xf "mpfr-4.1.0.tar.gz"
	tar xf "mpc-1.2.1.tar.gz"
fi

# Download binutils
	echo "Downloading binutils."
	curl -O "https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.gz"

# Extract binutils
	echo "Extracting binutils to directory $PREFIX/src."
	tar xf "binutils-2.37.tar.gz"

# Download GCC
	echo "Downloading gcc."
	if [[ "`uname`" = "Darwin" && "`uname -p`" = "arm" ]]; then
		git clone https://github.com/iains/gcc-darwin-arm64.git
		GCC_PATH="$PREFIX/src/gcc-darwin-arm64"
	else
		echo "Extracting gcc to directory $PREFIX/src."
		curl -O "https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.gz"
		tar xf "gcc-11.2.0.tar.gz"
		GCC_PATH="$PREFIX/src/gcc-11.2.0"
	fi
	cd ..
fi

# Move to target build dir
mkdir build
cd build

TARGET_DIR="target-$TARGET"
mkdir $TARGET_DIR
cd $TARGET_DIR

# Configure and build macos prerequisites
echo "Building gmp"
mkdir gmp
cd gmp
$PREFIX/src/gmp-6.2.1/configure --prefix="$PREFIX"
make -j4
make install
cd ..

echo "Building mpfr"
mkdir mpfr
cd mpfr
$PREFIX/src/mpfr-4.1.0/configure --prefix="$PREFIX" --with-gmp="$PREFIX"
make -j4
make install
cd ..

echo "Building mpc"
mkdir mpc
cd mpc
$PREFIX/src/mpc-1.2.1/configure --prefix="$PREFIX" --with-gmp="$PREFIX" --with-mpfr="$PREFIX"
make -j4
make install
cd ..

# Configure and build binutils
echo "Building binutils."
PCOUNT=`nproc`
BINUTILS_FLAGS="--target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror"
mkdir binutils
cd binutils
$PREFIX/src/binutils-2.37/configure $BINUTILS_FLAGS
make -j$PCOUNT
make install
cd ..

# Configure and build gcc
echo "Building gcc."
GCC_FLAGS="--target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers"
mkdir gcc
cd gcc
if [[ "`uname`" = "Darwin" ]]; then
	GCC_FLAGS="$GCC_FLAGS --with-gmp=$PREFIX --with-mpc=$PREFIX --with-mpfr=$PREFIX"
fi
$GCC_PATH/configure $GCC_FLAGS
make all-gcc -j$PCOUNT
make all-target-libgcc -j$PCOUNT
make install-gcc
make install-target-libgcc

