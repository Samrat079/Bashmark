#include <atomic>
#include <iostream>
#include <cmath>
#include <chrono>
#include "input.hpp"  // for wait_for_keypress_non_blocking()
#include "cpu_benchmark.hpp"

void benchmark_worker(std::atomic<bool>& stop_flag) {
    double result = 0.0;
    long long counter = 0;
    auto start = std::chrono::high_resolution_clock::now();


    while (!stop_flag) {
        result = std::sin(result) + std::cos(result);
        counter++;

        if (counter % 100000 == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
   
            if (elapsed >= 120 || stop_flag.load()) break;
            // Optional: print progress or score here if you want
        }
    }
    std::cout << std::endl; 

    int score = static_cast<int>(std::abs(result * 1000)) % 100000;
    std::cout << "\nBenchmark ended. Score: " << score << "\n";
}
