#include "../include/RandomVector.h"
#include <random>

std::vector<int> make_random_vector(std::size_t n) {
    std::vector<int> arreglo;
    arreglo.reserve(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1000000);
    for (std::size_t i = 0; i < n; ++i) arreglo.push_back(dist(gen));
    return arreglo;
}

std::vector<int> make_random_vector_big_numbers(std::size_t n) {
    std::vector<int> arreglo;
    arreglo.reserve(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1000000, 10000000);
    for (std::size_t i = 0; i < n; ++i) arreglo.push_back(dist(gen));
    return arreglo;
}
