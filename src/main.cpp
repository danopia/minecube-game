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


    // Create a renderer
    std::cout << "Started to generate terrain" << std::endl;
    Renderer renderer(Terrain(5, 0, 2,1,1, 50));
    std::cout << "Finished!" << std::endl;
    
    //App.UseVerticalSync(true);
    
    Player player(5.f, 0.f, 180.f, 5.f, 70.f, 30.f, "Foo");
 //   InputHandler input(&player, SDL_GetTicks());
    
    SDL_ShowCursor(SDL_DISABLE);
//    App.PreserveOpenGLStates(true);

    char buf[10];
    int lastTick = SDL_GetTicks();
    std::cout << "HI" << std::endl;
    int frameCount = 0;
    // Start game loop
    while (true)
    {
        float fps = (SDL_GetTicks() - lastTick) / 1000.0;
        lastTick = SDL_GetTicks();
        std::cout << "FPS: " << fps << std::endl;
        
        // Rotate view based on mouse movement 
       /* float mouseDeltaX = Input.GetMouseX() - 100; 
        float mouseDeltaY = Input.GetMouseY() - 100;
        
        if (!(mouseDeltaX == -100 && mouseDeltaY == -100) && !(mouseDeltaX == 0 && mouseDeltaY == 0)) 
            player.ChangeRotation((mouseDeltaY/10), (mouseDeltaX/10));*/

//        input.handleEvents();
        renderer.render(player);
        
        // Finally, display rendered frame on screen
        SDL_GL_SwapBuffers();
        
    }

    return EXIT_SUCCESS;
}

