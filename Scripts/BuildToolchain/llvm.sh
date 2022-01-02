#!/bin/bash

# Flags
LLVM_FLAGS="-G 'Unix Makefiles' -DCMAKE_INSTALL_PREFIX='$PREFIX' -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS='clang;compiler-rt;libcxx;libcxxabi'"

# Extras
PCOUNT=`nproc`
LLVM_BRANCH="release/13.x"

# Ensure toolchain dir is set
if [[ -z $PREFIX ]]; then
	echo "Set PREFIX to the toolchain directory path."
	exit 1
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

# Download llvm toolchain
echo "Downloading llvm project from github."
git clone "https://github.com/llvm/llvm-project.git"

# Build llvm toolchain
echo "Building llvm project branch $LLVM_BRANCH."
cd llvm-project
git switch $LLVM_BRANCH
cmake -S llvm -B build $LLVM_FLAGS
cmake --build build -j$PCOUNT
cmake --install build
