#include "common/lazyworld.h"
#include <cmath>
#include <stdio.h>

bool contains(std::vector<Vector3> vector, Vector3 value) {
    for (int i = 0; i < vector.size(); i++)
        if ((vector[i].X == value.X) && (vector[i].Y == value.Y) && (vector[i].Z == value.Z))
            return true;
    
    return false;
}
  
void LazyWorld::LoadChunk(sf::Packet Packet) {
    int BlockCount;
    Packet >> BlockCount;
    
    char type;
    Block *Block;
    Vector3 Pos;
    float SideLength;
    
    for (int i = 0; i < BlockCount; i++) {
        //Packet >> type;
        Packet >> Pos;
        Packet >> SideLength;
        
        if (type == 3)
            Block = new GrassBlock();
        else
            Block = new AirBlock();
        
        Blocks.push_back(PositionedBlock(Block, Pos, SideLength));
    }
}

void LazyWorld::HandleRequests(Vector3 Pos) {
    Vector3 CurrentChunk;
    
    printf("%f\n", ChunkSize);
    printf("%f\n", Pos.X / ChunkSize);
    CurrentChunk.X = floor(Pos.X / ChunkSize);
    CurrentChunk.Y = floor(Pos.Y / ChunkSize);
    CurrentChunk.Z = floor(Pos.Z / ChunkSize);
    
    if (!contains(LoadedChunks, CurrentChunk))
        RequestChunk(CurrentChunk);
}

void LazyWorld::RequestChunk(Vector3 ChunkIndex) {
    // Send it to the server
    sf::Packet Packet;
    printf("(%f,%f,%f)\n", ChunkIndex.X, ChunkIndex.Y, ChunkIndex.Z);
    Packet << "Terrain pl0z" << ChunkIndex;
    Socket->Send(Packet);
    
    // Extract the terrain and save it
    Socket->Receive(Packet);
    std::string command;
    Packet >> command;
    if (command == "Take this chunk. It will be useful in times of need.") {
        LoadChunk(Packet);
        LoadedChunks.push_back(ChunkIndex);
    }
}
