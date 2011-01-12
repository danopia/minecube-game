#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <cstdio>

#include "client/renderer.h"
#include "client/input.h"
#include "common/octree.h"
#include "common/player.h"
#include "common/terrain.h"

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
        std::string file;
        
        void Generate();
        void Load(std::string filename);
        void Save();
        void Save(std::string filename);
};

#endif
