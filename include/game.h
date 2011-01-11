#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <cstdio>

#include "renderer.h"
#include "input.h"
#include "octree.h"
#include "player.h"
#include "terrain.h"

#ifdef _WIN32
#define snprintf sprintf_s
#endif

class Game
{
    public:
        Game(sf::RenderWindow *app);
        void Loop();

        sf::RenderWindow *app;
        Player player;
        Terrain terrain;
        Renderer renderer;
        InputHandler inputHandler;
        
        bool Running;
};

#endif
