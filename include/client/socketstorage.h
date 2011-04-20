#pragma once
#ifndef SOCKETSTORAGE_H
#define SOCKETSTORAGE_H

#include "common/worldstorage.h"
#include "client/context.h"

#include <SFML/Network.hpp>

class SocketStorage : public WorldStorage
{
    public:
        SocketStorage(Terrain *Callback, Context *context) : WorldStorage(Callback), context(context) {};
        
        Chunk *RequestChunk(Vector3 Index);
        void PlaceBlock(char type, Vector3 chunkIndex, Vector3 blockIndex);
        
        Chunk *ReadChunk(sf::Packet &Packet);
        
    private:
        //void SendRequest(Vector3 Index);
        
        Context *context;
};

#endif

