#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "common/chunk.h"
#include "common/vector3.h"
#include "common/block.h"
#include "common/player.h"
#include "common/ray.h"
#include "common/worldstorage.h"
#include "common/positionedblock.h"

#include <SFML/System.hpp>
//#include <vector>
#include <list>
#include <algorithm>
#include <map>
//#include <iostream>
//#include <fstream>

class WorldStorage;

class Terrain
{
    public:
        Terrain(WorldStorage *Storage) : Storage(Storage), ChunkSize(16) {};

        int ChunkSize; // Side length of each chunk, in blocks

        WorldStorage *Storage;
        std::map<Vector3, Chunk*> LoadedChunks;
        std::list<PositionedBlock*> VisibleBlocks;
        std::list<Entity> Entities;
        
        void DoStep();
        void CheckCollision(Entity *entity);
        PositionedBlock *CheckAim(Player *player);
        void DestroyTarget(Player *player);
        void PlaceAboveTarget(Player *player);
        
        void DestroyBlock(PositionedBlock *block);
        void PlaceBlock(char type, Vector3 chunkIndex, Vector3 blockIndex);
        
        std::vector<Vector3> RequestedChunks;
        
        Chunk *GetChunk(Vector3 index);
        void LoadChunk(Chunk *chunk);
        void HandleRequests(Vector3 Pos);
        void RequestChunk(Vector3 index);

    private:
};

#endif
