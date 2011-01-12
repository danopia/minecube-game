#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "common/player.h"
#include "common/terrain.h"
#include "common/block.h"

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
