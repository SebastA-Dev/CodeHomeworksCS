#pragma once

#include <vector>

namespace task4 {

class OrdenamientoRecursivo {
public:
    static void quickSort(std::vector<int>& arreglo);
    static void radixSort(std::vector<int>& arreglo);
    static void mergeSort(std::vector<int>& arreglo, int first, int last);
};

}
