#pragma once

#include <vector>
#include <algorithm>

class SearchingAlgorithms {
public:
    // Simple linear search: returns index or -1
    int LinearSearch1(std::vector<size_t>& arr, int target) {
        for (size_t i = 0; i < arr.size(); ++i) {
            if (arr[i] == target) return static_cast<int>(i);
        }
        return -1;
    }

    // Linear search with sentinel (copy arr with sentinel) - returns index or -1
    int LinearSearch2(std::vector<size_t>& arr, int target) {
        if (arr.empty()) return -1;
        std::vector<size_t> tmp = arr;
        tmp.push_back(target); // sentinel
        size_t i = 0;
        while (tmp[i] != target) ++i;
        if (i < arr.size()) return static_cast<int>(i);
        return -1;
    }

    // Binary search (iterative). Assumes arr is sorted. Returns index or -1
    int BinarySearch(std::vector<size_t>& arr, int target) {        
        int l = 0;
        int r = static_cast<int>(arr.size()) - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (arr[mid] == target) return mid;
            if (arr[mid] < target) l = mid + 1;
            else r = mid - 1;
        }
        return -1;
    }
    
    int ShellAlgorithm(std::vector<size_t>& arr, int target) {
        for (size_t gap = arr.size() / 2; gap > 0; gap /= 2) {
            for (size_t i = gap; i < arr.size(); ++i) {
                int temp = arr[i];
                size_t j;
                for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                    arr[j] = arr[j - gap];
                }
                arr[j] = temp;
            }
        }
    }
};
