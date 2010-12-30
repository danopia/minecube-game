#ifndef _INPUT_H_
    #define _INPUT_H_
#include <SFML/Window.hpp>

class InputHandler {
    public:
        InputHandler(sf::Window* Window);
        void handleEvent(sf::Event Event);
        void handleEvents();
        
        sf::Window *app;
};

#endif
