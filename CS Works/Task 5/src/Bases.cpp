#include "../include/Bases.h"
#include <vector>
#include <algorithm>
#include <cstdint>

namespace {
    
    static void lsd(std::vector<int>& a, std::vector<int>& aux, int start, int end, int exp) {
        if (start >= end || exp <= 0) return;

        // Base 10 por la cual se recorre cada valor del array
        const int base = 10;
        std::vector<int> count(base, 0);

        for (int i = start; i <= end; ++i) {
            int d = (a[i] / exp) % base;
            ++count[d];
        }

        std::vector<int> idx(base);
        idx[0] = start;
        for (int i = 1; i < base; ++i) idx[i] = idx[i-1] + count[i-1];

        for (int i = start; i <= end; ++i) {
            int d = (a[i] / exp) % base;
            aux[idx[d]++] = a[i];
        }

        for (int i = start; i <= end; ++i) a[i] = aux[i];

        int cur = start;
        for (int b = 0; b < base; ++b) {
            int cnt = count[b];
            if (cnt > 1) {
                lsd(a, aux, cur, cur + cnt - 1, exp / base);
            }
            cur += cnt;
        }
    }
}

void basesSort(std::vector<int>& array) {
    if (array.empty()) return;

    const int offset = 1000000000;
    for (auto &x : array) x += offset;

    // Parte la cual limpia valores negativos y los separa
    int maxv = *std::max_element(array.begin(), array.end());

    //exponente, para recorrer por unidades, decenas ...
    int exp = 1;

    while (maxv / exp >= 10) exp *= 10;

    std::vector<int> aux(array.size());

    //llamada recursiva
    lsd(array, aux, 0, static_cast<int>(array.size()) - 1, exp);

    for (auto &x : array) x -= offset;
}
