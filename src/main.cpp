#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <cstdio>

#include "renderer.h"
#include "input.h"
#include "octree.h"
#include "player.h"
#include "terrain.h"

#ifdef _WIN32 
#define snprintf sprintf_s 
#endif 

int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "MineCube");
    
    //App.UseVerticalSync(true);
    
    // Create some instances
    Player player(5.f, Vector3(0.f, 0.f, 90.f), Vector3(5.f, 25.f, 60.f), "Foo");
    Terrain world (5, 0, 1,1,1, 50);
    Renderer renderer(world, &player);
    InputHandler input_handler(&App, &player, &renderer);
    
    // Set some stuff
    App.PreserveOpenGLStates(true);

    char buf[10];
    float Framerate;
    
    sf::String Text;
    Text.SetFont(sf::Font::GetDefaultFont());

    // Start game loop
    while (App.IsOpened())
    {
        Framerate = 1.f / App.GetFrameTime();
        
        // Handle mouse and keyboard stuff
        input_handler.handleEvents();

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        //App.SetActive();

        // Draw entire scene
        renderer.render();
        
        // Draw FPS
        snprintf(buf, 10, "%.1f FPS", Framerate);
        Text.SetText(buf);
        App.Draw(Text);

        // Finally, display rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}

