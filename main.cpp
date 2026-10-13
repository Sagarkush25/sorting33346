#include "sorters.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <random>

enum class Distribution { RANDOM, ASCENDING, DESCENDING };

std::vector<int> generateDataset(int size, Distribution dist) {
    std::vector<int> data(size);
    if (dist == Distribution::ASCENDING) {
        for (int i = 0; i < size; ++i) data[i] = i - size / 2;
    } else if (dist == Distribution::DESCENDING) {
        for (int i = 0; i < size; ++i) data[i] = size / 2 - i;
    } else {
        std::mt19937 gen(1337 + size);
        std::uniform_int_distribution<int> distrib(-1000000, 1000000);
        for (int i = 0; i < size; ++i) data[i] = distrib(gen);
    }
    return data;
}

int main() {
    std::vector<std::unique_ptr<ISorter>> algorithms;
    algorithms.push_back(std::make_unique<BubbleSort>());
    algorithms.push_back(std::make_unique<InsertionSort>());
    algorithms.push_back(std::make_unique<MergeSort>());
    algorithms.push_back(std::make_unique<QuickSort>(QuickPivotStrategy::FIRST, "Quick Sort (First Pivot)"));
    algorithms.push_back(std::make_unique<QuickSort>(QuickPivotStrategy::RANDOM, "Quick Sort (Random Pivot)"));
    algorithms.push_back(std::make_unique<QuickSort>(QuickPivotStrategy::MEDIAN_OF_THREE, "Quick Sort (Median of 3)"));
    algorithms.push_back(std::make_unique<HeapSort>());
    algorithms.push_back(std::make_unique<RadixSort>());

    const std::vector<int> testSizes = {1000, 10000, 100000};
    const std::vector<std::pair<Distribution, std::string>> distTypes = {
        {Distribution::RANDOM, "Random"},
        {Distribution::ASCENDING, "Increasing"},
        {Distribution::DESCENDING, "Decreasing"}
    };

    std::ofstream csv("results.csv");
    csv << "Algorithm,InputType,Size,Iterations,MeanTimeMicroseconds,MeanComparisons,Correctness\n";

    std::cout << "Starting Sorting Algorithm Benchmark Suite\n";
    std::cout << "==========================================\n";

    for (const auto& algo : algorithms) {
        std::cout << "\nAlgorithm: " << algo->getName() << std::endl;

        for (const auto& [dist, distName] : distTypes) {
            for (int n : testSizes) {
                if (n == 100000) {
                    if (algo->getName() == "Bubble Sort" && dist != Distribution::ASCENDING) {
                        csv << algo->getName() << "," << distName << "," << n << ",5,SKIPPED,SKIPPED,N/A\n";
                        std::cout << "  [" << distName << " | n=" << n << "] -> SKIPPED (O(n^2))\n";
                        continue;
                    }
                    if (algo->getName() == "Insertion Sort" && dist != Distribution::ASCENDING) {
                        csv << algo->getName() << "," << distName << "," << n << ",5,SKIPPED,SKIPPED,N/A\n";
                        std::cout << "  [" << distName << " | n=" << n << "] -> SKIPPED (O(n^2))\n";
                        continue;
                    }
                    if (algo->getName() == "Quick Sort (First Pivot)" && dist != Distribution::RANDOM) {
                        csv << algo->getName() << "," << distName << "," << n << ",5,SKIPPED,SKIPPED,N/A\n";
                        std::cout << "  [" << distName << " | n=" << n << "] -> SKIPPED (O(n^2))\n";
                        continue;
                    }
                }

                double totalTime = 0.0;
                uint64_t totalComps = 0;
                bool passedAll = true;

                for (int rep = 0; rep < 5; ++rep) {
                    auto dataset = generateDataset(n, dist);
                    auto res = algo->execute(dataset);
                    totalTime += res.duration_microseconds;
                    totalComps += res.comparisons;
                    if (!res.is_valid) passedAll = false;
                }

                double meanTime = totalTime / 5.0;
                uint64_t meanComps = totalComps / 5;

                std::string compStr = algo->isComparisonBased() ? std::to_string(meanComps) : "N/A";
                std::string statusStr = passedAll ? "Passed" : "Failed";

                csv << algo->getName() << "," << distName << "," << n << ",5," 
                    << meanTime << "," << compStr << "," << statusStr << "\n";

                std::cout << "  [" << distName << " | n=" << n << "] -> Time: " 
                          << meanTime << " us | Comparisons: " << compStr << " | " << statusStr << "\n";
            }
        }
    }

    std::cout << "\nBenchmark complete. Results exported to results.csv\n";
    return 0;
}
