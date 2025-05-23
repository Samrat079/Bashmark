#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

void run_single_core_benchmark() {
    const int windowSeconds = 1;
    const int minRunTime = 5;
    double baselineIPS = 0;
    double result = 0;
    unsigned long long iterationCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto lastWindowTime = startTime;

    std::cout << "\nStarting single-core benchmark...\n";

    while (true) {
        // Simulate a CPU-intensive task
        result = std::sin(result) + std::cos(result);
        iterationCount++;

        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        auto windowElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastWindowTime).count();

        if (windowElapsed >= windowSeconds) {
            double ips = iterationCount / static_cast<double>(windowElapsed);
            std::cout << "\r[Elapsed " << elapsed << "s] IPS: " << ips
                      << " | Result: " << result << "     " << std::flush;

            if (baselineIPS == 0 && elapsed >= minRunTime) {
                baselineIPS = ips;
                std::cout << "\nInitial performance baseline set at " << baselineIPS << " IPS.\n";
            } else if (baselineIPS > 0 && ips < baselineIPS * 0.60) {
                std::cout << "\n  Throttling detected. Performance dropped below 60% of baseline.\n";
                break;
            }

            iterationCount = 0;
            lastWindowTime = now;
        }
    }

    std::cout << "\n✅ Benchmark completed.\n";
}
