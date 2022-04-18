cmake_minimum_required (VERSION 3.13.4)
project(hashTbl)

set (CMAKE_BUILD_TYPE Debug)

set (SOURCES sources/main.cpp sources/hashTbl.cpp sources/stack.cpp sources/list.cpp sources/hash.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE includes)

add_compile_options (-g -Werror -Wall -Wextra -Wpedantic)
