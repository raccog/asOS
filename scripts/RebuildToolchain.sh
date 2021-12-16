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

# Download binutils and gcc
	echo "Downloading binutils and gcc."
	curl -O "https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.gz"
	curl -O "https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.gz"

# Extract binutils and gcc
	echo "Extracting binutils and gcc to directory $PREFIX/src."
	tar xf "binutils-2.37.tar.gz"
	tar xf "gcc-11.2.0.tar.gz"
	cd ..
fi

# Move to target build dir
mkdir build
cd build

TARGET_DIR="target-$TARGET"
mkdir $TARGET_DIR
cd $TARGET_DIR

# Configure and build binutils
echo "Building binutils."
PCOUNT=`nproc`
mkdir binutils
cd binutils
$PREFIX/src/binutils-2.37/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j$PCOUNT
make install

# Configure and build gcc
echo "Building gcc."
cd ..
mkdir gcc
cd gcc
$PREFIX/src/gcc-11.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j$PCOUNT
make all-target-libgcc -j$PCOUNT
make install-gcc
make install-target-libgcc

