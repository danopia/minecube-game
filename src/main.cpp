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
    sf::Window App(sf::VideoMode(800, 600, 32), "SFML OpenGL");
    
    // Create a renderer and input handler
    Renderer renderer;
    InputHandler input_handler(&App);
    
//    App.UseVerticalSync(true);

    // Create a clock for measuring time elapsed
    sf::Clock Clock;

    const float Speed = 5.f;
    float Left = 0.f;
    float Top  = 0.f;
    float Up  = 0.f;

    Octree<bool> terrain = makeTerrain(0);
    
    // Start game loop
    while (App.IsOpened())
    {
        float ElapsedTime = Clock.GetElapsedTime();
        Clock.Reset();
        
        if (App.GetInput().IsKeyDown(sf::Key::A)) Left -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::D)) Left += Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::W)) Top  -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::S)) Top  += Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Q)) Up   -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::E)) Up   += Speed * ElapsedTime;
        
        input_handler.handleEvents();

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();

        renderer.render(Left, Top, Up);

        // Finally, display rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}

