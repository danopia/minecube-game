#include "input.h"

InputHandler::InputHandler(sf::Window* Window) : app(Window) {
}

void InputHandler::handleEvent(sf::Event Event) {
/*    // Close window : exit
    if (Event.Type == sf::Event::Closed)
        SDL_Exit(1)

    // Escape key : exit
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
        app->Close();

    // Resize event : adjust viewport
    if (Event.Type == sf::Event::Resized)
        glViewport(0, 0, Event.Size.Width, Event.Size.Height);*/
}

void InputHandler::handleEvents() {
}

