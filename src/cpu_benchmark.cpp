#include <iostream>
#include <cmath>
#include <chrono>
#include "cpu_benchmark.hpp"

void benchmark_worker() {
    double result = 0.0;
    auto start = std::chrono::high_resolution_clock::now();

    while (true) {
        result = std::sin(result) + std::cos(result);

        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

        if (elapsed >= 120) break;
    }

    std::cout << "Thread completed. Final result: " << result << "\n";
}
