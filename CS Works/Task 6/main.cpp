#include <iostream>
#include <fstream>
#include <windows.h>
#include <chrono>
#include "NQueens.h"
#include "TorreHanoi.h"

int main() {
    std::ofstream archivoNQueens("tiempos_hanoi.txt");
    if (!archivoNQueens) {
        std::cerr << "Error al crear archivo de tiempos\n";
        return 1;
    }

    auto tiempoInicio = std::chrono::steady_clock::now();
    auto limite24h = tiempoInicio + std::chrono::hours(24);
    size_t n = 1;

   while (std::chrono::steady_clock::now() < limite24h && n < 20){
        TorreHanoi torre(n);

        LARGE_INTEGER freq, start, end;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);

        torre.resolver();

        QueryPerformanceCounter(&end);
        unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
        unsigned long long ns = ticks * 1000000000ULL / freq.QuadPart;

        archivoNQueens << ns << "\n";
        archivoNQueens.flush();

        std::cout << "Tiempo Torre Hanoi (N=" << n << "): " << ns << " ns" << std::endl;
        n++;
    }

    archivoNQueens.close();
    return 0;
}
