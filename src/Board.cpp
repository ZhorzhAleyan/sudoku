//board.cpp
#include "Board.h"

Board::Board() {
    clear();
}

Cell& Board::getCell(int row, int col) {
    return cells_[row][col];
}

const Cell& Board::getCell(int row, int col) const {
    return cells_[row][col];
}

void Board::setValue(int row, int col, int value) {
    if (row >= 0 && row < SIZE && col >= 0 && col < SIZE) {
        cells_[row][col].setValue(value);
    }
}

int Board::getValue(int row, int col) const {
    if (row >= 0 && row < SIZE && col >= 0 && col < SIZE) {
        return cells_[row][col].getValue();
    }
    return 0;
}

bool Board::isValidPlacement(int row, int col, int value) const {
    if (value < 1 || value > 9) return false;
    
    // Check row
    for (int c = 0; c < SIZE; ++c) {
        if (c != col && cells_[row][c].getValue() == value) {
            return false;
        }
    }
    
    // Check column
    for (int r = 0; r < SIZE; ++r) {
        if (r != row && cells_[r][col].getValue() == value) {
            return false;
        }
    }
    
    // Check 3x3 box
    int boxRow = (row / BOX_SIZE) * BOX_SIZE;
    int boxCol = (col / BOX_SIZE) * BOX_SIZE;
    
    for (int r = boxRow; r < boxRow + BOX_SIZE; ++r) {
        for (int c = boxCol; c < boxCol + BOX_SIZE; ++c) {
            if (r != row && c != col && cells_[r][c].getValue() == value) {
                return false;
            }
        }
    }
    
    return true;
}

bool Board::isComplete() const {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (cells_[r][c].isEmpty() || !isValidPlacement(r, c, cells_[r][c].getValue())) {
                return false;
            }
        }
    }
    return true;
}

void Board::clear() {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            cells_[r][c] = Cell();
        }
    }
}

void Board::copyFrom(const Board& other) {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            cells_[r][c] = other.cells_[r][c];
        }
    }
}

std::vector<std::pair<int, int>> Board::getBoxCells(int boxRow, int boxCol) const {
    std::vector<std::pair<int, int>> cells;
    int startRow = boxRow * BOX_SIZE;
    int startCol = boxCol * BOX_SIZE;
    
    for (int r = startRow; r < startRow + BOX_SIZE; ++r) {
        for (int c = startCol; c < startCol + BOX_SIZE; ++c) {
            cells.emplace_back(r, c);
        }
    }
    
    return cells;
}

