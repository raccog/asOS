#!/bin/sh

if [ ! -d build ]; then
    echo "Run this script in the source directory."
    exit 1
fi

toolchain/bin/i686-elf-objcopy -I binary -O binary --pad-to 0xe00 build/boot/i686-bootloader build/asos.img
cat build/kernel/asos-kernel >> build/asos.img
