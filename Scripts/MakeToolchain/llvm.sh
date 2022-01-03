#!/bin/bash

# Toolchain versions
LLVM="llvm-project-13.0.0"

# Paths
SRC_DIR="$PREFIX/src"
TARBALL_DIR="$PREFIX/tarballs"
LLVM_DIR="$SRC_DIR/llvm-project"

# Tarballs
LLVM_TAR="$LLVM.tar.xz"

# Flags
LLVM_FLAGS="-DCMAKE_INSTALL_PREFIX='$PREFIX' -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS='clang;compiler-rt;libcxx;libcxxabi'"

# URLs
LLVM_URL="https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/$LLVM.src.tar.xz"

# Extras
PCOUNT=`nproc`

# Ensure toolchain dir is set
if [[ -z $PREFIX ]]; then
	echo "Set PREFIX to the toolchain directory path."
	exit 1
fi

# Create directories that do not exist
if [[ ! -d $PREFIX ]]; then
	mkdir $PREFIX
fi

if [[ ! -d $SRC_DIR ]]; then
	mkdir $SRC_DIR
fi

if [[ ! -d $TARBALL_DIR ]]; then
	mkdir $TARBALL_DIR
fi

# Download llvm toolchain
echo "Downloading llvm-project from github."
#git clone "https://github.com/llvm/llvm-project.git" $LLVM_DIR
curl -L "$LLVM_URL" -o "$TARBALL_DIR/$LLVM_TAR"

echo "Extracting llvm-project"
tar xf "$TARBALL_DIR/$LLVM_TAR" -C "$SRC_DIR"
exit 1

# Build and install llvm toolchain
echo "Building llvm-project."
cd $LLVM_DIR
cmake -S llvm -B build $LLVM_FLAGS
cmake --build build -j$PCOUNT
echo "Installing llvm toolchain"
cmake --install build
