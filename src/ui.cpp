#include "ui.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // for getenv
#include <filesystem> // C++17
#if defined(__linux__)
#include <unistd.h>
#include <limits.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

namespace fs = std::filesystem;

std::string getExecutableDir() {
    char buffer[PATH_MAX];

#if defined(__linux__)
    ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
    if (count <= 0) throw std::runtime_error("Failed to get executable path");
    return fs::path(std::string(buffer, count)).parent_path().string();

#elif defined(__APPLE__)
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) != 0) {
        throw std::runtime_error("Buffer too small for executable path");
    }
    return fs::path(buffer).parent_path().string();

#elif defined(_WIN32)
    GetModuleFileNameA(NULL, buffer, PATH_MAX);
    return fs::path(buffer).parent_path().string();

#else
    throw std::runtime_error("Unsupported platform");
#endif
}

std::string getAssetsPath() {
    const char* overridePath = std::getenv("BASHMARK_ASSETS_PATH");
    if (overridePath && fs::exists(overridePath)) {
        return std::string(overridePath);
    }

    std::string systemPath = "/usr/local/share/bashmark/assets";
    if (fs::exists(systemPath)) {
        return systemPath;
    }

    std::string localPath = getExecutableDir() + "/assets";
    if (fs::exists(localPath)) {
        return localPath;
    }

    throw std::runtime_error("Assets directory not found in any expected location.");
}


void print_file(const std::string& filename) {
    std::string path = getAssetsPath() + "/" + filename;
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Error: could not open file: " << path << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
}

void print_logo() {
    print_file("ascii_logo.txt");
}

void print_menu() {
    print_file("menu.txt");
}
