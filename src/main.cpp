#include <iostream>
#include <thread>
#include <string>
#include "single_core.hpp"
#include "multicore.hpp"
#include "gpu_benchmark.hpp"
#include "full_benchmark.hpp"
#include "ui.hpp"  // Declare print_logo and print_menu here
#define ASSETS_DIR ASSET_PATH

#ifndef DEFAULT_ASSET_PATH
#define DEFAULT_ASSET_PATH "/usr/share/bashmark/assets"
#endif


int main() {
    std::string assetsDir = getAssetsPath();

    print_logo(assetsDir);
    print_menu(assetsDir);
    std::string choice;

    while (true) {

        std::cout << "What would you like to do [1-2](q to quit): ";
        std::getline(std::cin, choice);

        if (choice == "1") {
            run_single_core_benchmark();
        } else if (choice == "2") {
            run_multi_core_benchmark();
        } else if (choice == "q" || choice == "Q") {
            std::cout << "Exiting program......\n";
            break;
        } else {
            std::cout << "Invalid choice.";
        }
    }

    return 0;
}
