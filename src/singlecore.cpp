#include "single_core.hpp"
#include "cpu_benchmark.hpp"
#include "input.hpp"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

void run_single_core_benchmark() {
    std::cout << "\nStarting single CPU thread. Press 'q' to terminate."<< std::endl;

    std::atomic<bool> stop_flag{false};
    std::thread worker(benchmark_worker, std::ref(stop_flag));

    // Main thread polls for 'q' keypress
    while (!stop_flag) {
        char ch = wait_for_keypress_non_blocking();
        if (ch == 'q' || ch == 'Q') {
            stop_flag = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    worker.join();
    std::cout << "\nReturning to main menu...\n";
}
