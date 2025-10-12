#ifndef PROBLEMAS_NP_H
#define PROBLEMAS_NP_H

#include <vector>
#include <memory>
#include <random>
#include <iostream>

class ProblemasNP {
private:
    // Miembros Sudoku
    std::unique_ptr<int[]> sudokuBoard;
    size_t boardSize;
    size_t subGridSize;
    std::vector<std::string> tiemposSudoku;

    // Miembros Partición
    std::vector<int> conjunto;
    std::vector<std::vector<int>> particiones;
    std::vector<std::string> tiemposParticion;

    // Random number generation
    std::mt19937 rng;

    // Private Sudoku helper methods
    bool isValidMove(size_t row, size_t col, int num) const;
    bool findEmptyCell(size_t& row, size_t& col) const;
    bool solveSudokuBacktrack();
    int& getCell(size_t row, size_t col);
    const int& getCell(size_t row, size_t col) const;

    // Private Set Partition helper methods
    bool canPartitionDP(const std::vector<int>& nums);
    void generateRandomSet(size_t size, int minVal, int maxVal);
    int calculateSetSum() const;

public:
    // Constructor
    ProblemasNP();

    // Sudoku public interface
    void createSudoku(size_t size);
    void printSudoku() const;
    void setRandomInitialValues(size_t numInitial);
    bool verifySudokuSolution();
    bool hasSolution();

    // Set Partition public interface
    void createRandomSet(size_t size, int minVal = 1, int maxVal = 10000000);
    bool findPartition();
    void reorderSets();
    int getSetSum() const;

    // Métodos de utilidad
    void seedRandom(unsigned seed);            
};

#endif // PROBLEMAS_NP_H