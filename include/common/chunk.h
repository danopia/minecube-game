#pragma once
#ifndef CHUNK_H
#define CHUNK_H

#include "common/vector3.h"
#include "common/block.h"
#include "common/entity.h"

#include <map>
#include <list>

class Chunk
{
    public:
        //Chunk(Vector3 Offset, std::map<Vector3, Block*> Blocks) : Index(Index), Blocks(Blocks) {};
        Chunk() : Offset(0, 0, 0), SideLength(0) {};
        Chunk(Vector3 Offset, int SideLength) : Offset(Offset), SideLength(SideLength) {};

        Vector3 Offset;
        int SideLength;
        std::map<Vector3, Block*> Blocks;
        std::list<Entity> Entities;

        void FillWith(char type);
        void RecalcSides();
        
        Block *GetBlock(Vector3 blockIndex);
        Block *DestroyBlock(Vector3 blockIndex);
        Block *PlaceBlock(char type, Vector3 blockIndex);
        
        Vector3 GetWorldPos(Vector3 blockIndex);
        Vector3 GetChunkPos(Vector3 worldIndex);

    private:
};

#endif

