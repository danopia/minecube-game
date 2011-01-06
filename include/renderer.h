#ifndef _RENDERER_H_
    #define _RENDERER_H_
#include <SFML/Window.hpp>
#include "player.h"
#include "terrain.h"
#include "block.h"

class Renderer {
    public:
        Renderer(Terrain initterrain, Player* player);
        void render();
        void renderNode(Octree<Block*> terrain, float x, float y, float z, float size);
        void drawCube(float x, float y, float z, float length);
        
        Terrain terrain;
        Player *player;
};

#endif
