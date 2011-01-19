#pragma once
#ifndef SOCKET_H
#define SOCKET_H

#include <SFML/Network.hpp>
#include <string>

#include "common/lazyworld.h"
#include "common/player.h"
#include "common/entity.h"
#include "common/positionedblock.h"
#include "common/packets.h"
#include "common/vector3.h"

class Socket
{
    public:
        Socket(sf::SocketTCP *socket);
        
        void DoStep();
        
        Player *player;
        LazyWorld *world;
        sf::SocketTCP *socket;
        
        sf::Clock updateTimer;
        
        int Number;
};

#endif

