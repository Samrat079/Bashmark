cmake_minimum_required(VERSION 3.10)
project(bashmark)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(bashmark ${SOURCES})
target_link_libraries(bashmark ncurses)

set_source_files_properties(src/singlecore.cpp PROPERTIES COMPILE_FLAGS "-O0")
