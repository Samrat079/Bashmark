#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib> // for getenv
#include <filesystem> // C++17
#include <stdexcept>

#if defined(__linux__)
#include <unistd.h>
#include <limits.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(_WIN32)
#include <windows.h>
#include <libloaderapi.h>
#endif

namespace fs = std::filesystem;

// Function to get the directory of the executable
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
    if (GetModuleFileNameA(NULL, buffer, PATH_MAX) == 0) {
        throw std::runtime_error("Failed to get executable path");
    }
    return fs::path(buffer).parent_path().string();

#else
    throw std::runtime_error("Unsupported platform");
#endif
}

// Function to determine the assets path
std::string getAssetsPath() {
    // 1. Check BASHMARK_ASSETS_PATH environment variable
    if (const char* overridePath = std::getenv("BASHMARK_ASSETS_PATH")) {
        if (fs::exists(overridePath)) {
            return std::string(overridePath);
        }
    }

    // 2. User-specific data directory (~/.local/share/bashmark/assets)
    if (const char* home = std::getenv("HOME")) {
        fs::path userPath = fs::path(home) / ".local/share/bashmark/assets";
        if (fs::exists(userPath)) {
            return userPath.string();
        }
    }

    const std::vector<fs::path> systemPaths = {
        "/home/linuxbrew/.linuxbrew/share/bashmark/assets", // Linuxbrew
        "/usr/local/share/bashmark/assets",                 // System install
        "/usr/share/bashmark/assets"                        // Fallback
    };
    
    for (const auto& path : systemPaths) {
        if (fs::exists(path)) {
            return path.string();
        }
    }

    // 4. XDG data directory (e.g., for Flatpak or sandboxed environments)
    fs::path xdgDataHome;
    if (const char* xdg = std::getenv("XDG_DATA_HOME")) {
        xdgDataHome = xdg;
    } else if (const char* home = std::getenv("HOME")) {
        xdgDataHome = fs::path(home) / ".local/share";
    }
    if (!xdgDataHome.empty()) {
        fs::path flatpakPath = xdgDataHome / "bashmark/assets";
        if (fs::exists(flatpakPath)) {
            return flatpakPath.string();
        }
    }

    // 5. Relative to the executable's directory
    try {
        fs::path execDir = getExecutableDir();
        fs::path relativePath = execDir / "assets";
        if (fs::exists(relativePath)) {
            return relativePath.string();
        }
    } catch (...) {
        // Ignore errors and proceed to throw below
    }

    throw std::runtime_error("Assets directory not found in any expected location.");
}

// Function to print the contents of a file
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

// Function to print the ASCII logo
void print_logo(const std::string& assetsPath) {
    std::ifstream file(assetsPath + "/ascii_logo.txt");
    if (!file) {
        throw std::runtime_error("Could not open logo file");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}


void print_menu(const std::string& assetsPath) {
    std::ifstream file(assetsPath + "/menu.txt");
    if (!file) {
        throw std::runtime_error("Could not open menu file");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}
