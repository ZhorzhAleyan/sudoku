// ui.cpp
#include "UI.h"
#include <sstream>
#include <type_traits>

UI::UI(unsigned int windowWidth, unsigned int windowHeight)
    : windowWidth_(windowWidth)
    , windowHeight_(windowHeight)
    , fontLoaded_(false)
    , boardSize_(540.0f)
    , cellSize_(boardSize_ / 9.0f)
    , boardOffsetX_(50.0f)
    , boardOffsetY_(50.0f)
    , difficultyMenuOpen_(false)
    , selectedDifficulty_(Difficulty::MEDIUM) {
    
    // Modern color scheme
    backgroundColor_ = sf::Color(245, 245, 245);
    gridColor_ = sf::Color(200, 200, 200);
    thickGridColor_ = sf::Color(100, 100, 100);
    cellColor_ = sf::Color(255, 255, 255);
    selectedCellColor_ = sf::Color(173, 216, 230)
    highlightColor_ = sf::Color(230, 240, 255);
    givenTextColor_ = sf::Color(0, 0, 0);
    userTextColor_ = sf::Color(30, 144, 255);
    errorColor_ = sf::Color(255, 0, 0);
    noteTextColor_ = sf::Color(100, 100, 100);
    buttonColor_ = sf::Color(70, 130, 180);
    buttonHoverColor_ = sf::Color(100, 149, 237);
    buttonTextColor_ = sf::Color(255, 255, 255);
    
    setupButtons();
}

UI::~UI() {
}

bool UI::initialize() {
    // Try to load font from assets
    // First try relative path (for running from build directory)
    if (font_.openFromFile("assets/fonts/DejaVuSans.ttf")) {
        fontLoaded_ = true;
        return true;
    }
    
    // Try absolute path from source directory
    if (font_.openFromFile("../assets/fonts/DejaVuSans.ttf")) {
        fontLoaded_ = true;
        return true;
    }
    
    // Fallback: try Roboto if DejaVuSans not found
    if (font_.openFromFile("assets/fonts/Roboto-Regular.ttf")) {
        fontLoaded_ = true;
        return true;
    }
    
    // Font not found - game will still run but text may not render
    fontLoaded_ = false;
    return false;
}

void UI::setupButtons() {
    float buttonWidth = 120.0f;
    float buttonHeight = 40.0f;
    float buttonSpacing = 10.0f;
    float startX = boardOffsetX_ + boardSize_ + 30.0f;
    float startY = boardOffsetY_;
    
    newGameButton_.bounds = sf::FloatRect(sf::Vector2f(startX, startY), sf::Vector2f(buttonWidth, buttonHeight));
    newGameButton_.text = "New Game";
    newGameButton_.hovered = false;
    newGameButton_.active = false;
    
    hintButton_.bounds = sf::FloatRect(sf::Vector2f(startX, startY + (buttonHeight + buttonSpacing)), sf::Vector2f(buttonWidth, buttonHeight));
    hintButton_.text = "Hint";
    hintButton_.hovered = false;
    hintButton_.active = false;
    
    notesButton_.bounds = sf::FloatRect(sf::Vector2f(startX, startY + 2 * (buttonHeight + buttonSpacing)), sf::Vector2f(buttonWidth, buttonHeight));
    notesButton_.text = "Notes";
    notesButton_.hovered = false;
    notesButton_.active = false;
    
    pauseButton_.bounds = sf::FloatRect(sf::Vector2f(startX, startY + 3 * (buttonHeight + buttonSpacing)), sf::Vector2f(buttonWidth, buttonHeight));
    pauseButton_.text = "Pause";
    pauseButton_.hovered = false;
    pauseButton_.active = false;
    
    difficultyButton_.bounds = sf::FloatRect(sf::Vector2f(startX, startY + 4 * (buttonHeight + buttonSpacing)), sf::Vector2f(buttonWidth, buttonHeight));
    difficultyButton_.text = "Difficulty";
    difficultyButton_.hovered = false;
    difficultyButton_.active = false;
    
    buttons_ = {newGameButton_, hintButton_, notesButton_, pauseButton_, difficultyButton_};
}

void UI::handleEvent(sf::Event event, Game& game) {
    // SFML 3.x event handling using is() and getIf()
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto* e = event.getIf<sf::Event::MouseButtonPressed>();
        if (!e) return;
        if (e->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos(e->position.x, e->position.y);
            
            if (difficultyMenuOpen_ || game.isPaused()) {
                if (difficultyMenuOpen_) {
                    // Handle difficulty menu clicks
                    float menuX = windowWidth_ / 2.0f - 150.0f;
                    float menuY = windowHeight_ / 2.0f - 150.0f;
                    float itemHeight = 40.0f;
                    
                    for (int i = 0; i < 4; ++i) {
                        sf::FloatRect itemRect(sf::Vector2f(menuX, menuY + i * itemHeight), 
                                               sf::Vector2f(300.0f, itemHeight));
                        if (itemRect.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) {
                            selectedDifficulty_ = static_cast<Difficulty>(i);
                            game.newGame(selectedDifficulty_);
                            difficultyMenuOpen_ = false;
                            return;
                        }
                    }
                    
                    // Check if clicked outside menu
                    sf::FloatRect menuRect(sf::Vector2f(menuX, menuY), sf::Vector2f(300.0f, 160.0f));
                    if (!menuRect.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) {
                        difficultyMenuOpen_ = false;
                    }
                } else if (game.isPaused()) {
                    // Resume on click during pause
                    game.resume();
                }
                return;
            }
            
            // Check buttons
            sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            
            if (isPointInButton(mousePosF, newGameButton_)) {
                game.newGame(selectedDifficulty_);
            } else if (isPointInButton(mousePosF, hintButton_)) {
                game.useHint();
            } else if (isPointInButton(mousePosF, notesButton_)) {
                game.toggleNotesMode();
            } else if (isPointInButton(mousePosF, pauseButton_)) {
                game.togglePause();
            } else if (isPointInButton(mousePosF, difficultyButton_)) {
                game.pause();
                difficultyMenuOpen_ = true;
            } else {
                // Check board click
                auto cell = getCellFromMouse(mousePos.x, mousePos.y);
                if (cell.first >= 0 && cell.first < 9 && cell.second >= 0 && cell.second < 9) {
                    game.selectCell(cell.first, cell.second);
                }
            }
        }
    } else if (event.is<sf::Event::KeyPressed>()) {
        const auto* e = event.getIf<sf::Event::KeyPressed>();
        if (!e) return;
        if (difficultyMenuOpen_ || game.isPaused()) {
            if (e->code == sf::Keyboard::Key::Escape) {
                if (difficultyMenuOpen_) {
                    difficultyMenuOpen_ = false;
                } else {
                    game.resume();
                }
            }
            return;
        }
        
        auto selected = game.getSelectedCell();
        if (selected.first >= 0 && selected.second >= 0) {
            if (e->code >= sf::Keyboard::Key::Num1 && e->code <= sf::Keyboard::Key::Num9) {
                int value = static_cast<int>(e->code) - static_cast<int>(sf::Keyboard::Key::Num0);
                game.setValue(selected.first, selected.second, value);
            } else if (e->code >= sf::Keyboard::Key::Numpad1 && 
                       e->code <= sf::Keyboard::Key::Numpad9) {
                int value = static_cast<int>(e->code) - static_cast<int>(sf::Keyboard::Key::Numpad0);
                game.setValue(selected.first, selected.second, value);
            } else if (e->code == sf::Keyboard::Key::Backspace || 
                       e->code == sf::Keyboard::Key::Delete) {
                game.clearCell(selected.first, selected.second);
            }
        }
        
        if (e->code == sf::Keyboard::Key::Escape) {
            game.pause();
        }
        
        if (e->code == sf::Keyboard::Key::N) {
            game.toggleNotesMode();
        }
    } else if (event.is<sf::Event::MouseMoved>()) {
        const auto* e = event.getIf<sf::Event::MouseMoved>();
        if (e) {
            updateButtonHover(sf::Vector2i(e->position.x, e->position.y));
        }
    }
}

void UI::update(const Game& game) {
    // Update button states
    notesButton_.active = game.isNotesMode();
    pauseButton_.active = game.isPaused();
    
    // Update button text
    pauseButton_.text = game.isPaused() ? "Resume" : "Pause";
    notesButton_.text = game.isNotesMode() ? "Notes: ON" : "Notes: OFF";
    
    // Update hints button
    std::ostringstream hintText;
    hintText << "Hint (" << game.getHintsRemaining() << ")";
    hintButton_.text = hintText.str();
    
    // Update buttons vector
    buttons_ = {newGameButton_, hintButton_, notesButton_, pauseButton_, difficultyButton_};
}

void UI::render(sf::RenderWindow& window, const Game& game) {
    window.clear(backgroundColor_);
    
    renderBoard(window, game);
    renderButtons(window);
    renderTimer(window, game);
    
    if (difficultyMenuOpen_) {
        renderDifficultyMenu(window);
    }
    
    if (game.isPaused() && !difficultyMenuOpen_) {
        renderPauseOverlay(window);
    }
    
    window.display();
}

void UI::renderBoard(sf::RenderWindow& window, const Game& game) {
    const Board& board = game.getBoard();
    auto selected = game.getSelectedCell();
    
    // Draw cell backgrounds and highlights
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            float x = boardOffsetX_ + c * cellSize_;
            float y = boardOffsetY_ + r * cellSize_;
            
            sf::RectangleShape cellRect(sf::Vector2f(cellSize_, cellSize_));
            cellRect.setPosition(sf::Vector2f(x, y));
            
            // Highlight row, column, and box if cell is selected
            bool isHighlighted = false;
            if (selected.first == r || selected.second == c) {
                isHighlighted = true;
            } else if (selected.first >= 0 && selected.second >= 0) {
                int selectedBoxRow = selected.first / 3;
                int selectedBoxCol = selected.second / 3;
                int cellBoxRow = r / 3;
                int cellBoxCol = c / 3;
                if (selectedBoxRow == cellBoxRow && selectedBoxCol == cellBoxCol) {
                    isHighlighted = true;
                }
            }
            
            if (selected.first == r && selected.second == c) {
                cellRect.setFillColor(selectedCellColor_);
            } else if (isHighlighted) {
                cellRect.setFillColor(highlightColor_);
            } else {
                cellRect.setFillColor(cellColor_);
            }
            
            window.draw(cellRect);
            
            // Render cell content
            renderCell(window, r, c, board.getCell(r, c), game);
        }
    }
    
    // Draw grid lines
    for (int i = 0; i <= 9; ++i) {
        float pos = boardOffsetX_ + i * cellSize_;
        
        // Vertical lines
        sf::RectangleShape line(sf::Vector2f(1.0f, boardSize_));
        line.setPosition(sf::Vector2f(pos, boardOffsetY_));
        line.setFillColor((i % 3 == 0) ? thickGridColor_ : gridColor_);
        window.draw(line);
        
        // Horizontal lines
        pos = boardOffsetY_ + i * cellSize_;
        line.setSize(sf::Vector2f(boardSize_, 1.0f));
        line.setPosition(sf::Vector2f(boardOffsetX_, pos));
        window.draw(line);
    }
}

void UI::renderCell(sf::RenderWindow& window, int row, int col, const Cell& cell, 
                    const Game& game) {
    float x = boardOffsetX_ + col * cellSize_ + cellSize_ / 2.0f;
    float y = boardOffsetY_ + row * cellSize_ + cellSize_ / 2.0f;
    
    if (!cell.isEmpty()) {
        // Draw number
        std::string valueStr = std::to_string(cell.getValue());
        sf::Text text = createText(valueStr, 32, 
                                   cell.hasError() ? errorColor_ : 
                                   (cell.isGiven() ? givenTextColor_ : userTextColor_),
                                   x, y);
        text.setStyle(cell.isGiven() ? sf::Text::Bold : sf::Text::Regular);
        
        // Center text
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f,
                                   textBounds.position.y + textBounds.size.y / 2.0f));
        
        window.draw(text);
    } else if (!cell.getNotes().empty()) {
        // Draw notes
        renderNotes(window, row, col, cell);
    }
}

void UI::renderNotes(sf::RenderWindow& window, int row, int col, const Cell& cell) {
    float cellX = boardOffsetX_ + col * cellSize_;
    float cellY = boardOffsetY_ + row * cellSize_;
    float noteSize = cellSize_ / 3.0f;
    float fontSize = 14.0f;
    
    for (int note : cell.getNotes()) {
        int noteRow = (note - 1) / 3;
        int noteCol = (note - 1) % 3;
        
        float x = cellX + noteCol * noteSize + noteSize / 2.0f;
        float y = cellY + noteRow * noteSize + noteSize / 2.0f;
        
        std::string noteStr = std::to_string(note);
        sf::Text text = createText(noteStr, fontSize, noteTextColor_, x, y);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f,
                                   textBounds.position.y + textBounds.size.y / 2.0f));
        
        window.draw(text);
    }
}

void UI::renderButtons(sf::RenderWindow& window) {
    for (const auto& button : buttons_) {
        sf::RectangleShape rect(button.bounds.size);
        rect.setPosition(button.bounds.position);
        rect.setFillColor(button.active ? buttonHoverColor_ : 
                         (button.hovered ? buttonHoverColor_ : buttonColor_));
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(1.0f);
        window.draw(rect);
        
        sf::Text text = createText(button.text, 16, buttonTextColor_,
                                   button.bounds.position.x + button.bounds.size.x / 2.0f,
                                   button.bounds.position.y + button.bounds.size.y / 2.0f);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f,
                                   textBounds.position.y + textBounds.size.y / 2.0f));
        window.draw(text);
    }
}

void UI::renderTimer(sf::RenderWindow& window, const Game& game) {
    float timerX = boardOffsetX_;
    float timerY = boardOffsetY_ + boardSize_ + 20.0f;
    
    std::string timerStr = "Time: " + game.getTimerString();
    sf::Text text = createText(timerStr, 20, sf::Color::Black, timerX, timerY);
    window.draw(text);
}

void UI::renderDifficultyMenu(sf::RenderWindow& window) {
    // Semi-transparent overlay
    sf::RectangleShape overlay(sf::Vector2f(windowWidth_, windowHeight_));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Menu background
    float menuWidth = 300.0f;
    float menuHeight = 200.0f;
    float menuX = windowWidth_ / 2.0f - menuWidth / 2.0f;
    float menuY = windowHeight_ / 2.0f - menuHeight / 2.0f;
    
    sf::RectangleShape menuRect(sf::Vector2f(menuWidth, menuHeight));
    menuRect.setPosition(sf::Vector2f(menuX, menuY));
    menuRect.setFillColor(sf::Color(255, 255, 255));
    menuRect.setOutlineColor(sf::Color::Black);
    menuRect.setOutlineThickness(2.0f);
    window.draw(menuRect);
    
    // Title
    sf::Text title = createText("Select Difficulty", 24, sf::Color::Black,
                                menuX + menuWidth / 2.0f, menuY + 20.0f);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(sf::Vector2f(titleBounds.position.x + titleBounds.size.x / 2.0f,
                                titleBounds.position.y + titleBounds.size.y / 2.0f));
    window.draw(title);
    
    // Difficulty options
    std::vector<std::string> difficulties = {"Easy", "Medium", "Hard", "Expert"};
    float itemHeight = 35.0f;
    float startY = menuY + 60.0f;
    
    for (int i = 0; i < 4; ++i) {
        float itemY = startY + i * itemHeight;
        sf::RectangleShape itemRect(sf::Vector2f(menuWidth - 20.0f, itemHeight - 5.0f));
        itemRect.setPosition(sf::Vector2f(menuX + 10.0f, itemY));
        
        if (static_cast<int>(selectedDifficulty_) == i) {
            itemRect.setFillColor(buttonColor_);
        } else {
            itemRect.setFillColor(sf::Color(240, 240, 240));
        }
        itemRect.setOutlineColor(sf::Color::Black);
        itemRect.setOutlineThickness(1.0f);
        window.draw(itemRect);
        
        sf::Text itemText = createText(difficulties[i], 18,
                                       static_cast<int>(selectedDifficulty_) == i ? 
                                       buttonTextColor_ : sf::Color::Black,
                                       menuX + menuWidth / 2.0f, itemY + itemHeight / 2.0f);
        sf::FloatRect itemBounds = itemText.getLocalBounds();
        itemText.setOrigin(sf::Vector2f(itemBounds.position.x + itemBounds.size.x / 2.0f,
                                       itemBounds.position.y + itemBounds.size.y / 2.0f));
        window.draw(itemText);
    }
}

void UI::renderPauseOverlay(sf::RenderWindow& window) {
    // Semi-transparent overlay
    sf::RectangleShape overlay(sf::Vector2f(windowWidth_, windowHeight_));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Pause text
    sf::Text pauseText = createText("PAUSED", 48, sf::Color::White,
                                    windowWidth_ / 2.0f, windowHeight_ / 2.0f);
    sf::FloatRect textBounds = pauseText.getLocalBounds();
    pauseText.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f,
                                    textBounds.position.y + textBounds.size.y / 2.0f));
    window.draw(pauseText);
    
    sf::Text hintText = createText("Press ESC or click to resume", 20, sf::Color::White,
                                   windowWidth_ / 2.0f, windowHeight_ / 2.0f + 60.0f);
    sf::FloatRect hintBounds = hintText.getLocalBounds();
    hintText.setOrigin(sf::Vector2f(hintBounds.position.x + hintBounds.size.x / 2.0f,
                                   hintBounds.position.y + hintBounds.size.y / 2.0f));
    window.draw(hintText);
}

bool UI::isDifficultyMenuOpen() const {
    return difficultyMenuOpen_;
}

void UI::setDifficultyMenuOpen(bool open) {
    difficultyMenuOpen_ = open;
}

Difficulty UI::getSelectedDifficulty() const {
    return selectedDifficulty_;
}

std::pair<int, int> UI::getCellFromMouse(int mouseX, int mouseY) const {
    float x = static_cast<float>(mouseX) - boardOffsetX_;
    float y = static_cast<float>(mouseY) - boardOffsetY_;
    
    if (x < 0 || x >= boardSize_ || y < 0 || y >= boardSize_) {
        return {-1, -1};
    }
    
    int col = static_cast<int>(x / cellSize_);
    int row = static_cast<int>(y / cellSize_);
    
    return {row, col};
}

bool UI::isPointInButton(const sf::Vector2f& point, const Button& button) const {
    return button.bounds.contains(point);
}

void UI::updateButtonHover(const sf::Vector2i& mousePos) {
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    
    newGameButton_.hovered = isPointInButton(mousePosF, newGameButton_);
    hintButton_.hovered = isPointInButton(mousePosF, hintButton_);
    notesButton_.hovered = isPointInButton(mousePosF, notesButton_);
    pauseButton_.hovered = isPointInButton(mousePosF, pauseButton_);
    difficultyButton_.hovered = isPointInButton(mousePosF, difficultyButton_);
}

sf::Text UI::createText(const std::string& str, unsigned int size, 
                       const sf::Color& color, float x, float y) const {
    // SFML 3 requires font in constructor
    if (fontLoaded_) {
        sf::Text text(font_, str, size);
        text.setFillColor(color);
        text.setPosition(sf::Vector2f(x, y));
        return text;
    } else {
        // Fallback: create with a default-constructed font (may not work)
        static sf::Font defaultFont;
        sf::Text text(defaultFont, str, size);
        text.setFillColor(color);
        text.setPosition(sf::Vector2f(x, y));
        return text;
    }
}

