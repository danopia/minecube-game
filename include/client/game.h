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
#include "client/context.h"
#include "common/octree.h"
#include "common/player.h"
#include "common/lazyworld.h"
#include "common/packets.h"

class Context;

class Game
{
    public:
        Game(Context *context);
        void Loop();

        Context *context;
};

#endif
