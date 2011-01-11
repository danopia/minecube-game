#ifndef _RENDERER_H_
    #define _RENDERER_H_
#include <SFML/Graphics.hpp>
#include "player.h"
#include "terrain.h"
#include "block.h"

class Renderer {
    public:
        Renderer(Terrain *initterrain, Player *player);
        void InitGraphics();
        void render();
        void renderNode(Octree<Block*> terrain, float x, float y, float z, float size);
        void drawCube(Block *block, float x, float y, float z, float length);
        
        Terrain *terrain;
        Player *player;
        
        sf::Font Font;
};

#endif
