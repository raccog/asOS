cmake_minimum_required(VERSION 3.10)

project(asos-bootloader VERSION 0.0.1 LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include("${ASOS_CMAKE_DIR}/Toolchain.cmake")

set(ASOS_BOOT_DIR "${ASOS_SOURCE_DIR}/boot")
set(BOOT_i686_LINKER_SCRIPT "${ASOS_BOOT_DIR}/i686/linker.ld")

add_executable(i686-bootloader i686/boot.S i686/boot2.S i686/pmode.S)

set_target_properties(i686-bootloader PROPERTIES 
    LINK_FLAGS "--oformat binary -T ${BOOT_i686_LINKER_SCRIPT}"
    LINK_DEPENDS "${BOOT_i686_LINKER_SCRIPT}")
