#ifndef _RENDERER_H_
    #define _RENDERER_H_
#include <SFML/Window.hpp>
#include "terrain.h"

class Renderer {
    public:
        Renderer(Terrain initterrain);
        void render(float Left, float Top, float Up, float rotation, float zRotation);
        
        Terrain terrain;
};

#endif
