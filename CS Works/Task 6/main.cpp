#include <iostream>
#include <fstream>
#include <windows.h>
#include <chrono>
#include "NQueens.h"
#include "TorreHanoi.h"

int main() {
    std::ofstream archivoNQueens("tiempos_queens.txt");
    if (!archivoNQueens) {
        std::cerr << "Error al crear archivo de tiempos\n";
        return 1;
    }
    
    std::ofstream archivoHanoi("tiempos_hanoi.txt");
    if (!archivoNQueens) {
        std::cerr << "Error al crear archivo de tiempos\n";
        return 1;
    }

    //auto tiempoInicio = std::chrono::steady_clock::now();
    //auto limite24h = tiempoInicio + std::chrono::hours(24);
    size_t n = 1;

   while (n < 10){
        TorreHanoi torre(n);

        LARGE_INTEGER freq, start, end;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);

        torre.resolver();

        QueryPerformanceCounter(&end);
        unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
        unsigned long long ns = ticks * 1000000000ULL / freq.QuadPart;

        archivoHanoi << ns << "\n";
        archivoHanoi.flush();

        std::cout << "Tiempo Torre Hanoi (N=" << n << "): " << ns << " ns" << std::endl;
        n++;
    }
    
    size_t m = 1;
    
    while (m < 10) {
	    NQueens reinas(m);
	
	    LARGE_INTEGER freq, start, end;
	    QueryPerformanceFrequency(&freq);
	    QueryPerformanceCounter(&start);
	
	    reinas.findSolution(); 
	
	    QueryPerformanceCounter(&end);
	    unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
	    unsigned long long ns = ticks * 1000000000ULL / freq.QuadPart;
	
	    archivoNQueens << ns << "\n";
	    archivoNQueens.flush();
	
	    std::cout << "Tiempo NQueens (N=" << n << "): " << ns << " ns" << std::endl;
	    m++;
	}


    archivoNQueens.close();
    return 0;
}
