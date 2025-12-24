//game.cpp
#include "Game.h"
#include <algorithm>

Game::Game() 
    : currentDifficulty_(Difficulty::MEDIUM)
    , selectedCell_({-1, -1})
    , notesMode_(false)
    , hintsRemaining_(3)
    , paused_(false)
    , accumulatedPauseTime_(0)
    , totalElapsedSeconds_(0) {
    newGame(Difficulty::MEDIUM);
}

void Game::newGame(Difficulty difficulty) {
    currentDifficulty_ = difficulty;
    board_ = SudokuGenerator::generate(difficulty);
    generateSolution();
    
    selectedCell_ = {-1, -1};
    notesMode_ = false;
    hintsRemaining_ = 3;
    paused_ = false;
    accumulatedPauseTime_ = 0;
    totalElapsedSeconds_ = 0;
    startTime_ = std::chrono::steady_clock::now();
}

void Game::generateSolution() {
    solution_ = board_;
    SudokuSolver::solve(solution_);
}

void Game::setValue(int row, int col, int value) {
    if (row < 0 || row >= Board::SIZE || col < 0 || col >= Board::SIZE) {
        return;
    }
    
    Cell& cell = board_.getCell(row, col);
    if (cell.isGiven()) {
        return; // Cannot modify given cells
    }
    
    if (notesMode_) {
        if (cell.hasNote(value)) {
            cell.removeNote(value);
        } else {
            cell.addNote(value);
        }
    } else {
        cell.setValue(value);
        cell.clearNotes();
        checkErrors();
    }
}

void Game::setNote(int row, int col, int value) {
    if (row < 0 || row >= Board::SIZE || col < 0 || col >= Board::SIZE) {
        return;
    }
    
    Cell& cell = board_.getCell(row, col);
    if (!cell.isGiven() && cell.isEmpty()) {
        if (cell.hasNote(value)) {
            cell.removeNote(value);
        } else {
            cell.addNote(value);
        }
    }
}

void Game::clearCell(int row, int col) {
    if (row < 0 || row >= Board::SIZE || col < 0 || col >= Board::SIZE) {
        return;
    }
    
    Cell& cell = board_.getCell(row, col);
    if (!cell.isGiven()) {
        cell.setValue(0);
        cell.clearNotes();
        cell.setError(false);
    }
}

bool Game::isComplete() const {
    return board_.isComplete();
}

bool Game::isPaused() const {
    return paused_;
}

void Game::pause() {
    if (!paused_) {
        paused_ = true;
        pauseStartTime_ = std::chrono::steady_clock::now();
    }
}

void Game::resume() {
    if (paused_) {
        auto pauseEnd = std::chrono::steady_clock::now();
        auto pauseDuration = std::chrono::duration_cast<std::chrono::seconds>(
            pauseEnd - pauseStartTime_).count();
        accumulatedPauseTime_ += pauseDuration;
        paused_ = false;
    }
}

void Game::togglePause() {
    if (paused_) {
        resume();
    } else {
        pause();
    }
}

void Game::updateTimer() {
    if (!paused_) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            now - startTime_).count();
        totalElapsedSeconds_ = elapsed - accumulatedPauseTime_;
    }
}

std::string Game::getTimerString() const {
    int hours = totalElapsedSeconds_ / 3600;
    int minutes = (totalElapsedSeconds_ % 3600) / 60;
    int seconds = totalElapsedSeconds_ % 60;
    
    if (hours > 0) {
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
        return std::string(buffer);
    } else {
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
        return std::string(buffer);
    }
}

int Game::getElapsedSeconds() const {
    return totalElapsedSeconds_;
}

void Game::useHint() {
    if (hintsRemaining_ <= 0) {
        return;
    }
    
    if (selectedCell_.first == -1 || selectedCell_.second == -1) {
        return;
    }
    
    int row = selectedCell_.first;
    int col = selectedCell_.second;
    
    Cell& cell = board_.getCell(row, col);
    if (!cell.isGiven() && cell.isEmpty()) {
        int correctValue = solution_.getValue(row, col);
        cell.setValue(correctValue);
        cell.clearNotes();
        cell.setError(false);
        hintsRemaining_--;
    }
}

int Game::getHintsRemaining() const {
    return hintsRemaining_;
}

void Game::toggleNotesMode() {
    notesMode_ = !notesMode_;
}

bool Game::isNotesMode() const {
    return notesMode_;
}

void Game::selectCell(int row, int col) {
    if (row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE) {
        selectedCell_ = {row, col};
    }
}

std::pair<int, int> Game::getSelectedCell() const {
    return selectedCell_;
}

Board& Game::getBoard() {
    return board_;
}
Difficulty Game::getCurrentDifficulty() const {
    return currentDifficulty_;
}
const Board& Game::getBoard() const {
    return board_;
}

void Game::checkErrors() {
    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            Cell& cell = board_.getCell(r, c);
            if (!cell.isEmpty()) {
                bool isValid = board_.isValidPlacement(r, c, cell.getValue());
                cell.setError(!isValid);
            } else {
                cell.setError(false);
            }
        }
    }
}

