#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <iomanip>
#include "../include/RandomVector.h"
#include "../include/OrdenamientoRecursivo.h"


static void imprimir_arreglo(const std::vector<long long>& arreglo) {
    for (long long x : arreglo) std::cout << x << " ";
    std::cout << std::endl;
}

int main() {
    const std::size_t N = 9000;

    auto arreglo_original = task4::make_random_vector(N);
    auto arreglo_quick = task4::make_random_vector(N);
    auto arreglo_radix = task4::make_random_vector_big_numbers(N);

    // el peor caso de radix es cuando los valores de ordenamiento son demasiados grandes por la cantidad 
    // de veces que tiene que sacar el residuo usando los bytes t(n) = O(n*k) siendo k = 256 bits
    // merge no tiene peor o mejor caso

    // para ver el cambio de tiempos en quick, comenta esta linea y compila, t(n) = O(n^2) peor caso
    std::sort(arreglo_quick.begin(), arreglo_quick.end());
    {
        LARGE_INTEGER freq, start, end;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
        task4::OrdenamientoRecursivo::quickSort(arreglo_quick);
        QueryPerformanceCounter(&end);
        unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
        unsigned long long ns = static_cast<unsigned long long>(ticks * 1000000000ULL / static_cast<unsigned long long>(freq.QuadPart));
        unsigned long long minutes = ns / 60000000000ULL;
        unsigned long long seconds = (ns / 1000000000ULL) % 60ULL;
        unsigned long long msecs = (ns / 1000000ULL) % 1000ULL;
        unsigned long long nsrem = ns % 1000000ULL;
        std::cout << "Tiempo quickSort: " << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds << ":" << std::setw(3) << msecs << ":" << std::setw(6) << nsrem << "\n\n";
    }
        
    {
        LARGE_INTEGER freq, start, end;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
        task4::OrdenamientoRecursivo::radixSort(arreglo_radix);
        QueryPerformanceCounter(&end);
        unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
        unsigned long long ns = static_cast<unsigned long long>(ticks * 1000000000ULL / static_cast<unsigned long long>(freq.QuadPart));
        unsigned long long minutes = ns / 60000000000ULL;
        unsigned long long seconds = (ns / 1000000000ULL) % 60ULL;
        unsigned long long msecs = (ns / 1000000ULL) % 1000ULL;
        unsigned long long nsrem = ns % 1000000ULL;
        std::cout << "Tiempo radixSort: " << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds << ":" << std::setw(3) << msecs << ":" << std::setw(6) << nsrem << "\n\n";
    }
    
    auto arreglo_merge = arreglo_original;    
    {
        LARGE_INTEGER freq, start, end;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
        task4::OrdenamientoRecursivo::mergeSort(arreglo_merge, 0, static_cast<int>(arreglo_merge.size()) - 1);
        QueryPerformanceCounter(&end);
        unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
        unsigned long long ns = static_cast<unsigned long long>(ticks * 1000000000ULL / static_cast<unsigned long long>(freq.QuadPart));
        unsigned long long minutes = ns / 60000000000ULL;
        unsigned long long seconds = (ns / 1000000000ULL) % 60ULL;
        unsigned long long msecs = (ns / 1000000ULL) % 1000ULL;
        unsigned long long nsrem = ns % 1000000ULL;
        std::cout << "Tiempo mergeSort: " << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds << ":" << std::setw(3) << msecs << ":" << std::setw(6) << nsrem << "\n\n";
    }

    return 0;
}
 
