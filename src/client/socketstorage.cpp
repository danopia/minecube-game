#include "client/socketstorage.h"

// Terrain &Callback;
// std::map<Vector3, Chunk*> Loaded;

void SocketStorage::RequestChunk(Vector3 Index) {
    sf::Packet Packet;
    Packet << (sf::Uint8) 4 << Index;
    context->socket->socket.Send(Packet);
}

// TODO: goes elsewhere?
Block *MakeBlock2(char type) {
    if (type == 3)
        return new GrassBlock();
    else
        return new AirBlock();
}

//void LoadChunk(Vector3 index, Chunk chunk);

Chunk SocketStorage::ReadChunk(sf::Packet &Packet) {
    int BlockCount;
    Vector3 ChunkIndex;
    Packet >> ChunkIndex >> BlockCount;
    
    Chunk chunk(ChunkIndex, 16);
    
    sf::Uint8 type;
    Block *block;
    Vector3 Pos;
    float brb;
    
    for (int i = 0; i < BlockCount; i++) {
        Packet >> type >> Pos;
        
        chunk.Blocks[Vector3(Pos)] = MakeBlock2(type);
    }
    
    Loaded[ChunkIndex] = &chunk;
    Callback->LoadChunk(chunk);
    
    return chunk;
}

