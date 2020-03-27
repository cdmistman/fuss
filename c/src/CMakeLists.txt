cmake_minimum_required (VERSION 3.10)
project (fuss)
set (fuss_VERSION_MAJOR 0)
set (fuss_VERSION_MINOR 1)

add_executable(fuss fuss.c)

add_subdirectory(cstack)
add_subdirectory(svec)

target_link_libraries(fuss cstack svec)

