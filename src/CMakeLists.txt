add_library(format STATIC table.cpp table.h trace.cpp trace.h)
target_link_libraries(format logic parser filemap)
add_executable(c45 c45.cpp)
target_link_libraries(c45 format)
add_custom_command(TARGET c45
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c45> ${CMAKE_SOURCE_DIR}/c45)
