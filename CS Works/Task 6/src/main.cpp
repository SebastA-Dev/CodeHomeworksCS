#include <iostream>   
#include <vector>       
#include <iomanip>      
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <fstream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../include/ProblemasNP.h"

void probarSudoku(ProblemasNP& np) {    
    np.hasSolution();
}

void probarParticion(ProblemasNP& np) {
    if (np.findPartition()) {        
        np.reorderSets();        
    } 
}

int main() {
    ProblemasNP np;
    int i = 1;
    np.seedRandom(std::random_device{}());
    
    std::ofstream archivoSudoku("tiempos_sudoku.txt");
    std::ofstream archivoParticion("tiempos_particion.txt");
    
    if (!archivoSudoku || !archivoParticion) {
        std::cerr << "Error al crear archivos de tiempos\n";
        return 1;
    }
        
    auto tiempoInicio = std::chrono::steady_clock::now();
    auto limite24h = tiempoInicio + std::chrono::hours(24);
    size_t iteracion = 1;
    

    while (std::chrono::steady_clock::now() < limite24h) {        
                
        auto tiempoActual = std::chrono::steady_clock::now();
        auto tiempoTranscurrido = std::chrono::duration_cast<std::chrono::hours>(tiempoActual - tiempoInicio);        
        
        np.createSudoku(3+i);
        np.setRandomInitialValues(9);        
        np.printSudoku();
        
        {
            LARGE_INTEGER freq, start, end;
            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&start);
            probarSudoku(np);        
            QueryPerformanceCounter(&end);
            unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
            unsigned long long ns = static_cast<unsigned long long>(ticks * 1000000000ULL / static_cast<unsigned long long>(freq.QuadPart));
            unsigned long long minutes = ns / 60000000000ULL;
            unsigned long long seconds = (ns / 1000000000ULL) % 60ULL;
            unsigned long long msecs = (ns / 1000000ULL) % 1000ULL;
            unsigned long long nsrem = ns % 1000000ULL;
            unsigned long long usecs = nsrem / 1000ULL;
            unsigned long long nsecs = nsrem % 1000ULL;
            std::ostringstream oss;
            oss << std::setfill('0') << std::setw(2) << minutes << ":"
                << std::setw(2) << seconds << ":"
                << std::setw(3) << msecs << ":"
                << std::setw(3) << usecs << ":"
                << std::setw(3) << nsecs;
            
            archivoSudoku << "    " << oss.str() << "\n";
            archivoSudoku.flush(); // Forzar escritura inmediata
            std::cout << "Tiempo Sudoku: " << oss.str() << "\n";
        }

        np.createRandomSet(i*10, 1, 100000);
        
        {
            LARGE_INTEGER freq, start, end;
            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&start);
            probarParticion(np);
            QueryPerformanceCounter(&end);
            unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
            unsigned long long ns = static_cast<unsigned long long>(ticks * 1000000000ULL / static_cast<unsigned long long>(freq.QuadPart));
            unsigned long long minutes = ns / 60000000000ULL;
            unsigned long long seconds = (ns / 1000000000ULL) % 60ULL;
            unsigned long long msecs = (ns / 1000000ULL) % 1000ULL;
            unsigned long long nsrem = ns % 1000000ULL;
            unsigned long long usecs = nsrem / 1000ULL;
            unsigned long long nsecs = nsrem % 1000ULL;
            std::ostringstream oss;
            oss << std::setfill('0') << std::setw(2) << minutes << ":"
                << std::setw(2) << seconds << ":"
                << std::setw(3) << msecs << ":"
                << std::setw(3) << usecs << ":"
                << std::setw(3) << nsecs;
            
            archivoParticion << "    " << oss.str() << "\n";
            archivoParticion.flush(); // Forzar escritura inmediata
            std::cout << "Tiempo Partición: " << oss.str() << "\n";
        }
        i+=1;
    }
    
    std::cout << "\n=== PRUEBAS COMPLETADAS ===\n";
    std::cout << "Los tiempos han sido guardados en:\n";
    std::cout << "  - tiempos_sudoku.txt\n";
    std::cout << "  - tiempos_particion.txt\n";
    
    // Cerrar archivos
    archivoSudoku.close();
    archivoParticion.close();

    return 0;
}
