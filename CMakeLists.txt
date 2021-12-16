cmake_minimum_required(VERSION 3.10)

set(ASOS_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
set(ASOS_TOOLCHAIN_DIR "${ASOS_SOURCE_DIR}/toolchain")
set(ASOS_CMAKE_DIR "${ASOS_SOURCE_DIR}/cmake")

add_subdirectory(boot)
