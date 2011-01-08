#ifndef _INPUT_H_
    #define _INPUT_H_
#include <SFML/Window.hpp>

#include "player.h"
#include "renderer.h"

class InputHandler {
    public:
        InputHandler(sf::Window* Window, Player* Player, Renderer* Renderer);
        void handleEvent(sf::Event Event);
        void handleEvents();
        
        sf::Window *app;
        sf::Clock Clock; // Track elapsed time for player movement
        Player *player;
        Renderer *renderer;
};

#endif
