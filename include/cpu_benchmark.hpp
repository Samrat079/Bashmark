// cpu_benchmark.hpp
#ifndef CPU_BENCHMARK_HPP
#define CPU_BENCHMARK_HPP

#include <atomic>

void benchmark_worker(std::atomic<bool>& stop_flag);

#endif
