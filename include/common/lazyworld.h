#pragma once
#ifndef LAZYWORLD_H
#define LAZYWORLD_H

#include <SFML/Network.hpp>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include "common/block.h"
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
        std::list<PositionedBlock*> VisibleBlocks;
        std::list<Entity> Entities;
        
        void DoStep();
        void CheckCollision(Entity *entity);
        PositionedBlock *CheckAim(Player *player);
        void DestroyTarget(Player *player);
        
        void DestroyBlock(PositionedBlock *block);
        void PlaceBlock(char type, Vector3 chunkIndex, Vector3 blockIndex);
              
        int ChunkSize;
        std::vector<Vector3> RequestedChunks;
        std::map<Vector3, std::map<Vector3, Block*> > LoadedChunks;
        
        void LoadChunk(sf::Packet Packet);
        void HandleRequests(Vector3 Pos);
        void RequestChunk(Vector3 ChunkIndex);
};

#endif

