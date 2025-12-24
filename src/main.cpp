//main.cpp
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "UI.h"
#include <iostream>

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 700;
    
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)), 
                           "Sudoku Game", sf::Style::Close);
    window.setFramerateLimit(60);
    
    Game game;
    UI ui(windowWidth, windowHeight);
    
    if (!ui.initialize()) {
        std::cerr << "Warning: Could not load font. Text rendering may not work properly." << std::endl;
        std::cerr << "Please ensure assets/fonts/Roboto-Regular.ttf exists." << std::endl;
    }
    
    sf::Clock clock;
    
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            ui.handleEvent(*event, game);
        }
        
        // Update game timer
        game.updateTimer();
        
        // Update UI
        ui.update(game);
        
        // Render
        ui.render(window, game);
        
        // Check for completion
        if (game.isComplete()) {
            // Could show completion message here
            // For now, just continue
        }
    }
    
    return 0;
}

