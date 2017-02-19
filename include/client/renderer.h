#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "client/context.h"
#include "common/player.h"
#include "common/terrain.h"
#include "common/block.h"
//#include "common/lazyworld.h"

class Context;

class Renderer {
    public:
        Renderer(Context *context);
        void InitGraphics();
        void render();
        void renderBlock(PositionedBlock *block);
        void drawCube(Block *block, float x, float y, float z, float length);

        Context *context;

        sf::Font Font;
};

#endif
