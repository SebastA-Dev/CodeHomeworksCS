#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <chrono>
#include "NQueens.h"

int main() {
    std::ofstream archivoNQueens("tiempos_nqueens.txt");
    if (!archivoNQueens) {
        std::cerr << "Error al crear archivo de tiempos\n";
        return 1;
    }

    auto tiempoInicio = std::chrono::steady_clock::now();
    auto limite24h = tiempoInicio + std::chrono::hours(24);
    size_t n = 4;

    while (std::chrono::steady_clock::now() < limite24h) {
        NQueens solver(n);

        LARGE_INTEGER freq, start, end;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);

        solver.findSolution();

        QueryPerformanceCounter(&end);
        unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
        unsigned long long ns = ticks * 1000000000ULL / freq.QuadPart;

        unsigned long long total_seconds = ns / 1000000000ULL;
        unsigned long long hours = total_seconds / 3600ULL;
        unsigned long long minutes = (total_seconds % 3600ULL) / 60ULL;
        unsigned long long seconds = total_seconds % 60ULL;

        unsigned long long msecs = (ns / 1000000ULL) % 1000ULL;
        unsigned long long nsrem = ns % 1000000ULL;
        unsigned long long usecs = nsrem / 1000ULL;
        unsigned long long nsecs = nsrem % 1000ULL;

        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setw(2) << minutes << ":"
            << std::setw(2) << seconds << ":"
            << std::setw(3) << msecs << ":"
            << std::setw(3) << usecs << ":"
            << std::setw(3) << nsecs;

        archivoNQueens << "N=" << n << "    " << oss.str() << "\n";
        archivoNQueens.flush();
        std::cout << "Tiempo NQueens (N=" << n << "): " << oss.str() << std::endl;

        // Puedes imprimir la solución si quieres:
//      solver.printBoard();
//      std::cout << std::endl;

        ++n;
    }

    archivoNQueens.close();
    std::cout << "\n=== PRUEBAS COMPLETADAS ===\n";
    std::cout << "Los tiempos han sido guardados en: tiempos_nqueens.txt\n";

    return 0;
}



