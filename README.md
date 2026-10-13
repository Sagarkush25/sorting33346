# Sorting Algorithms Benchmarking Assignment (Object-Oriented Design)

This project implements an experimental evaluation of six classic sorting algorithms (with three Quick Sort pivot strategies) using an Object-Oriented C++17 design.

## Features
- **Polymorphic Architecture**: Abstract base class `ISorter` with virtual `execute` method returning structured `BenchmarkResult`.
- **Hoare Partition Scheme**: Quick Sort implemented using Hoare's two-pointer bidirectional partition scheme.
- **Tail-Call Elimination**: Call stack bounded to O(log n) depth.
- **Signed Integer Radix Sort**: 4-pass LSD radix sort using MSB sign-bit inversion (`^ 0x80000000U`).
- **Benchmark Suite**: Tests on Random, Increasing, and Decreasing distributions across sizes 1,000, 10,000, and 100,000 with 5 iterations per configuration.

## Build and Run Instructions

### 1. Compile and Run C++ Benchmark
```bash
# Direct g++ compilation
g++ -std=c++17 -O3 main.cpp sorters.cpp -o sorting_bench
./sorting_bench
```
The execution times and comparisons will be saved to `results.csv`.

### 2. Generate Plots
```bash
python3 plot_benchmarks.py
```
This generates PNG and PDF plots for all algorithms inside the `plots/` folder.
