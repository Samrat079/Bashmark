#include <iostream>
#include <vector>
#include <chrono>
#include <atomic>
#include <cmath>
#include <thread>

void benchmark_worker(std::atomic<bool>& stop_flag) {
    constexpr int N = 128;  // Matrix size (N x N)
    using Matrix = std::vector<std::vector<float>>;

    // Initialize matrices A and B
    Matrix A(N, std::vector<float>(N, 1.1f));
    Matrix B(N, std::vector<float>(N, 2.2f));
    Matrix C(N, std::vector<float>(N, 0.0f)); // Result matrix

    long long mult_count = 0;
    auto start = std::chrono::high_resolution_clock::now();

    while (!stop_flag) {
        // Perform matrix multiplication: C = A * B
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                float sum = 0.0f;
                for (int k = 0; k < N; ++k) {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }

        mult_count++;

        if (mult_count % 10 == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
            if (elapsed >= 120 || stop_flag.load()) break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto total_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    double mults_per_sec = static_cast<double>(mult_count) / (total_elapsed / 1000.0);

    std::cout << "Thread completed. Matrices multiplied: " << mult_count << ", ";
    std::cout << "Performance score: " << static_cast<int>(mults_per_sec) << " multiplications/sec\n";
}
