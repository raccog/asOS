cmake_minimum_required(VERSION 3.10)

project(asos VERSION 0.0.1 LANGUAGES C CXX ASM)

set(ASOS_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
set(ASOS_TOOLCHAIN_DIR "${ASOS_SOURCE_DIR}/toolchain")
set(ASOS_CMAKE_DIR "${ASOS_SOURCE_DIR}/cmake")

add_subdirectory(boot)

include(cmake/Toolchain.cmake)
add_executable(asos-kernel kernel/arch/i686/VGA.cpp kernel/arch/i686/Entry.cpp)
add_library(asos-kernel-boot kernel/arch/i686/Kboot.S)
add_dependencies(asos-kernel i686-bootloader)

set(ASOS_i686_LINKER_SCRIPT "${ASOS_SOURCE_DIR}/kernel/arch/i686/Linker.ld")

set_target_properties(asos-kernel PROPERTIES 
    COMPILE_FLAGS "-ffreestanding -Wall -Wextra"
    LINK_FLAGS "--oformat binary -T ${ASOS_i686_LINKER_SCRIPT}"
    LINK_DEPENDS "${ASOS_i686_LINKER_SCRIPT}")

target_link_libraries(asos-kernel PUBLIC asos-kernel-boot)
