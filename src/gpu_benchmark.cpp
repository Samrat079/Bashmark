#include <iostream>
#include <vector>
#include <chrono>
#include <atomic>
#include <string>
#include <sstream>
#include <thread>
#include "input.hpp"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#include <cstdlib>  // for getenv, setenv
#endif

    // OpenCL kernel for matrix multiplication
    const char* matrix_mult_kernel = R"(
    __kernel void matrix_multiply(__global const float* A,
                                 __global const float* B,
                                 __global float* C,
                                 const int N) {
        int row = get_global_id(0);
        int col = get_global_id(1);
        
        if (row < N && col < N) {
            float sum = 0.0f;
            for (int k = 0; k < N; ++k) {
                sum += A[row * N + k] * B[k * N + col];
            }
            C[row * N + col] = sum;
        }
    }
    )";
    
    // Error checking function
    void check_error(cl_int error, const std::string& operation) {
        if (error != CL_SUCCESS) {
            std::cerr << "OpenCL error in " << operation << ": " << error << std::endl;
            throw std::runtime_error("OpenCL error");
        }
    }
    
    std::string get_device_info(cl_device_id device, cl_device_info param) {
        size_t size;
        clGetDeviceInfo(device, param, 0, nullptr, &size);
        std::string result(size, '\0');
        clGetDeviceInfo(device, param, size, &result[0], nullptr);
        result.pop_back(); // Remove null terminator
        return result;
    }
    
    #ifndef __APPLE__
    void ensureOpenCLVendorPath() {
        const char* vendorEnv = std::getenv("OPENCL_VENDOR_PATH");
        if (!vendorEnv || std::string(vendorEnv).empty()) {
            setenv("OPENCL_VENDOR_PATH", "/etc/OpenCL/vendors", 1);
        }
    }
    #else
    void ensureOpenCLVendorPath() {
        // macOS: do nothing; system handles OpenCL vendor paths differently
    }
    #endif
    
    void run_gpu_benchmark() {
        ensureOpenCLVendorPath();  // <-- Call this first
    
        try {
            // Get platform
            cl_uint num_platforms;
            cl_int error = clGetPlatformIDs(0, nullptr, &num_platforms);
            check_error(error, "getting platform count");
            
            if (num_platforms == 0) {
                std::cout << "No OpenCL platforms found. GPU benchmark cannot run. Please install opencl runtime on your system\n";
                return;
            }
            
            std::vector<cl_platform_id> platforms(num_platforms);
            error = clGetPlatformIDs(num_platforms, platforms.data(), nullptr);
            check_error(error, "getting platforms");
            
            // Find GPU device
            cl_device_id device = nullptr;
            std::string device_name, platform_name;
            
            for (auto platform : platforms) {
                size_t size;
                clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, nullptr, &size);
                std::string plat_name(size, '\0');
                clGetPlatformInfo(platform, CL_PLATFORM_NAME, size, &plat_name[0], nullptr);
                plat_name.pop_back();
                
                cl_uint num_devices;
                error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices);
                
                if (error == CL_SUCCESS && num_devices > 0) {
                    std::vector<cl_device_id> devices(num_devices);
                    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, num_devices, devices.data(), nullptr);
                    
                    device = devices[0]; // Use first GPU found
                    device_name = get_device_info(device, CL_DEVICE_NAME);
                    platform_name = plat_name;
                    break;
                }
            }
            
            if (device == nullptr) {
                std::cout << "No GPU devices found. GPU benchmark cannot run.\n";
                return;
            }

        std::cout << "\nStarting GPU threads...press 'q' to terminate." << std::endl;
        std::cout << "GPU detected: " << device_name << std::endl;

        cl_ulong global_mem_size;
        clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(global_mem_size), &global_mem_size, nullptr);
        std::cout << "VRAM: " << global_mem_size / (1024*1024) << " MB" << std::endl;

        cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &error);
        check_error(error, "creating context");

        cl_command_queue queue = clCreateCommandQueue(context, device, 0, &error);
        check_error(error, "creating command queue");

        cl_program program = clCreateProgramWithSource(context, 1, &matrix_mult_kernel, nullptr, &error);
        check_error(error, "creating program");

        error = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
        if (error != CL_SUCCESS) {
            size_t log_size;
            clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
            std::string build_log(log_size, '\0');
            clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, &build_log[0], nullptr);
            std::cerr << "Build error:\n" << build_log << std::endl;
            throw std::runtime_error("Program build failed");
        }

        cl_kernel kernel = clCreateKernel(program, "matrix_multiply", &error);
        check_error(error, "creating kernel");

        constexpr int N = 512;
        const size_t matrix_size = N * N * sizeof(float);

        std::vector<float> h_A(N * N, 1.1f);
        std::vector<float> h_B(N * N, 2.2f);
        std::vector<float> h_C(N * N, 0.0f);

        cl_mem d_A = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, matrix_size, h_A.data(), &error);
        check_error(error, "creating buffer A");

        cl_mem d_B = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, matrix_size, h_B.data(), &error);
        check_error(error, "creating buffer B");

        cl_mem d_C = clCreateBuffer(context, CL_MEM_WRITE_ONLY, matrix_size, nullptr, &error);
        check_error(error, "creating buffer C");

        error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_A);
        error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_B);
        error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_C);
        error |= clSetKernelArg(kernel, 3, sizeof(int), &N);
        check_error(error, "setting kernel arguments");

        size_t global_work_size[2] = {static_cast<size_t>(N), static_cast<size_t>(N)};
        size_t local_work_size[2] = {16, 16};

        std::atomic<bool> stop_flag{false};

        // Input thread
        std::thread input_thread([&]() {
            while (!stop_flag.load()) {
                char ch = wait_for_keypress_non_blocking();
                if (ch == 'q' || ch == 'Q') {
                    stop_flag = true;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });

        long long mult_count = 0;
        auto start = std::chrono::high_resolution_clock::now();

        while (!stop_flag.load()) {
            error = clEnqueueNDRangeKernel(queue, kernel, 2, nullptr,
                                           global_work_size, local_work_size,
                                           0, nullptr, nullptr);
            check_error(error, "executing kernel");

                clFlush(queue);  // Just submit kernel, don't wait

                // Optional small delay to avoid CPU busy loop
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

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

        double gflops = (static_cast<double>(mult_count) * N * N * N * 2) / (total_elapsed / 1000.0) / 1e9;

        std::cout << "\nGPU benchmark completed " << std::endl;
        std::cout << "Matrices multiplied: " << mult_count << std::endl;
        std::cout << "Average performance: " << static_cast<int>(mults_per_sec) << " Ops/sec" << std::endl;
        std::cout << "GPU Performance: " << static_cast<int>(gflops) << " GFLOPS\n" << std::endl;

        // Cleanup input thread
        stop_flag = true;
        input_thread.join();

        // Cleanup
        clReleaseMemObject(d_A);
        clReleaseMemObject(d_B);
        clReleaseMemObject(d_C);
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        clReleaseCommandQueue(queue);
        clReleaseContext(context);

    } catch (const std::exception& e) {
        std::cerr << "GPU benchmark failed: " << e.what() << std::endl;
    }
}
