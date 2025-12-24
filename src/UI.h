//ui.h
#pragma once

#include <SFML/Graphics.hpp>
#include "Game.h"
#include <string>
#include <vector>
#include <variant>
#include <optional>

struct Button {
    sf::FloatRect bounds;
    std::string text;
    bool hovered;
    bool active;
};

class UI {
public:
    UI(unsigned int windowWidth, unsigned int windowHeight);
    ~UI();
    
    bool initialize();
    void handleEvent(sf::Event event, Game& game);
    void update(const Game& game);
    void render(sf::RenderWindow& window, const Game& game);
    
    bool isDifficultyMenuOpen() const;
    void setDifficultyMenuOpen(bool open);
    Difficulty getSelectedDifficulty() const;
    
private:
    unsigned int windowWidth_;
    unsigned int windowHeight_;
    
    sf::Font font_;
    bool fontLoaded_;
    
    // Board rendering
    float boardSize_;
    float cellSize_;
    float boardOffsetX_;
    float boardOffsetY_;
    
    // Colors
    sf::Color backgroundColor_;
    sf::Color gridColor_;
    sf::Color thickGridColor_;
    sf::Color cellColor_;
    sf::Color selectedCellColor_;
    sf::Color highlightColor_;
    sf::Color givenTextColor_;
    sf::Color userTextColor_;
    sf::Color errorColor_;
    sf::Color noteTextColor_;
    sf::Color buttonColor_;
    sf::Color buttonHoverColor_;
    sf::Color buttonTextColor_;
    
    // UI state
    bool difficultyMenuOpen_;
    Difficulty selectedDifficulty_;
    
    // Buttons
    std::vector<Button> buttons_;
    Button newGameButton_;
    Button hintButton_;
    Button notesButton_;
    Button pauseButton_;
    Button difficultyButton_;
    
    // Helper methods
    void setupButtons();
    void renderBoard(sf::RenderWindow& window, const Game& game);
    void renderCell(sf::RenderWindow& window, int row, int col, const Cell& cell, 
                    const Game& game);
    void renderNotes(sf::RenderWindow& window, int row, int col, const Cell& cell);
    void renderButtons(sf::RenderWindow& window);
    void renderTimer(sf::RenderWindow& window, const Game& game);
    void renderDifficultyMenu(sf::RenderWindow& window);
    void renderPauseOverlay(sf::RenderWindow& window);
    
    std::pair<int, int> getCellFromMouse(int mouseX, int mouseY) const;
    bool isPointInButton(const sf::Vector2f& point, const Button& button) const;
    void updateButtonHover(const sf::Vector2i& mousePos);
    
    sf::Text createText(const std::string& str, unsigned int size, 
                       const sf::Color& color, float x, float y) const;
};

