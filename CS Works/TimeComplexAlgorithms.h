#include <iostream>
#include <array>
#include <windows.h>
#include <x86intrin.h>
#include <chrono>
#include <iomanip>
#include <cstdio>   
#include <windows.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
// Clase para medir el tiempo de ejecución de algoritmos usando diferentes métodos
class TimeComplexAlgorithms {

public:

	// Mide el tiempo usando std::chrono (alta resolución)
	// Recibe cualquier función y argumentos
	template<typename Func, typename... Args>
	void herramienta_tiempo_1(Func&& algoritmo, Args&&... args) {
		auto start = std::chrono::high_resolution_clock::now(); // Marca inicio
		std::invoke(std::forward<Func>(algoritmo), std::forward<Args>(args)...);
		auto end = std::chrono::high_resolution_clock::now(); // Marca fin

		double tiempo_ns = std::chrono::duration<double, std::nano>(end - start).count(); // Calcula tiempo en nanosegundos

		long long segundos      = static_cast<long long>(tiempo_ns) / 1'000'000'000;
		long long milisegundos  = (static_cast<long long>(tiempo_ns) / 1'000'000) % 1000;
		long long nanosegundos  = static_cast<long long>(tiempo_ns) % 1'000'000;

		// Imprime el tiempo formateado
		std::cout << "[chrono precise] "
				  << std::setfill('0') << std::setw(2) << segundos << ":"
				  << std::setfill('0') << std::setw(3) << milisegundos << ":"
				  << std::setfill('0') << std::setw(6) << nanosegundos << std::endl;
	}

	// Mide el tiempo usando QueryPerformanceCounter (Windows, alta precisión)
	template<typename Func, typename... Args>
	void herramienta_tiempo_2(Func&& algoritmo, Args&&... args) {
		LARGE_INTEGER freq, start, end;
		QueryPerformanceFrequency(&freq); // Obtiene frecuencia del contador
		QueryPerformanceCounter(&start);  // Marca inicio

		invoke(std::forward<Func>(algoritmo), std::forward<Args>(args)...); // Ejecuta el algoritmo

		QueryPerformanceCounter(&end);    // Marca fin

		double elapsed_ns = (end.QuadPart - start.QuadPart) * 1e9 / freq.QuadPart; // Calcula tiempo en nanosegundos

		long long segundos = static_cast<long long>(elapsed_ns) / 1'000'000'000;
		long long milisegundos = (static_cast<long long>(elapsed_ns) / 1'000'000) % 1000;
		long long nanosegundos = static_cast<long long>(elapsed_ns) % 1'000'000;

		// Imprime el tiempo formateado
		std::cout << "[QPC] "
				 << std::setfill('0') << std::setw(2) << segundos << ":"
				 << std::setfill('0') << std::setw(3) << milisegundos << ":"
				 << std::setfill('0') << std::setw(6) << nanosegundos << std::endl;
	}

	// Mide el tiempo usando el contador de ciclos de CPU (RDTSC)
	template<typename Func, typename... Args>
	void herramienta_tiempo_3(Func&& algoritmo, Args&&... args) {
		unsigned long long start = __rdtsc(); // Marca inicio
		invoke(std::forward<Func>(algoritmo), std::forward<Args>(args)...); // Ejecuta el algoritmo
		unsigned long long end = __rdtsc();   // Marca fin

		unsigned long long ciclos = end - start; // Calcula ciclos usados
		double frecuencia_ghz = 4.87; // Obtiene frecuencia CPU
		if (frecuencia_ghz <= 0) {
			std::cerr << "[RDTSC] Error al obtener frecuencia de CPU\n";
			return;
		}

		double frecuencia_hz = frecuencia_ghz * 1e9;
		double tiempo_ns = ciclos * (1e9 / frecuencia_hz); // Convierte ciclos a nanosegundos

		long long segundos = static_cast<long long>(tiempo_ns) / 1'000'000'000;
		long long milisegundos = (static_cast<long long>(tiempo_ns) / 1'000'000) % 1000;
		long long nanosegundos = static_cast<long long>(tiempo_ns) % 1'000'000;

		// Imprime el tiempo formateado
		std::cout << "[RDTSC] "
				 << std::setfill('0') << std::setw(2) << segundos << ":"
				 << std::setfill('0') << std::setw(3) << milisegundos << ":"
				 << std::setfill('0') << std::setw(6) << nanosegundos << std::endl;
	}

	// Mide el tiempo usando GetSystemTimePreciseAsFileTime (Windows, alta precisión)
	template<typename Func, typename... Args>
	void herramienta_tiempo_4(Func&& algoritmo, Args&&... args) {
		using GetSystemTimePreciseAsFileTime_t = VOID (WINAPI*)(LPFILETIME);
		HMODULE hKernel = GetModuleHandleW(L"kernel32.dll"); // Obtiene handle de kernel32
		GetSystemTimePreciseAsFileTime_t pPrecise = nullptr;
		if (hKernel) {
			pPrecise = (GetSystemTimePreciseAsFileTime_t)GetProcAddress(hKernel, "GetSystemTimePreciseAsFileTime"); // Obtiene puntero a función
		}

		if (pPrecise) {
			FILETIME ftStart, ftEnd;
			pPrecise(&ftStart); // Marca inicio

			invoke(std::forward<Func>(algoritmo), std::forward<Args>(args)...); // Ejecuta el algoritmo

			pPrecise(&ftEnd);   // Marca fin

			ULARGE_INTEGER uStart, uEnd;
			uStart.LowPart  = ftStart.dwLowDateTime;
			uStart.HighPart = ftStart.dwHighDateTime;
			uEnd.LowPart    = ftEnd.dwLowDateTime;
			uEnd.HighPart   = ftEnd.dwHighDateTime;

			unsigned long long diff100ns = (uEnd.QuadPart - uStart.QuadPart); // Diferencia en intervalos de 100ns
			unsigned long long tiempo_ns  = diff100ns * 100ULL; // Convierte a nanosegundos

			long long segundos     = tiempo_ns / 1'000'000'000;
			long long milisegundos = (tiempo_ns / 1'000'000) % 1000;
			long long nanosegundos = tiempo_ns % 1'000'000;

			// Imprime el tiempo formateado
			std::cout << "[GetSystemTimePreciseAsFileTime] "
					  << std::setfill('0') << std::setw(2) << segundos << ":"
					  << std::setfill('0') << std::setw(3) << milisegundos << ":"
					  << std::setfill('0') << std::setw(6) << nanosegundos << std::endl;
		}
	}
};
