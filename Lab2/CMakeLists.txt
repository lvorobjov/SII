cmake_minimum_required(VERSION 3.12)
project(lab2)
list(APPEND CMAKE_MODULE_PATH "$ENV{WORK}/cmake")
include(CsoiTEX)
include(CsoiWin)

set(CMAKE_EXE_LINKER_FLAGS_RELEASE  "-s -Wl,--gc-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -masm=intel")

aux_source_directory(src SRCS)
add_executable(${PROJECT_NAME} WIN32 ${SRCS} 
    src/${PROJECT_NAME}.rc)
target_link_libraries(${PROJECT_NAME} comctl32)

add_tex(report report.ad 1.tex 2.tex 3.tex 4.tex 5.tex)