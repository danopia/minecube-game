#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <SFML/Window.hpp>

#include "client/context.h"
#include "client/renderer.h"
#include "common/player.h"

class Context;

class InputHandler {
    public:
        InputHandler(Context *context);
        void handleEvent(sf::Event Event);
        void handleEvents();
        void toggleFullscreen();
        
        // Booleans for pre-keypress movement
        bool WasRunning, WasCrouching, WasCrawling;
        
        Context *context;
        sf::Clock Clock; // Track elapsed time for player movement
        sf::Clock MovementTimer; // Track elapsed time for crouching, crawling, running
        
        bool InChat;
};

#endif
