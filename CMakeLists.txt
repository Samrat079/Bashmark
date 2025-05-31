cmake_minimum_required(VERSION 3.10)
project(bashmark VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Default to Debug build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(bashmark ${SOURCES})

# Link OpenCL
find_package(OpenCL REQUIRED)
target_link_libraries(bashmark PRIVATE OpenCL::OpenCL)

# Link pthread (needed for std::thread)
find_package(Threads REQUIRED)
target_link_libraries(bashmark PRIVATE Threads::Threads)

# Link filesystem for older GCC versions
if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
    target_link_libraries(bashmark stdc++fs)
endif()

# Disable optimizations for specific file
set_source_files_properties(src/singlecore.cpp PROPERTIES COMPILE_FLAGS "-O0")

# ===== INSTALLATION RULES =====

# Binary
install(TARGETS bashmark DESTINATION bin)

# Assets
install(DIRECTORY assets/ DESTINATION share/bashmark/assets
        FILES_MATCHING PATTERN "*")

# Kernels
install(DIRECTORY kernels/ DESTINATION share/bashmark/kernels
        FILES_MATCHING PATTERN "*")

add_compile_definitions(DEFAULT_ASSET_PATH=\"/usr/share/bashmark/assets\")

add_compile_definitions(DEFAULT_ASSET_PATH=\"${CMAKE_INSTALL_PREFIX}/share/bashmark/assets\")



        
# ===== OPTIONAL: Packaging with CPack =====
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "bashmark")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "DEB;TGZ")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name")
include(CPack)
