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
    vector<Octree<float> > testnodes(8, Octree<float>(5.2));
    vector<Octree<float> > testnodes2(8, Octree<float>(testnodes));
    Octree<float> foo(testnodes2);
    foo.Collapse();
    cout << foo.value << endl;
    // Create a renderer and input handler
    Renderer renderer(terrain);
    InputHandler input_handler(&App);
    
    //App.UseVerticalSync(true);

    // Create a clock for measuring time elapsed
    sf::Clock Clock;
    
    Player player(5.f, 0.f, 180.f, 5.f, 70.f, 30.f);
    
    App.ShowMouseCursor(false);
    
    // Start game loop
    while (App.IsOpened())
    {
        const sf::Input& Input = App->GetInput(); 
        
        float ElapsedTime = Clock.GetElapsedTime();
        Clock.Reset();
        
        if ((Input.IsKeyDown(sf::Key::S))) player.Forward(-ElapsedTime);
        if ((Input.IsKeyDown(sf::Key::W))) player.Forward( ElapsedTime);
        if ((Input.IsKeyDown(sf::Key::D))) player.Strafe(-ElapsedTime);
        if ((Input.IsKeyDown(sf::Key::A))) player.Strafe( ElapsedTime);
        if ((Input.IsKeyDown(sf::Key::Z))) player.Speed++;
        if ((Input.IsKeyDown(sf::Key::X))) player.Speed--;
        
        if (Input.IsKeyDown(sf::Key::Space)) renderer.terrain = makeTerrain(0);
        
        // Rotate view based on mouse movement 
        float mouseDeltaX = Input.GetMouseX() - 100; 
        float mouseDeltaY = Input.GetMouseY() - 100;
        App.SetCursorPosition(100, 100);
        
        if (!(mouseDeltaX == -100 && mouseDeltaY == -100) && !(mouseDeltaX == 0 && mouseDeltaY == 0)) 
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

