#pragma once
#ifndef SOCKET_H
#define SOCKET_H

#include <SFML/Network.hpp>
#include <string>
#include <map>

#include "client/context.h"
#include "client/socketstorage.h"
#include "common/terrain.h"
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
        void SendChat(const std::string Line);
        void SendBlock(sf::Uint8 type, Vector3 chunkIndex, Vector3 blockIndex);
        
        Context *context;
        sf::SocketTCP socket;
        
        sf::Clock Clock; // Track elapsed time for player movement
        sf::Clock updateTimer;
        
        int Number;
        bool Connected;
        std::map<int, Player*> Players;
};

#endif

