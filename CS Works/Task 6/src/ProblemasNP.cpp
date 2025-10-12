#include "../include/ProblemasNP.h"
#include <cmath>
#include <memory>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <iomanip>
#include <set>

// Constructor
ProblemasNP::ProblemasNP() 
    : boardSize(0), subGridSize(0), rng(std::random_device{}()) {}

// Utility methods
void ProblemasNP::seedRandom(unsigned seed) {
    rng.seed(seed);
}

// ============== Sudoku Implementation ==============

void ProblemasNP::createSudoku(size_t size) {
    
    subGridSize = static_cast<size_t>(std::ceil(std::sqrt(size)));
    boardSize = subGridSize * subGridSize;

    
    sudokuBoard = std::unique_ptr<int[]>(new int[boardSize * boardSize]);
    std::fill_n(sudokuBoard.get(), boardSize * boardSize, 0);
}

int& ProblemasNP::getCell(size_t row, size_t col) {
    return sudokuBoard[row * boardSize + col];
}

const int& ProblemasNP::getCell(size_t row, size_t col) const {
    return sudokuBoard[row * boardSize + col];
}

void ProblemasNP::printSudoku() const {
    for (size_t i = 0; i < boardSize; ++i) {
        if (i > 0 && i % subGridSize == 0) {
            for (size_t k = 0; k < boardSize * 2 + subGridSize; ++k) std::cout << "-";
            std::cout << "\n";
        }
        for (size_t j = 0; j < boardSize; ++j) {
            if (j > 0 && j % subGridSize == 0) std::cout << "| ";
            std::cout << getCell(i, j) << " ";
        }
        std::cout << "\n";
    }
}

bool ProblemasNP::isValidMove(size_t row, size_t col, int num) const {
    // Check row
    for (size_t x = 0; x < boardSize; x++) {
        if (getCell(row, x) == num) return false;
    }

    // Check column
    for (size_t x = 0; x < boardSize; x++) {
        if (getCell(x, col) == num) return false;
    }

    // Check sub-grid
    size_t startRow = row - row % subGridSize;
    size_t startCol = col - col % subGridSize;
    for (size_t i = 0; i < subGridSize; i++) {
        for (size_t j = 0; j < subGridSize; j++) {
            if (getCell(i + startRow, j + startCol) == num) return false;
        }
    }

    return true;
}

bool ProblemasNP::findEmptyCell(size_t& row, size_t& col) const {
    for (row = 0; row < boardSize; row++) {
        for (col = 0; col < boardSize; col++) {
            if (getCell(row, col) == 0) return true;
        }
    }
    return false;
}

bool ProblemasNP::solveSudokuBacktrack() {
    size_t row, col;
    
    if (!findEmptyCell(row, col)) return true;
    
    for (int num = 1; num <= static_cast<int>(boardSize); num++) {
        if (isValidMove(row, col, num)) {
            getCell(row, col) = num;
            
            if (solveSudokuBacktrack()) return true;
            
            getCell(row, col) = 0;
        }
    }
    return false;
}

void ProblemasNP::setRandomInitialValues(size_t numInitial) {
    if (numInitial > boardSize * boardSize) {
        throw std::invalid_argument("Too many initial values requested");
    }

    // Clear the board first
    std::fill_n(sudokuBoard.get(), boardSize * boardSize, 0);

    // Generate random positions
    std::vector<size_t> positions(boardSize * boardSize);
    std::iota(positions.begin(), positions.end(), 0);
    std::shuffle(positions.begin(), positions.end(), rng);

    // Try to fill numInitial cells
    size_t filled = 0;
    for (size_t i = 0; i < positions.size() && filled < numInitial; ++i) {
        size_t row = positions[i] / boardSize;
        size_t col = positions[i] % boardSize;
        
        std::vector<int> numbers(boardSize);
        std::iota(numbers.begin(), numbers.end(), 1);
        std::shuffle(numbers.begin(), numbers.end(), rng);
        
        for (int num : numbers) {
            if (isValidMove(row, col, num)) {
                getCell(row, col) = num;
                filled++;
                break;
            }
        }
    }
}

bool ProblemasNP::verifySudokuSolution() {
    for (size_t i = 0; i < boardSize; i++) {
        for (size_t j = 0; j < boardSize; j++) {
            if (getCell(i, j) == 0) return false;
        }
    }

    for (size_t i = 0; i < boardSize; i++) {
        for (size_t j = 0; j < boardSize; j++) {
            int num = getCell(i, j);
            getCell(i, j) = 0;
            if (!isValidMove(i, j, num)) {
                getCell(i, j) = num;
                return false;
            }
            getCell(i, j) = num;
        }
    }

    return true;
}

bool ProblemasNP::hasSolution() {

    auto tempBoard = std::unique_ptr<int[]>(new int[boardSize * boardSize]);
    std::copy_n(sudokuBoard.get(), boardSize * boardSize, tempBoard.get());
    bool solvable = solveSudokuBacktrack();
    std::copy_n(tempBoard.get(), boardSize * boardSize, sudokuBoard.get());
    
    return solvable;
}

// ============== Set Partition Implementation ==============

void ProblemasNP::createRandomSet(size_t size, int minVal, int maxVal) {
    conjunto.clear();
    conjunto.reserve(size);
    
    std::uniform_int_distribution<> dist(minVal, maxVal);
    for (size_t i = 0; i < size; ++i) {
        conjunto.push_back(dist(rng));
    }
}

bool ProblemasNP::canPartitionDP(const std::vector<int>& nums) {
    int sum = std::accumulate(nums.begin(), nums.end(), 0);
    if (sum % 2 != 0) return false;
    
    int target = sum / 2;
    std::vector<bool> dp(target + 1, false);
    dp[0] = true;
    
    for (int num : nums) {
        for (int j = target; j >= num; j--) {
            dp[j] = dp[j] || dp[j - num];
        }
    }
    
    return dp[target];
}

bool ProblemasNP::findPartition() {
    if (conjunto.empty()) return false;

    if (!canPartitionDP(conjunto)) return false;
    particiones.clear();
    particiones.resize(2);

    int totalSum = std::accumulate(conjunto.begin(), conjunto.end(), 0);
    int targetSum = totalSum / 2;
    
    std::vector<std::vector<bool>> dp(conjunto.size() + 1, std::vector<bool>(targetSum + 1, false));
    for (size_t i = 0; i <= conjunto.size(); i++) dp[i][0] = true;

    for (size_t i = 1; i <= conjunto.size(); i++) {
        for (int j = 1; j <= targetSum; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j >= conjunto[i - 1]) {
                dp[i][j] = dp[i][j] || dp[i - 1][j - conjunto[i - 1]];
            }
        }
    }

    int i = conjunto.size(), j = targetSum;
    while (i > 0 && j > 0) {
        if (dp[i][j] != dp[i - 1][j]) {
            particiones[0].push_back(conjunto[i - 1]);
            j -= conjunto[i - 1];
        } else {
            particiones[1].push_back(conjunto[i - 1]);
        }
        i--;
    }

    return true;
}

void ProblemasNP::reorderSets() {
    for (auto& particion : particiones) {
        std::sort(particion.begin(), particion.end());
    }
    std::sort(particiones.begin(), particiones.end());
}

int ProblemasNP::calculateSetSum() const {
    return std::accumulate(conjunto.begin(), conjunto.end(), 0);
}

int ProblemasNP::getSetSum() const {
    return calculateSetSum();
}
