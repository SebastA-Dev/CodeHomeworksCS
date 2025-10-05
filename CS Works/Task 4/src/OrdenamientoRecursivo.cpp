#include "../include/OrdenamientoRecursivo.h"
#include <algorithm>
#include <vector>
#include <cstdint>
#include <functional>

namespace task4 {

namespace {
    
    void particion(std::vector<int>& array, int low, int high) {
        if (low >= high) return;
        int pivote = array[high];
        int i = low, j = high;
        while (i <= j) {
            while (array[i] < pivote) ++i;
            while (array[j] > pivote) --j;
            if (i <= j) {
                std::swap(array[i], array[j]);
                ++i; --j;
            }
        }
        if (low < j) particion(array, low, j);
        if (i < high) particion(array, i, high);
    }
    
    static void msd(std::vector<uint32_t>& u, std::vector<uint32_t>& aux,
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

        // colocar elementos en aux b
        for (int i = inicio; i <= fin; ++i) {
            int byte = static_cast<int>((u[i] >> desplazamiento) & 0xFFu);
            aux[inicioBucket[byte]++] = u[i];
        }

        // copiar de vuelta a u
        for (int i = inicio; i <= fin; ++i) u[i] = aux[i];

        // ordenar al menos significativo
        int inicioActual = inicio;
        for (int i = 0; i < RADIX; ++i) {
            int cuenta = contador[i];
            if (cuenta > 1) {
                msd(u, aux, inicioActual, inicioActual + cuenta - 1, b - 1);
            }
            inicioActual += cuenta;
        }
    }
}

void OrdenamientoRecursivo::quickSort(std::vector<int>& array) {
    if (array.empty()) return;
    particion(array, 0, static_cast<int>(array.size()) - 1);
}

void OrdenamientoRecursivo::radixSort(std::vector<int>& array) {
    if (array.empty()) return;

    using u32 = uint32_t;
    const u32 INVERSOR_SIGNO = 0x80000000u;

    std::vector<u32> u;

    //asegura la suficiente memoria para el array
    u.reserve(array.size());
    for (int x : array) u.push_back(static_cast<u32>(static_cast<int32_t>(x)) ^ INVERSOR_SIGNO);

    std::vector<u32> aux(u.size());

    const int RADIX = 256;

    // iniciar con el mas signficativo
    msd(u, aux, 0, static_cast<int>(u.size()) - 1, 3);

    // ingresar los signos
    for (std::size_t i = 0; i < u.size(); ++i) array[i] = static_cast<int32_t>(u[i] ^ INVERSOR_SIGNO);
}

namespace {
    static void MergeLists(std::vector<int>& lista, int start1, int end1, int start2, int end2) {        
        int finalStart = start1;
        int finalEnd = end2;

        int tam = finalEnd - finalStart + 1;
        std::vector<int> result(tam);

        int indexC = 0;

        int i1 = start1;
        int i2 = start2;

        while (i1 <= end1 && i2 <= end2) {
            if (lista[i1] < lista[i2]) {
                result[indexC++] = lista[i1++];
            } else {
                result[indexC++] = lista[i2++];
            }
        }

        if (i1 <= end1) {
            for (int i = i1; i <= end1; ++i) result[indexC++] = lista[i];
        } else {
            for (int i = i2; i <= end2; ++i) result[indexC++] = lista[i];
        }

        // copiar resultado de vuelta
        indexC = 0;
        for (int i = finalStart; i <= finalEnd; ++i) lista[i] = result[indexC++];
    }
}

void OrdenamientoRecursivo::mergeSort(std::vector<int>& array, int first, int last) {
    if (first < last) {
            int middle = (first + last) / 2;
            mergeSort(array, first, middle);
            mergeSort(array, middle + 1, last);
            MergeLists(array, first, middle, middle + 1, last);
        }
    }
} 
