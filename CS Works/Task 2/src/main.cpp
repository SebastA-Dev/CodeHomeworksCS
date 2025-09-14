#include <iostream>
#include <vector>
#include <cstdio>
#include "../include/SortingAlgorithm.h"
#include "../../TimeComplexAlgorithms.h"


// Generates a vector of N random integers
std::vector<int> generateRandomVector(int N) {
    std::vector<int> arr(N);
    for (int& x : arr) {
        x = rand() % 1000;
    }
    return arr;
}

int main() {
    int N = 1000; 
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<int> arr = generateRandomVector(N);
    SortingAlgorithm sorter;
    TimeComplexAlgorithms timer;
    
#define RUN_TOOL(tool, algPtr, name) \
    do { \
        auto arr_copy = arr; \
        std::cout << "  " << name << " - Tool " << tool << ": "; \
        timer.herramienta_tiempo_##tool(algPtr, sorter, arr_copy); \
    } while(0)

    std::cout << "=== Insertion Algorithm ===" << std::endl;
    RUN_TOOL(1, &SortingAlgorithm::InsertionAlgorithm, "InsertionAlgorithm");
    RUN_TOOL(2, &SortingAlgorithm::InsertionAlgorithm, "InsertionAlgorithm");
    RUN_TOOL(3, &SortingAlgorithm::InsertionAlgorithm, "InsertionAlgorithm");
    RUN_TOOL(4, &SortingAlgorithm::InsertionAlgorithm, "InsertionAlgorithm");

    std::cout << "=== Selection Algorithm ===" << std::endl;
    RUN_TOOL(1, &SortingAlgorithm::SelectionAlgorithm, "SelectionAlgorithm");
    RUN_TOOL(2, &SortingAlgorithm::SelectionAlgorithm, "SelectionAlgorithm");
    RUN_TOOL(3, &SortingAlgorithm::SelectionAlgorithm, "SelectionAlgorithm");
    RUN_TOOL(4, &SortingAlgorithm::SelectionAlgorithm, "SelectionAlgorithm");

    std::cout << "=== Bubble Algorithm ===" << std::endl;
    RUN_TOOL(1, &SortingAlgorithm::BubbleAlgorithm, "BubbleAlgorithm");
    RUN_TOOL(2, &SortingAlgorithm::BubbleAlgorithm, "BubbleAlgorithm");
    RUN_TOOL(3, &SortingAlgorithm::BubbleAlgorithm, "BubbleAlgorithm");
    RUN_TOOL(4, &SortingAlgorithm::BubbleAlgorithm, "BubbleAlgorithm");

    std::cout << "=== Shell Algorithm ===" << std::endl;
    RUN_TOOL(1, &SortingAlgorithm::ShellAlgorithm, "ShellAlgorithm");
    RUN_TOOL(2, &SortingAlgorithm::ShellAlgorithm, "ShellAlgorithm");
    RUN_TOOL(3, &SortingAlgorithm::ShellAlgorithm, "ShellAlgorithm");
    RUN_TOOL(4, &SortingAlgorithm::ShellAlgorithm, "ShellAlgorithm");

#undef RUN_TOOL

    return 0;
}
