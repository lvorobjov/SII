cmake_minimum_required(VERSION 3.12)
project(SII)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
include(CsoiWin)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
add_definitions(-D_UNICODE -DUNICODE)

add_subdirectory(Lab2/src)
add_subdirectory(Lab4/src)
