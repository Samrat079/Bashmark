#include <iostream>
#include <thread>

#include "single_core.hpp"
#include "multicore.hpp"
#include "gpu_benchmark.hpp"
#include "full_benchmark.hpp"
#include "ui.hpp"  // Declare print_logo and print_menu here

int main() {
    print_logo("../assets/ascii_logo.txt");
    print_menu("../assets/menu.txt");

    char choice;

    while (true) {
        

        std::cout << "Enter choice (q to exit)[1-4]: ";
        std::cin >> choice;

        if (choice == '1') {
            run_single_core_benchmark();
        } else if (choice == '2') {
            run_multi_core_benchmark();
        } else if (choice == '3') {
            run_gpu_benchmark();
        } else if (choice == '4') {
            run_full_benchmark();
        } else if (choice == 'q') {
            std::cout << "Exiting program.\n";
            break;
        } else {
            std::cout << "Invalid choice.";
        }
    }

    return 0;
}
