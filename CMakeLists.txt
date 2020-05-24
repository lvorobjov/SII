cmake_minimum_required(VERSION 3.12)
project(SII)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
include(CsoiWin)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
option(UNICODE "Use UTF-16 strings" ON)
if (UNICODE)
    add_definitions(-D_UNICODE -DUNICODE)
endif()
set(CMAKE_EXE_LINKER_FLAGS_RELEASE  "-static -s -Wl,--gc-sections")

add_subdirectory(Lab2/src)
add_subdirectory(Lab4/src)
