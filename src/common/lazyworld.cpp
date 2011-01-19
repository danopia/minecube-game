#include "common/lazyworld.h"
#include <cmath>

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
        
        //if (type == 3)
            Block = new GrassBlock();
        //else
        //    Block = new AirBlock();
        
        Blocks.push_back(PositionedBlock(Block, Pos, SideLength));
    }
}

void LazyWorld::HandleRequests(Vector3 Pos) {
    Vector3 CurrentChunk;
    
    CurrentChunk.X = floor(Pos.X / ChunkSize);
    CurrentChunk.Y = floor(Pos.Y / ChunkSize);
    CurrentChunk.Z = floor(Pos.Z / ChunkSize);
    
    if (!contains(LoadedChunks, CurrentChunk))
        RequestChunk(CurrentChunk);
}

void LazyWorld::RequestChunk(Vector3 ChunkIndex) {
    // Send request to the server
    sf::Packet Packet;
    Packet << "Terrain pl0z" << ChunkIndex;
    Socket.Send(Packet);
    
    LoadedChunks.push_back(ChunkIndex);
}
