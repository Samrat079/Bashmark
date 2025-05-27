#include "single_core.hpp"
#include "cpu_benchmark.hpp"
#include <iostream>


void run_single_core_benchmark() {
        std::cout << "\nStarting single cpu thread\n";

    benchmark_worker();
}
