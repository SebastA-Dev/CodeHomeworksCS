#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <chrono>
#include "TorreHanoi.h"

int main() {
    std::ofstream archivoHanoi("tiempos_hanoi.txt");
    if (!archivoHanoi) {
        std::cerr << "Error al crear archivo de tiempos\n";
        return 1;
    }

    auto tiempoInicio = std::chrono::steady_clock::now();
    auto limite24h = tiempoInicio + std::chrono::hours(24);
    size_t n = 1;

    while (std::chrono::steady_clock::now() < limite24h) {
        TorreHanoi torre(n);

        LARGE_INTEGER freq, start, end;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);

        torre.resolver();

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

        archivoHanoi << "N=" << n << "    " << oss.str() << "\n";
        archivoHanoi.flush();
        std::cout << "Tiempo Torre Hanoi (N=" << n << "): " << oss.str() << std::endl;
        n++;
    }

    archivoHanoi.close();
    return 0;
}

