#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include "cpu_benchmark.hpp"
#include "input.hpp" // for wait_for_keypress_non_blocking

void run_multi_core_benchmark(int threadCount) {
    if (threadCount == 0) {
        threadCount = std::thread::hardware_concurrency();
        if (threadCount == 0) threadCount = 2;
    }

    std::cout << "\nStarting with " << threadCount << " CPU threads... Press 'q' terminate." << std::endl;

    std::atomic<bool> stop_flag{false};
    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back(benchmark_worker, std::ref(stop_flag));
    }


    while (!stop_flag) {
        char ch = wait_for_keypress_non_blocking();
        if (ch == 'q' || ch == 'Q') {
            stop_flag = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "Multicore benchmark ended.\n";
}
