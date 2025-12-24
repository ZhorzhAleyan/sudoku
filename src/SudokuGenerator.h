//generator.h
#pragma once

#include "Board.h"
#include "SudokuSolver.h"
#include <random>

enum class Difficulty {
    EASY = 0,
    MEDIUM = 1,
    HARD = 2,
    EXPERT = 3
};

class SudokuGenerator {
public:
    static Board generate(Difficulty difficulty);
    
private:
    static Board generateFullBoard();
    static Board removeCells(Board board, Difficulty difficulty);
    static int getClueCount(Difficulty difficulty);
};

