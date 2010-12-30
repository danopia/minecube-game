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
    
    App.SetFramerateLimit(60);

    // Create a clock for measuring time elapsed
    sf::Clock Clock;

    const float Speed = 5.f;
    float Left = 0.f;
    float Top  = 0.f;

    // Start game loop
    while (App.IsOpened())
    {
        float ElapsedTime = Clock.GetElapsedTime();
        Clock.Reset();
        
        if (App.GetInput().IsKeyDown(sf::Key::Left))  Left -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Right)) Left += Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Up))    Top  -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Down))  Top  += Speed * ElapsedTime;
        
        input_handler.handleEvents();

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();

        renderer.render(Left, Top);

        // Finally, display rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}

