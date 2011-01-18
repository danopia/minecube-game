#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Network.hpp>
#include <cmath>
#include <iostream>
#include <cstdio>

#include "client/renderer.h"
#include "client/input.h"
#include "client/socket.h"
#include "common/octree.h"
#include "common/player.h"
#include "common/terrain.h"
#include "common/lazyworld.h"
#include "common/packets.h"

#ifdef _WIN32
#define snprintf sprintf_s
#endif

class Game
{
    public:
        Game(sf::RenderWindow *app, Socket *socket);
        void Loop();

        sf::RenderWindow *app;
        Socket *socket;
        Player player;
        LazyWorld world;
        Renderer renderer;
        InputHandler inputHandler;
        
        bool Running;
};

#endif
