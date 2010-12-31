#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "renderer.h"
#include "input.h"
#include "octree.h"
#include "player.h"

using namespace std;

Octree<bool> makeTerrain(int level)
{
    vector<Octree<bool> >blocks;
    for(int i = 0; i < 8; i++)
    {
        bool leaf = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
        if(level < 5)
        {

            if(leaf)
            {
                bool type = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
                blocks.push_back(Octree<bool>(type));
            } 
            else
            {
                blocks.push_back(makeTerrain(level + 1));
            }
        }
        else if(level == 5)
        {
            bool type = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
            blocks.push_back(Octree<bool>(type));
        }
    }
    Octree<bool> terrain(blocks);
    return terrain;
}


int main()
{
    // Create the main window
    sf::Window App(sf::VideoMode(800, 600, 32), "MineCube");

    Octree<bool> terrain = makeTerrain(0);
    
    // Create a renderer and input handler
    Renderer renderer(terrain);
    InputHandler input_handler(&App);
    
    //App.UseVerticalSync(true);

    // Create a clock for measuring time elapsed
    sf::Clock Clock;
    
    Player player(5.f, 0.f, 180.f, 5.f, 20.f, 5.f);
    
    App.ShowMouseCursor(false);
    
    // Start game loop
    while (App.IsOpened())
    {
        float ElapsedTime = Clock.GetElapsedTime();
        Clock.Reset();
        
        if ((App.GetInput().IsKeyDown(sf::Key::S)))    // W = forwards 
        { 
            player.Forward(-ElapsedTime);
        } 

        if ((App.GetInput().IsKeyDown(sf::Key::W)))    // S = backwards 
        { 
            player.Forward(ElapsedTime);
        }

        if ((App.GetInput().IsKeyDown(sf::Key::D)))    //A = strafe left 
        { 
            player.Strafe(-ElapsedTime);
        } 

        if ((App.GetInput().IsKeyDown(sf::Key::A)))    //D = strafe right 
        { 
            player.Strafe(ElapsedTime);
        } 
        
        if (App.GetInput().IsKeyDown(sf::Key::Space)) renderer.terrain = makeTerrain(0);
        
        // Rotate view based on mouse movement 
        float mouseDeltaX = App.GetInput().GetMouseX() - 100; 
        float mouseDeltaY = App.GetInput().GetMouseY() - 100;
        App.SetCursorPosition(100, 100);
        if (!(mouseDeltaX == -100 && mouseDeltaY == -100)) 
            player.ChangeRotation((mouseDeltaY/10), (mouseDeltaX/10));
        
        input_handler.handleEvents();

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();

        renderer.render(player.X, player.Y, player.Z, player.Yrot, player.Zrot);

        // Finally, display rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}

