#ifndef _RENDERER_H_
    #define _RENDERER_H_
#include <SFML/Window.hpp>
#include "player.h"
#include "terrain.h"

class Renderer {
    public:
        Renderer(Terrain initterrain, Player* player);
        void render();
        void renderNode(Octree<bool> terrain, float x, float y, float z, float size);
        
        Terrain terrain;
        Player *player;
};

#endif
