#include <SFML/Window.hpp>
#include <renderer.h>
#include <input.h>

int main()
{
    // Create the main window
    sf::Window App(sf::VideoMode(800, 600, 32), "SFML OpenGL");
    
    // Create a renderer and input handler
    Renderer renderer;
    InputHandler input_handler(&App);

    // Create a clock for measuring time elapsed
    sf::Clock Clock;

    // Start game loop
    while (App.IsOpened())
    {
        input_handler.handleEvents();

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();

        renderer.render(Clock);

        // Finally, display rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}

