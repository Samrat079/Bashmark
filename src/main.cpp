#include <iostream>
#include "single_core.hpp"
#include "multicore.hpp"
#include "ui.hpp"  // Declare print_logo and print_menu here

int main() {
    print_logo("../assets/ascii_logo.txt");
    print_menu("../assets/menu.txt");

    char choice;

    while (true) {
        

        std::cout << "Enter choice (q to exit)[1-2]: ";
        std::cin >> choice;

        if (choice == '1') {
            run_single_core_benchmark();
        } else if (choice == '2') {
            run_multi_core_benchmark();
        } else if (choice == 'q') {
            std::cout << "Exiting program.\n";
            break;
        } else {
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
