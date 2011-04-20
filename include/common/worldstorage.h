#pragma once
#ifndef WORLDSTORAGE_H
#define WORLDSTORAGE_H

#include "common/vector3.h"
#include "common/block.h"
#include "common/chunk.h"
#include "common/terrain.h"

#include <map>

class Terrain;

class WorldStorage
{
    public:
        WorldStorage(Terrain *Callback) : Callback(Callback) {};

        virtual Chunk *RequestChunk(Vector3 Index);
        virtual void PlaceBlock(char type, Vector3 chunkIndex, Vector3 blockIndex);

        Terrain *Callback;
        
    protected:
        std::map<Vector3, Chunk*> Loaded;
};

#endif

