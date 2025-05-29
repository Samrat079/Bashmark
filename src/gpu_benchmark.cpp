#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <vector>
#include <cmath>

const int ELEMENTS = 1024 * 1024; // 1 million elements
std::vector<float> data(ELEMENTS, 1.0f);

// Helper to load the kernel source code from file
std::string load_kernel_source(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

float measure_kernel_time(cl_command_queue queue, cl_kernel kernel, cl_mem buffer, size_t global_work_size) {
    cl_int err;
    auto start = std::chrono::high_resolution_clock::now();
    err = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &global_work_size, nullptr, 0, nullptr, nullptr);
    clFinish(queue);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<float>(end - start).count();
}


void run_gpu_benchmark() {
    std::string source = load_kernel_source("kernels/gpu_benchmark.cl");
    const char* src = source.c_str();

    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;

    clGetPlatformIDs(1, &platform, nullptr);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);

    context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
    queue = clCreateCommandQueue(context, device, 0, &err);

    const int ELEMENTS = 1024 * 1024;
    std::vector<float> data(ELEMENTS, 1.0f);
    cl_mem buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                   sizeof(float) * ELEMENTS, data.data(), &err);

    cl_program program = clCreateProgramWithSource(context, 1, &src, nullptr, &err);
    clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
    cl_kernel kernel = clCreateKernel(program, "gpu_benchmark", &err);
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);

        const int WARMUP_ROUNDS = 10;
    const size_t global_work_size = ELEMENTS;
        
    float min_time = std::numeric_limits<float>::max();
    int iteration = 0;
        
    // Warm up the GPU
    for (int i = 0; i < WARMUP_ROUNDS; ++i) {
        float time = measure_kernel_time(queue, kernel, buffer, global_work_size);
        if (time < min_time) min_time = time;
    }

        std::cout << "Warm-up complete. Baseline time: " << min_time << " seconds\n";

    while (true) {
        float current_time = measure_kernel_time(queue, kernel, buffer, global_work_size);
        float performance_ratio = (min_time / current_time) * 100.0f;

        std::cout << "\rIteration: " << ++iteration
                  << " | Time: " << current_time
                  << " | Performance: " << performance_ratio << "%" << std::flush;

        if (performance_ratio <= 50.0f) {
            std::cout << "\n\nGPU performance dropped below 50%. Benchmark complete.\n";
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseMemObject(buffer);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}

