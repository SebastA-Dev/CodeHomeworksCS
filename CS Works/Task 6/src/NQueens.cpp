#include "NQueens.h"
#include <iostream>
#include <cmath>

NQueens::NQueens(size_t n) : queen_pos(n, -1), N(n), solutionFound(false) { }

bool NQueens::isSafe(int row, int col) const {
    for (int i = 0; i < row; ++i) {
        if (queen_pos[i] == col || std::abs(queen_pos[i] - col) == (row - i))
            return false;
    }
    return true;
}

bool NQueens::solve(int row) {
    if (row == (int)N) {
        solutionFound = true;
        return true;
    }
    for (int col = 0; col < (int)N; ++col) {
        if (isSafe(row, col)) {
            queen_pos[row] = col;
            if (solve(row + 1)) return true;
        }
    }
    queen_pos[row] = -1;
    return false;
}

bool NQueens::findSolution() {
    solutionFound = false;
    return solve(0);
}

void NQueens::printBoard() const {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j)
            std::cout << (queen_pos[i] == (int)j ? "Q " : ". ");
        std::cout << "\n";
    }
}

