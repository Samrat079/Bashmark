#include "ui.hpp"
#include <iostream>
#include <fstream>

void print_logo(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Error: could not open logo file: " << path << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
}

void print_menu(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Error: could not open menu file: " << path << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
}
