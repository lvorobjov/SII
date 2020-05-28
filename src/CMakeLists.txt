add_library(format STATIC parser.cpp parser.h table.cpp table.h trace.cpp trace.h)
target_link_libraries(format logic filemap)
add_executable(c45 c45.cpp)
target_link_libraries(c45 format)
