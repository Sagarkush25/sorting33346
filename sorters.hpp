#ifndef SORTERS_HPP
#define SORTERS_HPP

#include <vector>
#include <string>
#include <cstdint>

struct BenchmarkResult {
    std::string algorithm_name;
    uint64_t comparisons;
    double duration_microseconds;
    bool is_valid;
};

class ISorter {
public:
    virtual ~ISorter() = default;
    virtual std::string getName() const = 0;
    virtual bool isComparisonBased() const { return true; }
    virtual BenchmarkResult execute(std::vector<int>& data) = 0;
};

class BubbleSort : public ISorter {
public:
    std::string getName() const override { return "Bubble Sort"; }
    BenchmarkResult execute(std::vector<int>& data) override;
};

class InsertionSort : public ISorter {
public:
    std::string getName() const override { return "Insertion Sort"; }
    BenchmarkResult execute(std::vector<int>& data) override;
};

class MergeSort : public ISorter {
public:
    std::string getName() const override { return "Merge Sort"; }
    BenchmarkResult execute(std::vector<int>& data) override;
private:
    void merge(std::vector<int>& arr, std::vector<int>& temp, int left, int mid, int right, uint64_t& comps);
    void sortSubarray(std::vector<int>& arr, std::vector<int>& temp, int left, int right, uint64_t& comps);
};

enum class QuickPivotStrategy {
    FIRST,
    RANDOM,
    MEDIAN_OF_THREE
};

class QuickSort : public ISorter {
private:
    QuickPivotStrategy strategy;
    std::string name;

    int choosePivotIndex(std::vector<int>& arr, int low, int high, uint64_t& comps);
    int hoarePartition(std::vector<int>& arr, int low, int high, uint64_t& comps);
    void quickSortRecursive(std::vector<int>& arr, int low, int high, uint64_t& comps);

public:
    explicit QuickSort(QuickPivotStrategy strat, std::string custom_name)
        : strategy(strat), name(std::move(custom_name)) {}

    std::string getName() const override { return name; }
    BenchmarkResult execute(std::vector<int>& data) override;
};

class HeapSort : public ISorter {
public:
    std::string getName() const override { return "Heap Sort"; }
    BenchmarkResult execute(std::vector<int>& data) override;
private:
    void siftDown(std::vector<int>& arr, int start, int end, uint64_t& comps);
};

class RadixSort : public ISorter {
public:
    std::string getName() const override { return "Radix Sort"; }
    bool isComparisonBased() const override { return false; }
    BenchmarkResult execute(std::vector<int>& data) override;
};

#endif
