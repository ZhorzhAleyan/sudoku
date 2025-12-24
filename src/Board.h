//board.h
#pragma once

#include "Cell.h"
#include <array>
#include <vector>

class Board {
public:
    static constexpr int SIZE = 9;
    static constexpr int BOX_SIZE = 3;
    
    Board();
    
    Cell& getCell(int row, int col);
    const Cell& getCell(int row, int col) const;
    
    void setValue(int row, int col, int value);
    int getValue(int row, int col) const;
    
    bool isValidPlacement(int row, int col, int value) const;
    bool isComplete() const;
    
    void clear();
    void copyFrom(const Board& other);
    
    std::vector<std::pair<int, int>> getBoxCells(int boxRow, int boxCol) const;
    
private:
    std::array<std::array<Cell, SIZE>, SIZE> cells_;
};

