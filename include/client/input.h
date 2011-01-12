#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <SFML/Window.hpp>

#include "common/player.h"
#include "client/renderer.h"

class InputHandler {
    public:
        InputHandler(sf::Window* Window, Player* Player, Renderer* Renderer);
        void handleEvent(sf::Event Event);
        bool handleEvents();
        void toggleFullscreen();
        
        // Booleans for pre-keypress movement
        bool WasRunning, WasCrouching, WasCrawling;
        
        sf::Window *app;
        sf::Clock Clock; // Track elapsed time for player movement
        sf::Clock MovementTimer; // Track elapsed time for crouching, crawling, running
        Player *player;
        Renderer *renderer;
};

#endif
