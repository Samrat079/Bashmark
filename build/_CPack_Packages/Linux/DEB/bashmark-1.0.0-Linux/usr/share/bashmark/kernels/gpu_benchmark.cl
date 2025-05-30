__kernel void gpu_benchmark(__global float* buffer) {
    int gid = get_global_id(0);
    float val = buffer[gid];

    // Simulate heavy work (no iteration parameter)
    for (int i = 0; i < 10000; i++) {
        val = sin(val) + cos(val);
    }

    buffer[gid] = val;
}
