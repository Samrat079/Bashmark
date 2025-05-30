#include <iostream>
#include <thread>
#include <string>
#include "single_core.hpp"
#include "multicore.hpp"
#include "gpu_benchmark.hpp"
#include "full_benchmark.hpp"
#include "ui.hpp"  // Declare print_logo and print_menu here

int main() {
    print_logo();
    

    std::string choice;

    while (true) {

        print_menu();

        std::cout << "What would you like to do [1-4](q to quit): ";

        std::getline(std::cin, choice);

        if (choice == "1") {
            run_single_core_benchmark();
        } else if (choice == "2") {
            run_multi_core_benchmark();
        } else if (choice == "3") {
            run_gpu_benchmark();
        } else if (choice == "4") {
            run_full_benchmark();
        } else if (choice == "q" || choice == "Q") {
            std::cout << "Exiting program.\n";
            break;
        } else {
            std::cout << "Invalid choice. Please try again: ";
        }
    }

    return 0;
}
