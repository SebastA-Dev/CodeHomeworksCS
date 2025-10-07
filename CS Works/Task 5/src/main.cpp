// Librerías estándar
#include <iostream>      // Para cout, cin, etc.
#include <vector>        // Para std::vector
#include <iomanip>       // Para manipuladores como std::setw, std::setprecision

// Encabezados de tu proyecto
#include "../include/RandomVector.h"
#include "../include/Radix.h"

// API de Windows (solo si realmente usas funciones del sistema)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


void print_vec(const std::vector<int>& v, const char* title) {
	std::cout << title;
	for (std::size_t i = 0; i < v.size(); ++i) {
		if (i) std::cout << ", ";
		std::cout << v[i];
		if (i >= 19) { std::cout << ", ..."; break; }
	}
	std::cout << std::endl;
}

int main(int argc, char** argv) {
	
	
	for (size_t i = 0; i < 10000000; i+=50000)
	{
		std::vector<int> array_peor_caso = make_random_vector_big_numbers(i);

		{
			LARGE_INTEGER freq, start, end;
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&start);
			radixSort(array_peor_caso);
			QueryPerformanceCounter(&end);
			unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
			unsigned long long ns = static_cast<unsigned long long>(ticks * 1000000000ULL / static_cast<unsigned long long>(freq.QuadPart));
			unsigned long long minutes = ns / 60000000000ULL;
			unsigned long long seconds = (ns / 1000000000ULL) % 60ULL;
			unsigned long long msecs = (ns / 1000000ULL) % 1000ULL;
			unsigned long long nsrem = ns % 1000000ULL;
			std::cout << "Tiempo radixSort Peor Caso: " << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds << ":" << std::setw(3) << msecs << ":" << std::setw(6) << nsrem << "\n\n";
		}
	}

	for (size_t i = 0; i < 10000000; i+=50000)
	{
		std::vector<int> array_normal = make_random_vector(i);

		{
			LARGE_INTEGER freq, start, end;
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&start);
			radixSort(array_normal);
			QueryPerformanceCounter(&end);
			unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
			unsigned long long ns = static_cast<unsigned long long>(ticks * 1000000000ULL / static_cast<unsigned long long>(freq.QuadPart));
			unsigned long long minutes = ns / 60000000000ULL;
			unsigned long long seconds = (ns / 1000000000ULL) % 60ULL;
			unsigned long long msecs = (ns / 1000000ULL) % 1000ULL;
			unsigned long long nsrem = ns % 1000000ULL;
			std::cout << "Tiempo radixSort Mejor Caso: " << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds << ":" << std::setw(3) << msecs << ":" << std::setw(6) << nsrem << "\n\n";
		}
	}

	return 0;
}
