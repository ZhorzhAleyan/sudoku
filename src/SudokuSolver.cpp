// solver.cpp
#include "SudokuSolver.h"
#include <algorithm>
#include <random>

SudokuSolver::Result SudokuSolver::solve(Board& board) {
    Board copy = board;
    if (solveRecursive(copy)) {
        board = copy;
        return Result::SOLVED;
    }
    return Result::UNSOLVABLE;
}

SudokuSolver::Result SudokuSolver::countSolutions(const Board& board, int maxSolutions) {
    int count = 0;
    Board copy = board;
    countSolutionsRecursive(copy, count, maxSolutions);
    
    if (count == 0) {
        return Result::UNSOLVABLE;
    } else if (count == 1) {
        return Result::SOLVED;
    } else {
        return Result::MULTIPLE_SOLUTIONS;
    }
}

bool SudokuSolver::hasUniqueSolution(const Board& board) {
    return countSolutions(board, 2) == Result::SOLVED;
}

bool SudokuSolver::solveRecursive(Board& board) {
    auto empty = findEmptyCell(board);
    if (empty.first == -1) {
        return true; // Board is complete
    }
    
    int row = empty.first;
    int col = empty.second;
    
    auto numbers = getShuffledNumbers();
    for (int num : numbers) {
        if (board.isValidPlacement(row, col, num)) {
            board.setValue(row, col, num);
            if (solveRecursive(board)) {
                return true;
            }
            board.setValue(row, col, 0);
        }
    }
    
    return false;
}

int SudokuSolver::countSolutionsRecursive(Board board, int& count, int maxSolutions) {
    if (count >= maxSolutions) {
        return count;
    }
    
    auto empty = findEmptyCell(board);
    if (empty.first == -1) {
        count++;
        return count;
    }
    
    int row = empty.first;
    int col = empty.second;
    
    for (int num = 1; num <= 9; ++num) {
        if (board.isValidPlacement(row, col, num)) {
            board.setValue(row, col, num);
            countSolutionsRecursive(board, count, maxSolutions);
            if (count >= maxSolutions) {
                return count;
            }
        }
    }
    
    return count;
}

std::pair<int, int> SudokuSolver::findEmptyCell(const Board& board) {
    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            if (board.getCell(r, c).isEmpty()) {
                return {r, c};
            }
        }
    }
    return {-1, -1};
}

std::vector<int> SudokuSolver::getShuffledNumbers() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);
    return numbers;
}

