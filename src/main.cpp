#include <SFML/Window.hpp>
#include <renderer.h>
#include <input.h>
#include <octree.h>
#include <iostream>

using namespace std;

Octree<bool> makeTerrain(int level)
{
    cout << "At level " << level << endl;
    vector<Octree<bool> >blocks;
    for(int i = 0; i < 8; i++)
    {
        bool leaf = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
        if(level < 2)
        {

            if(leaf)
            {
                bool type = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
                blocks.push_back(Octree<bool>(type));
            } 
            else
            {
                cout << "Going deeper!" << endl;
                blocks.push_back(makeTerrain(level + 1));
            }
        }
        else if(level == 2)
        {
            bool type = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
            blocks.push_back(Octree<bool>(type));
        }
        cout << "Block " << i << "at level " << level << ": " << (leaf ? "leaf" : "node") << endl;
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
    
    App.UseVerticalSync(true);

    // Create a clock for measuring time elapsed
    sf::Clock Clock;

    const float Speed = 5.f;
    float Left = 5.f;
    float Top  = 20.f;
    float Up  = 5.f;
    
    float rotation = 0.f;
    float zRotation = 180.f;
    
    App.ShowMouseCursor(false);
    
    // Start game loop
    while (App.IsOpened())
    {
        float ElapsedTime = Clock.GetElapsedTime();
        Clock.Reset();
        
        if (App.GetInput().IsKeyDown(sf::Key::D)) Left -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::A)) Left += Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::W)) Top  -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::S)) Top  += Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Q)) Up   -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::E)) Up   += Speed * ElapsedTime;
        
        if (App.GetInput().IsKeyDown(sf::Key::Space)) renderer.terrain = makeTerrain(0);
        
        // Rotate view based on mouse movement 
        float mouseDeltaX = App.GetInput().GetMouseX() - 100; 
        float mouseDeltaY = App.GetInput().GetMouseY() - 100;
        App.SetCursorPosition(100, 100);
        if (!(mouseDeltaX == -100 && mouseDeltaY == -100)) {
            zRotation += (mouseDeltaX / 10); 
            rotation += (mouseDeltaY / 10); 
            //cout << "DeltaX: " << mouseDeltaX << " DeltaY: " << mouseDeltaY << endl; 

            // Z rotation normalisation - between 0 and 360 
            if (zRotation >= 360) 
            { 
                zRotation -= 360; 
            } 

            if (zRotation < 0) 
            { 
                zRotation += 360; 
            } 

            // X/Y rotation limits 
            if (rotation < -90) 
            { 
                rotation = -90; 
            } 
            if (rotation >= 90) 
            { 
                rotation = 90; 
            } 
        }
        
        input_handler.handleEvents();

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();

        renderer.render(Left, Top, Up, rotation, zRotation);

        // Finally, display rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}

