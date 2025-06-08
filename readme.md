```text    
██████╗  █████╗ ███████╗██╗  ██╗███╗   ███╗ █████╗ ██████╗ ██╗  ██╗
██╔══██╗██╔══██╗██╔════╝██║  ██║████╗ ████║██╔══██╗██╔══██╗██║ ██╔╝
██████╔╝███████║███████╗███████║██╔████╔██║███████║██████╔╝█████╔╝ 
██╔══██╗██╔══██║╚════██║██╔══██║██║╚██╔╝██║██╔══██║██╔══██╗██╔═██╗ 
██████╔╝██║  ██║███████║██║  ██║██║ ╚═╝ ██║██║  ██║██║  ██║██║  ██╗
╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
```

# 🚀 Bashmark

**Bashmark** is a terminal-based benchmarking tool for testing CPU and GPU performance.
Once installed, you can run it by simply typing:

```bash
bashmark
```

The program allows you to choose the type of benchmark you want to perform.
**Currently supported options:**

* 🧮 CPU Single-core Benchmark
* 🧠 CPU Multi-core Benchmark
* 🎮 GPU Benchmark

*(More features are coming soon!)*

---

# 🛠️ How Does It Work?

Bashmark performs complex **matrix math** to simulate graphical rendering loads and computational tasks.

* The program runs each benchmark for **2 minutes**.
* Outputs are displayed in a concise, easy-to-understand format.

---

# 🖥️ CPU Benchmarking

🧵 **Single-core** and **Multi-core** benchmarking is done by performing matrix math on one or more threads:

* **Single-core** → One thread
* **Multi-core** → Multiple threads

⚠️ **Caution:**

* The number of results may differ from the actual number of CPU cores, as the process depends on the number of threads, not cores.
* On CPUs with hybrid architectures (e.g. performance + efficiency cores), thread performance may vary.

---

# 🎮 GPU Benchmarking

GPU benchmarking works similarly to CPU benchmarking but runs matrix math directly on the GPU using **OpenCL**.

* GPU performance is independent of the number of threads.
* Device information is printed at the start of the process for debugging purposes.

⚠️ **Caution:**

* Ensure the correct GPU device is selected (especially on multi-GPU systems).
* For systems with unified memory, check if reported VRAM makes sense.
* If OpenCL is not installed, GPU benchmarks won't run — ensure your GPU drivers and OpenCL runtime are correctly installed.

---

# 🌍 Compatibility

**Bashmark** is designed from the ground up to work on:

* 🪟 **Windows**
* 🍎 **macOS**
* 🐧 **Linux**

**Notes:**

* CPU benchmarking works out of the box.
* GPU benchmarking requires proper installation of OpenCL — this may need manual setup on some systems.
* You may experience some lag when terminating a benchmark mid-run. The program has been tested to terminate cleanly without hanging.

---

# 🛠️ Build Instructions

# 📦 Pre-built Packages

You can download pre-built packages from the [Releases page](https://github.com/Samrat079/bashmark/releases).

### For Fedora / RHEL / CentOS:

Download `bashmark-1.0.0.rpm` and install:

```bash
sudo dnf install bashmark-1.0.0.rpm
```

### For Debian:

Download `bashmark-1.0.0.deb` and install:

```bash
sudo apt install bashmark-1.0.0.deb
```

---

# 🚧 Known Issues

* OpenCL must be installed and working correctly for GPU benchmarks.
* On systems with hybrid CPUs, thread performance can vary.
* Manual device verification is recommended before starting benchmarks.

---

