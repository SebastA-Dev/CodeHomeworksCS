#include <vector>

class SortingAlgorithm {
public:
    // Insertion Sort
    void InsertionAlgorithm(std::vector<int>& arr) {
        for (int i = 1; i < arr.size(); ++i) {
            int key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                --j;
            }
            arr[j + 1] = key;
        }
    }

    // Bubble Sort
    void BubbleAlgorithm(std::vector<int>& arr) {
        for (int i = 0; i < arr.size(); ++i) {
            for (int j = 0; j < arr.size() - i - 1; ++j) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    // Shell Sort
    void ShellAlgorithm(std::vector<int>& arr) {
        for (int gap = arr.size() / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < arr.size(); ++i) {
                int temp = arr[i];
                int j;
                for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                    arr[j] = arr[j - gap];
                }
                arr[j] = temp;
            }
        }
    }

    // Selection Sort
    void SelectionAlgorithm(std::vector<int>& arr) {
        for (int i = 0; i < arr.size() - 1; ++i) {
            int min_idx = i;
            for (int j = i + 1; j < arr.size(); ++j) {
                if (arr[j] < arr[min_idx]) {
                    min_idx = j;
                }
            }
            std::swap(arr[i], arr[min_idx]);
        }
    }
};
