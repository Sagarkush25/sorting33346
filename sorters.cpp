#include "sorters.hpp"
#include <chrono>
#include <algorithm>
#include <random>

static bool verifySorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

BenchmarkResult BubbleSort::execute(std::vector<int>& data) {
    uint64_t comps = 0;
    int n = static_cast<int>(data.size());

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            comps++;
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    auto stop = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double, std::micro>(stop - start).count();
    return { getName(), comps, elapsed, verifySorted(data) };
}

BenchmarkResult InsertionSort::execute(std::vector<int>& data) {
    uint64_t comps = 0;
    int n = static_cast<int>(data.size());

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < n; ++i) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0) {
            comps++;
            if (data[j] > key) {
                data[j + 1] = data[j];
                j--;
            } else {
                break;
            }
        }
        data[j + 1] = key;
    }
    auto stop = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double, std::micro>(stop - start).count();
    return { getName(), comps, elapsed, verifySorted(data) };
}

void MergeSort::merge(std::vector<int>& arr, std::vector<int>& temp, int left, int mid, int right, uint64_t& comps) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        comps++;
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (int idx = left; idx <= right; ++idx) arr[idx] = temp[idx];
}

void MergeSort::sortSubarray(std::vector<int>& arr, std::vector<int>& temp, int left, int right, uint64_t& comps) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    sortSubarray(arr, temp, left, mid, comps);
    sortSubarray(arr, temp, mid + 1, right, comps);
    merge(arr, temp, left, mid, right, comps);
}

BenchmarkResult MergeSort::execute(std::vector<int>& data) {
    uint64_t comps = 0;
    std::vector<int> temp(data.size());

    auto start = std::chrono::high_resolution_clock::now();
    if (!data.empty()) {
        sortSubarray(data, temp, 0, static_cast<int>(data.size()) - 1, comps);
    }
    auto stop = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double, std::micro>(stop - start).count();
    return { getName(), comps, elapsed, verifySorted(data) };
}

int QuickSort::choosePivotIndex(std::vector<int>& arr, int low, int high, uint64_t& comps) {
    if (strategy == QuickPivotStrategy::FIRST) {
        return low;
    }
    if (strategy == QuickPivotStrategy::RANDOM) {
        static std::mt19937 rng(42);
        std::uniform_int_distribution<int> dist(low, high);
        return dist(rng);
    }
    int mid = low + (high - low) / 2;
    comps += 3;
    if (arr[low] > arr[mid]) std::swap(low, mid);
    if (arr[low] > arr[high]) std::swap(low, high);
    if (arr[mid] > arr[high]) std::swap(mid, high);
    return mid;
}

int QuickSort::hoarePartition(std::vector<int>& arr, int low, int high, uint64_t& comps) {
    int pIdx = choosePivotIndex(arr, low, high, comps);
    int pivot = arr[pIdx];

    int i = low - 1;
    int j = high + 1;

    while (true) {
        do {
            i++;
            comps++;
        } while (arr[i] < pivot);

        do {
            j--;
            comps++;
        } while (arr[j] > pivot);

        if (i >= j) return j;
        std::swap(arr[i], arr[j]);
    }
}

void QuickSort::quickSortRecursive(std::vector<int>& arr, int low, int high, uint64_t& comps) {
    while (low < high) {
        int splitPoint = hoarePartition(arr, low, high, comps);
        if (splitPoint - low < high - splitPoint) {
            quickSortRecursive(arr, low, splitPoint, comps);
            low = splitPoint + 1;
        } else {
            quickSortRecursive(arr, splitPoint + 1, high, comps);
            high = splitPoint;
        }
    }
}

BenchmarkResult QuickSort::execute(std::vector<int>& data) {
    uint64_t comps = 0;
    auto start = std::chrono::high_resolution_clock::now();
    if (!data.empty()) {
        quickSortRecursive(data, 0, static_cast<int>(data.size()) - 1, comps);
    }
    auto stop = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double, std::micro>(stop - start).count();
    return { getName(), comps, elapsed, verifySorted(data) };
}

void HeapSort::siftDown(std::vector<int>& arr, int start, int end, uint64_t& comps) {
    int root = start;
    while (2 * root + 1 <= end) {
        int child = 2 * root + 1;
        int swapIdx = root;

        comps++;
        if (arr[swapIdx] < arr[child]) {
            swapIdx = child;
        }
        if (child + 1 <= end) {
            comps++;
            if (arr[swapIdx] < arr[child + 1]) {
                swapIdx = child + 1;
            }
        }
        if (swapIdx == root) return;
        std::swap(arr[root], arr[swapIdx]);
        root = swapIdx;
    }
}

BenchmarkResult HeapSort::execute(std::vector<int>& data) {
    uint64_t comps = 0;
    int n = static_cast<int>(data.size());

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = (n - 2) / 2; i >= 0; --i) {
        siftDown(data, i, n - 1, comps);
    }
    for (int end = n - 1; end > 0; --end) {
        std::swap(data[0], data[end]);
        siftDown(data, 0, end - 1, comps);
    }
    auto stop = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double, std::micro>(stop - start).count();
    return { getName(), comps, elapsed, verifySorted(data) };
}

BenchmarkResult RadixSort::execute(std::vector<int>& data) {
    int n = static_cast<int>(data.size());
    auto start = std::chrono::high_resolution_clock::now();

    if (n > 1) {
        std::vector<uint32_t> udata(n);
        for (int i = 0; i < n; ++i) {
            udata[i] = static_cast<uint32_t>(data[i]) ^ 0x80000000U;
        }

        std::vector<uint32_t> buffer(n);
        for (int shift = 0; shift < 32; shift += 8) {
            int counts[256] = {0};
            for (int i = 0; i < n; ++i) {
                counts[(udata[i] >> shift) & 0xFF]++;
            }
            int offset[256];
            offset[0] = 0;
            for (int i = 1; i < 256; ++i) {
                offset[i] = offset[i - 1] + counts[i - 1];
            }
            for (int i = 0; i < n; ++i) {
                int bucket = (udata[i] >> shift) & 0xFF;
                buffer[offset[bucket]++] = udata[i];
            }
            udata = buffer;
        }

        for (int i = 0; i < n; ++i) {
            data[i] = static_cast<int>(udata[i] ^ 0x80000000U);
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::micro>(stop - start).count();
    return { getName(), 0, elapsed, verifySorted(data) };
}
