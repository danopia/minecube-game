#pragma once
#ifndef LAZYWORLD_H
#define LAZYWORLD_H

#include <SFML/Network.hpp>
#include <vector>

#include "common/positionedblock.h"
#include "common/entity.h"
#include "common/vector3.h"
#include "common/packets.h"
#include "common/block.h"

class LazyWorld
{
    public:
        LazyWorld(sf::SocketTCP *Socket) : Socket(Socket) {};
        
        sf::SocketTCP *Socket;
        std::vector<PositionedBlock> Blocks;
        std::vector<Entity> Entities;
        
        void LoadChunk(sf::Packet Packet);
};

#endif

