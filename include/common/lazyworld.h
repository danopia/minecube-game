#pragma once
#ifndef LAZYWORLD_H
#define LAZYWORLD_H

#include <SFML/Network.hpp>
#include <vector>
#include <list>
#include <algorithm>

#include "common/positionedblock.h"
#include "common/entity.h"
#include "common/vector3.h"
#include "common/packets.h"
#include "common/block.h"
#include "common/ray.h"

class LazyWorld
{
    public:
        LazyWorld(sf::SocketTCP& Socket) : Socket(Socket) {};
        
        sf::SocketTCP& Socket;
        std::list<PositionedBlock*> Blocks;
        std::list<PositionedBlock*> Blocks2;
        std::list<Entity> Entities;
        
        void DoStep();
        void CheckCollision(Entity *entity);
        PositionedBlock *CheckAim(Player *player);
        void DestroyTarget(Player *player);
        
        int ChunkSize;
        std::vector<Vector3> RequestedChunks;
        
        void LoadChunk(sf::Packet Packet);
        void HandleRequests(Vector3 Pos);
        void RequestChunk(Vector3 ChunkIndex);
};

#endif

