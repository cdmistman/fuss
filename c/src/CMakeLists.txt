add_executable(fuss fuss.c)

add_subdirectory(cstack)
add_subdirectory(svec)

target_link_libraries(fuss cstack svec)

