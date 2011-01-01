#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <stdio.h>

#include "renderer.h"
#include "input.h"
#include "octree.h"
#include "player.h"
#include "terrain.h"

using namespace std;




int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "MineCube");

    // Generate terrain

    // Create a renderer and input handler
    Renderer renderer(Terrain(5, 0));
    InputHandler input_handler(&App);
    
    //App.UseVerticalSync(true);
    
    Player player(5.f, 0.f, 180.f, 5.f, 70.f, 30.f);
    
    // Track elapsed time for player movement
    sf::Clock Clock;
    
    App.ShowMouseCursor(false);
    App.PreserveOpenGLStates(true);

    char buf[10];

    // Start game loop
    while (App.IsOpened())
    {
        const sf::Input& Input = App.GetInput();
        
        float Framerate = 1.f / App.GetFrameTime();
        
        float ElapsedTime = Clock.GetElapsedTime();
        Clock.Reset();
        
        if ((Input.IsKeyDown(sf::Key::S))) player.Forward(-ElapsedTime);
        if ((Input.IsKeyDown(sf::Key::W))) player.Forward( ElapsedTime);
        if ((Input.IsKeyDown(sf::Key::D))) player.Strafe(-ElapsedTime);
        if ((Input.IsKeyDown(sf::Key::A))) player.Strafe( ElapsedTime);
        if ((Input.IsKeyDown(sf::Key::Z))) player.Speed++;
        if ((Input.IsKeyDown(sf::Key::X))) player.Speed--;
        
        if (Input.IsKeyDown(sf::Key::Space)) renderer.terrain.Regenerate();
        
        input_handler.handleEvents();
        
        // Rotate view based on mouse movement 
        float mouseDeltaX = Input.GetMouseX() - 100; 
        float mouseDeltaY = Input.GetMouseY() - 100;
        App.SetCursorPosition(100, 100);
        
        if (!(mouseDeltaX == -100 && mouseDeltaY == -100) && !(mouseDeltaX == 0 && mouseDeltaY == 0)) 
            player.ChangeRotation((mouseDeltaY/10), (mouseDeltaX/10));

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();

        renderer.render(player);
        
        snprintf(buf, 10, "%.1f FPS", Framerate);
        sf::String Text;
        Text.SetText(buf);
        Text.SetFont(sf::Font::GetDefaultFont());
        App.Draw(Text);

        // Finally, display rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}

