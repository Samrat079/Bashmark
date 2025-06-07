cmake_minimum_required(VERSION 3.16)
project(bashmark VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenCL REQUIRED)
find_package(Threads REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/cpu_benchmark.cpp
    src/gpu_benchmark.cpp
    src/multicore.cpp
    src/singlecore.cpp
    src/ui.cpp
)

# Create executable
add_executable(bashmark ${SOURCES})

# Link libraries
target_link_libraries(bashmark 
    OpenCL::OpenCL
    Threads::Threads
)

# Include directories for target
target_include_directories(bashmark PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCL_INCLUDE_DIRS}
)

# Compiler flags
target_compile_options(bashmark PRIVATE -Wall -Wextra -O3)

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Install configuration
install(TARGETS bashmark
    RUNTIME DESTINATION bin
)

install(DIRECTORY assets/
    DESTINATION share/bashmark/assets
    FILES_MATCHING PATTERN "*"
)

# Optional: Create kernels directory for GPU OpenCL kernels
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/kernels)

# CPack configuration for creating packages
set(CPACK_PACKAGE_NAME "bashmark")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CPU and GPU Benchmarking Tool")
set(CPACK_PACKAGE_VENDOR "Your Name")
set(CPACK_GENERATOR "RPM")
set(CPACK_RPM_PACKAGE_MAINTAINER "Samrat")
set(CPACK_RPM_PACKAGE_DEPENDS "libc6, libstdc++6, libgcc1, ocl-icd-libopencl1")

include(CPack)

# Debug information
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenCL found: ${OpenCL_FOUND}")
if(OpenCL_FOUND)
    message(STATUS "OpenCL include dirs: ${OpenCL_INCLUDE_DIRS}")
    message(STATUS "OpenCL libraries: ${OpenCL_LIBRARIES}")
endif()