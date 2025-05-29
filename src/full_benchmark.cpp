#include <thread>
#include <iostream>
#include "gpu_benchmark.hpp"
#include "multicore.hpp"

void run_full_benchmark() {
    std::thread gpu_thread(run_gpu_benchmark);
    std::thread cpu_thread(run_multi_core_benchmark, 0); // pass 0 to auto-detect cores

    gpu_thread.join();
    cpu_thread.join();

    std::cout << "Full benchmark (GPU + Multi-Core) completed.\n";
}
