#include <cmath>
#include <iostream>
#include <stdio.h>

#include "renderer.h"
#include "input.h"
#include "octree.h"
#include "player.h"
#include "terrain.h"
#include "blocks.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>



int main(int argc, char **argv)
{

    // Create the main window
    if(SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        printf("Failed to init SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Surface* screen = SDL_SetVideoMode(800, 600, 16, SDL_OPENGL);


    // Generate terrain

    // Create a renderer and input handler
    Renderer renderer(Terrain(5, 0, 2,1,1, 50));
    InputHandler input_handler();
    
    //App.UseVerticalSync(true);
    
    Player player(5.f, 0.f, 180.f, 5.f, 70.f, 30.f, "Foo");
    // Track elapsed time for player movement
    
    SDL_ShowCursor(SDL_DISABLE);
//    App.PreserveOpenGLStates(true);

    char buf[10];

    // Start game loop
    while (true)
    {
       /* const sf::Input& Input = App.GetInput();
        
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
        
        if (!(mouseDeltaX == -100 && mouseDeltaY == -100) && !(mouseDeltaX == 0 && mouseDeltaY == 0)) 
            player.ChangeRotation((mouseDeltaY/10), (mouseDeltaX/10));

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
*/
        renderer.render(player);
        

        // Finally, display rendered frame on screen
        SDL_GL_SwapBuffers();
    }

    return EXIT_SUCCESS;
}

