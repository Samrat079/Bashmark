// main.cpp

#include <iostream>
#include "singlecore.hpp"
#include "input.hpp"
#include "ui.hpp"   // include for print_logo

int main() {
    // Print ASCII logo from assets folder
    print_logo("../assets/ascii_logo.txt");

    char choice;
        
    while (true) {
        std::cout << "\nWhat would you like to do?\n";
        std::cout << "1. Single Core Benchmark\n";
        std::cout << "2. Multi Core Benchmark (Not implemented yet)\n";
        std::cout << "q. Quit\n";
        std::cout << "Enter choice: ";

        choice = wait_for_keypress();

        if (choice == '1') {
            std::cout << "\nStarting single core benchmark...\n";
            run_single_core_benchmark();
        } 
        else if (choice == '2') {
            std::cout << "\nMulti core benchmark not implemented yet.\n";
        } 
        else if (choice == 'q') {
            std::cout << "\nExiting program.\n";
            break;
        } 
        else {
            std::cout << "\nInvalid choice, please try again.\n";
        }
    }

    return 0;
}
