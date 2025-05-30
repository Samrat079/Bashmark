cmake_minimum_required(VERSION 3.10)
project(bashmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

file(GLOB SOURCES "src/*.cpp")

add_executable(bashmark ${SOURCES})

# Link OpenCL
find_package(OpenCL REQUIRED)
target_link_libraries(bashmark PRIVATE OpenCL::OpenCL)

# Link pthread (needed for std::thread)
find_package(Threads REQUIRED)
target_link_libraries(bashmark PRIVATE Threads::Threads)

# Disable optimizations for specific file if needed
set_source_files_properties(src/singlecore.cpp PROPERTIES COMPILE_FLAGS "-O0")

# Copy kernel files to build output
file(COPY kernels/gpu_benchmark.cl DESTINATION ${CMAKE_BINARY_DIR}/kernels)

# Optional: place final binary in project root instead of build/
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
