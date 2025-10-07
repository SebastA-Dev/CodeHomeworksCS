// ...existing code...

// Radix sort (byte-wise MSD) copied from Task 4 implementation
#include <vector>
#include <cstdint>

namespace {
	static void msd_byte(std::vector<uint32_t>& u, std::vector<uint32_t>& aux,
						 int inicio, int fin, int b) {
		if (inicio >= fin || b < 0) return;

		const int RADIX = 256;

		// contar ocurrencias del byte
		std::vector<int> contador(RADIX, 0);
		int desplazamiento = b * 8;
		for (int i = inicio; i <= fin; ++i) {
			int byte = static_cast<int>((u[i] >> desplazamiento) & 0xFFu);
			++contador[byte];
		}

		// calcular indices iniciales
		std::vector<int> inicioBucket(RADIX);
		inicioBucket[0] = inicio;
		for (int i = 1; i < RADIX; ++i) inicioBucket[i] = inicioBucket[i-1] + contador[i-1];

		// colocar elementos en aux
		for (int i = inicio; i <= fin; ++i) {
			int byte = static_cast<int>((u[i] >> desplazamiento) & 0xFFu);
			aux[inicioBucket[byte]++] = u[i];
		}

		// copiar de vuelta a u
		for (int i = inicio; i <= fin; ++i) u[i] = aux[i];

		// recursión por buckets
		int inicioActual = inicio;
		for (int i = 0; i < RADIX; ++i) {
			int cuenta = contador[i];
			if (cuenta > 1) {
				msd_byte(u, aux, inicioActual, inicioActual + cuenta - 1, b - 1);
			}
			inicioActual += cuenta;
		}
	}
}

// Public API for this file
void radixSort(std::vector<int>& array) {
	if (array.empty()) return;

	using u32 = uint32_t;
	const u32 INVERSOR_SIGNO = 0x80000000u;

	std::vector<u32> u;
	u.reserve(array.size());
	for (int x : array) u.push_back(static_cast<u32>(static_cast<int32_t>(x)) ^ INVERSOR_SIGNO);

	std::vector<u32> aux(u.size());

	// iniciar con el byte más significativo (3 para 32-bit)
	msd_byte(u, aux, 0, static_cast<int>(u.size()) - 1, 3);

	// restaurar signos y escribir en array
	for (std::size_t i = 0; i < u.size(); ++i) array[i] = static_cast<int32_t>(u[i] ^ INVERSOR_SIGNO);
}

