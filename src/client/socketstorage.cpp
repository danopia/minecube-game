#include "client/socketstorage.h"

// Terrain &Callback;
// std::map<Vector3, Chunk*> Loaded;

Chunk *SocketStorage::RequestChunk(Vector3 Index) {
    sf::Packet Packet;
    Packet << (sf::Uint8) 4 << Index;
    context->socket->socket.Send(Packet);
    
    return NULL;
}
#include <cstdio>

// TODO: goes elsewhere?
Block *MakeBlock2(char type) {
    if (type == 3)
        return new GrassBlock();
    else
        return new AirBlock();
}

void SocketStorage::PlaceBlock(char type, Vector3 chunkIndex, Vector3 blockIndex) {
    Chunk *chunk = Loaded[chunkIndex];
    if (chunk == NULL) return; // TODO: this *will* segfault on unloaded chunks!
    
    Vector3 absolute = chunk->GetWorldPos(blockIndex);
    Block *block = chunk->GetBlock(blockIndex);
    chunk->PlaceBlock(type, blockIndex);
    
    if (block->Type != type)
        context->socket->SendBlock(type, chunkIndex, blockIndex);
}

Chunk *SocketStorage::ReadChunk(sf::Packet &Packet) {
    int BlockCount;
    Vector3 ChunkIndex;
    Packet >> ChunkIndex >> BlockCount;
    
    Chunk *chunk = new Chunk(ChunkIndex, 16);
    
    sf::Uint8 type;
    Block *block;
    Vector3 Pos;
    float brb;
    
    for (int i = 0; i < BlockCount; i++) {
        Packet >> type >> Pos;
        
        chunk->Blocks[Vector3(Pos)] = MakeBlock2(type);
    }
    
    Loaded[ChunkIndex] = chunk;
    Callback->LoadChunk(chunk);
    
    return chunk;
}

