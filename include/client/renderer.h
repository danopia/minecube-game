#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "common/player.h"
#include "common/terrain.h"
#include "common/block.h"
#include "common/lazyworld.h"

class Renderer {
    public:
        Renderer(LazyWorld *world, Player *player);
        void InitGraphics();
        void render();
        void renderBlock(PositionedBlock block);
        void drawCube(Block *block, float x, float y, float z, float length);
        
        LazyWorld *world;
        Player *player;
        
        sf::Font Font;
};

#endif
