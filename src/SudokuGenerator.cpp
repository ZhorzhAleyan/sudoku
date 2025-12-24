//generator_cpp
#include "SudokuGenerator.h"
#include <algorithm>
#include <random>
#include <vector>

Board SudokuGenerator::generate(Difficulty difficulty) {
    Board board = generateFullBoard();
    return removeCells(board, difficulty);
}

Board SudokuGenerator::generateFullBoard() {
    Board board;
    SudokuSolver::solve(board);
    return board;
}

Board SudokuGenerator::removeCells(Board board, Difficulty difficulty) {
    // Mark all cells as given initially
    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            board.getCell(r, c).setGiven(true);
        }
    }
    
    int targetClues = getClueCount(difficulty);
    int cellsToRemove = Board::SIZE * Board::SIZE - targetClues;
    
    // Create list of all positions
    std::vector<std::pair<int, int>> positions;
    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            positions.emplace_back(r, c);
        }
    }
    
    // Shuffle positions
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(positions.begin(), positions.end(), g);
    
    // Try to remove cells while maintaining unique solution
    int removed = 0;
    for (const auto& pos : positions) {
        if (removed >= cellsToRemove) break;
        
        int row = pos.first;
        int col = pos.second;
        int value = board.getValue(row, col);
        
        // Temporarily remove the cell
        board.setValue(row, col, 0);
        board.getCell(row, col).setGiven(false);
        
        // Check if solution is still unique
        if (SudokuSolver::hasUniqueSolution(board)) {
            removed++;
        } else {
            // Restore the cell if removal breaks uniqueness
            board.setValue(row, col, value);
            board.getCell(row, col).setGiven(true);
        }
    }
    
    return board;
}

int SudokuGenerator::getClueCount(Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::EASY:
            return 45; // ~50% clues
        case Difficulty::MEDIUM:
            return 36; // ~40% clues
        case Difficulty::HARD:
            return 27; // ~30% clues
        case Difficulty::EXPERT:
            return 17; // ~19% clues (minimum for unique solution)
        default:
            return 36;
    }
}

