#pragma once
#ifndef SOCKET_H
#define SOCKET_H

#include <SFML/Network.hpp>
#include <string>

#include "client/context.h"
#include "common/lazyworld.h"
#include "common/player.h"
#include "common/entity.h"
#include "common/positionedblock.h"
#include "common/packets.h"
#include "common/vector3.h"

class Context;

class Socket
{
    public:
        Socket(Context *context, int port, std::string hostname);
        Socket(Context *context, sf::SocketTCP socket);
        
        void DoStep();
        void Close();
        
        Context *context;
        sf::SocketTCP socket;
        
        sf::Clock updateTimer;
        
        int Number;
        bool Connected;
};

#endif

