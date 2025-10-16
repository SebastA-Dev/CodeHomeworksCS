#ifndef NQUEENS_H
#define NQUEENS_H

#include <vector>
#include <cstddef>

class NQueens {
private:
    std::vector<int> queen_pos;
    size_t N;
    bool solutionFound;
    bool isSafe(int row, int col) const;
    bool solve(int row = 0);

public:
    NQueens(size_t n);
    bool findSolution();
    void printBoard() const;
};

#endif // NQUEENS_H

