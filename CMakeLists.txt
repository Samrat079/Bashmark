cmake_minimum_required(VERSION 3.16)
project(bashmark VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define license
set(PROJECT_LICENSE "MIT")  # Change if you use a different license

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

# Compiler flags
target_compile_options(bashmark PRIVATE -Wall -Wextra -O3)

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Optional: Create kernels directory for GPU OpenCL kernels
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/kernels)

# Install configuration
install(TARGETS bashmark
    RUNTIME DESTINATION bin
)

install(DIRECTORY assets/
    DESTINATION share/bashmark/assets
    FILES_MATCHING PATTERN "*"
)

# CPack configuration for creating packages
set(CPACK_GENERATOR "TGZ;RPM;DEB")

# Package basic metadata
set(CPACK_PACKAGE_NAME "bashmark")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CPU and GPU Benchmarking Tool")
set(CPACK_PACKAGE_DESCRIPTION "A fast terminal-based CPU and GPU benchmarking tool using OpenCL.")
set(CPACK_PACKAGE_VENDOR "Samrat")
set(CPACK_PACKAGE_LICENSE "${PROJECT_LICENSE}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/license.txt")

# Contact / Maintainer info
set(CPACK_PACKAGE_CONTACT "samratdutta079@gmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Samrat")

# Package file names
set(CPACK_PACKAGE_FILE_NAME "bashmark-${PROJECT_VERSION}")
set(CPACK_RPM_FILE_NAME "bashmark-${PROJECT_VERSION}.rpm")
set(CPACK_DEBIAN_FILE_NAME "bashmark-${PROJECT_VERSION}.deb")

# Dependencies

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libstdc++6, libgcc1, ocl-icd-libopencl1, intel-opencl, nvidia-opencl-icd, rocm-opencl-runtime, pocl-opencl-icd")
set(CPACK_RPM_PACKAGE_DEPENDS "libc6, libstdc++6, libgcc1, ocl-icd, intel-opencl, nvidia-opencl-icd, rocm-opencl-runtime, pocl")


# Include CPack last
include(CPack)

# Debug information
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "License: ${PROJECT_LICENSE}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenCL found: ${OpenCL_FOUND}")
if(OpenCL_FOUND)
    message(STATUS "OpenCL include dirs: ${OpenCL_INCLUDE_DIRS}")
    message(STATUS "OpenCL libraries: ${OpenCL_LIBRARIES}")
endif()

