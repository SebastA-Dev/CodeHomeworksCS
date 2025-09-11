#include <iostream>
#include <windows.h>
#include <x86intrin.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;
// Clase para medir el tiempo de ejecución de algoritmos usando diferentes métodos
class TimeComplexAlgorithms {

public:

    double obtener_velocidad_CPU() {
        std::array<char, 128> buffer;
        std::string result;
        FILE* pipe = _popen("wmic cpu get currentclockspeed", "r");
        if (!pipe) {
            return -1;
        }

        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
        _pclose(pipe);

        std::string::size_type pos = result.find_first_of("0123456789");
        if (pos != std::string::npos) {
            int mhz = std::stoi(result.substr(pos));
            return mhz / 1000.0; 
        }

        return -1; 
    }


	// Mide el tiempo usando std::chrono (alta resolución)
	// Recibe N y un puntero a función que retorna vector<size_t>
	void herramienta_tiempo_1(int N, std::vector<size_t> (*algoritmo)(int, int)) {
		auto start = std::chrono::high_resolution_clock::now(); // Marca inicio
		std::vector<size_t> mem = algoritmo(N, N); // Ejecuta el algoritmo
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
	void herramienta_tiempo_2(int N, std::vector<size_t> (*algoritmo)(int, int)) {
		LARGE_INTEGER freq, start, end;
		QueryPerformanceFrequency(&freq); // Obtiene frecuencia del contador
		QueryPerformanceCounter(&start);  // Marca inicio

		std::vector<size_t> mem = algoritmo(N, N); // Ejecuta el algoritmo

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
	void herramienta_tiempo_3(int N, std::vector<size_t> (*algoritmo)(int, int)) {
		unsigned long long start = __rdtsc(); // Marca inicio
		std::vector<size_t> mem = algoritmo(N,N); // Ejecuta el algoritmo
		unsigned long long end = __rdtsc();   // Marca fin

		unsigned long long ciclos = end - start; // Calcula ciclos usados
		double frecuencia_ghz = obtener_velocidad_CPU(); // Obtiene frecuencia CPU
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
	void herramienta_tiempo_4(int N, std::vector<size_t> (*algoritmo)(int, int)) {
		using GetSystemTimePreciseAsFileTime_t = VOID (WINAPI*)(LPFILETIME);
		HMODULE hKernel = GetModuleHandleW(L"kernel32.dll"); // Obtiene handle de kernel32
		GetSystemTimePreciseAsFileTime_t pPrecise = nullptr;
		if (hKernel) {
			pPrecise = (GetSystemTimePreciseAsFileTime_t)GetProcAddress(hKernel, "GetSystemTimePreciseAsFileTime"); // Obtiene puntero a función
		}

		if (pPrecise) {
			FILETIME ftStart, ftEnd;
			pPrecise(&ftStart); // Marca inicio

			std::vector<size_t> mem = algoritmo(N, N); // Ejecuta el algoritmo

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
