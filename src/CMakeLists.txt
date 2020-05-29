add_library(format STATIC table.cpp table.h trace.cpp trace.h)
target_link_libraries(format logic parser filemap)
add_executable(c45 c45.cpp)
target_link_libraries(c45 format)
