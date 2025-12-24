//game.h
#pragma once

#include "Board.h"
#include "SudokuGenerator.h"
#include "SudokuSolver.h"
#include <chrono>
#include <string>

class Game {
public:
    Game();
    Difficulty getCurrentDifficulty() const;
    void newGame(Difficulty difficulty);
    void setValue(int row, int col, int value);
    void setNote(int row, int col, int value);
    void clearCell(int row, int col);
    
    bool isComplete() const;
    bool isPaused() const;
    void pause();
    void resume();
    void togglePause();
    
    void updateTimer();
    std::string getTimerString() const;
    int getElapsedSeconds() const;
    
    void useHint();
    int getHintsRemaining() const;
    
    void toggleNotesMode();
    bool isNotesMode() const;
    
    void selectCell(int row, int col);
    std::pair<int, int> getSelectedCell() const;
    
    Board& getBoard();
    const Board& getBoard() const;
    
    void checkErrors();
    
private:
    Board board_;
    Board solution_;
    Difficulty currentDifficulty_;
    
    std::pair<int, int> selectedCell_;
    bool notesMode_;
    int hintsRemaining_;
    
    bool paused_;
    std::chrono::steady_clock::time_point startTime_;
    std::chrono::steady_clock::time_point pauseStartTime_;
    int accumulatedPauseTime_;
    int totalElapsedSeconds_;
    
    void generateSolution();
};

