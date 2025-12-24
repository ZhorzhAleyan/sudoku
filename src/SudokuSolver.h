//solver.h
#pragma once

#include "Board.h"
#include <vector>
#include <random>

class SudokuSolver {
public:
    enum class Result {
        SOLVED,
        UNSOLVABLE,
        MULTIPLE_SOLUTIONS
    };
    
    static Result solve(Board& board);
    static Result countSolutions(const Board& board, int maxSolutions = 2);
    static bool hasUniqueSolution(const Board& board);
    
private:
    static bool solveRecursive(Board& board);
    static int countSolutionsRecursive(Board board, int& count, int maxSolutions);
    static std::pair<int, int> findEmptyCell(const Board& board);
    static std::vector<int> getShuffledNumbers();
};

