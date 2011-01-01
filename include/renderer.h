#ifndef _RENDERER_H_
    #define _RENDERER_H_
#include <SFML/Window.hpp>
#include "player.h"
#include "terrain.h"

class Renderer {
    public:
        Renderer(Terrain initterrain);
        void render(Player player);
        
        Terrain terrain;
};

#endif
