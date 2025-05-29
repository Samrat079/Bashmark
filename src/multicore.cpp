#include "multicore.hpp"
#include "cpu_benchmark.hpp"
#include <iostream>
#include <thread>
#include <vector>

void run_multi_core_benchmark(int threadCount) {
    if (threadCount == 0) {
        threadCount = std::thread::hardware_concurrency();
        if (threadCount == 0) threadCount = 2;
    }

    std::cout << "\nStarting with " << threadCount << " CPU threads...\n";

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back(benchmark_worker);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Multi-core benchmark finished.\n";
}
