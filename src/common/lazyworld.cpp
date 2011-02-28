#include "common/lazyworld.h"

void LazyWorld::DoStep() {
    // Collision check against player
}

void LazyWorld::CheckCollision(Entity *entity) {
    PositionedBlock *block;
    
    for (int i = 0; i < Blocks.size(); i++) {
        block = Blocks[i];
        if (entity->Pos.X + entity->Hitbox.X >= block->pos.X && entity->Pos.X <= block->pos.X + block->sideLength
         && entity->Pos.Y + entity->Hitbox.Y >= block->pos.Y && entity->Pos.Y <= block->pos.Y + block->sideLength
         && entity->Pos.Z + entity->Hitbox.Z >= block->pos.Z && entity->Pos.Z <= block->pos.Z + block->sideLength)
            entity->Collide(*block);
    }
}

void LazyWorld::CheckAim(Player *player) {
    PositionedBlock *block;
    Ray ray = Ray(player);
    
    for (int i = 0; i < Blocks.size(); i++) {
        block = Blocks[i];
        
        if (ray.CheckCollision(block))
            block->marked = true;
    }
}

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
        
        Blocks.push_back(new PositionedBlock(Block, Pos, SideLength));
    }
}

void LazyWorld::HandleRequests(Vector3 Pos) {
    Vector3 CurrentChunk;
    
    CurrentChunk.X = floor(Pos.X / ChunkSize);
    CurrentChunk.Y = floor(Pos.Y / ChunkSize);
    CurrentChunk.Z = floor(Pos.Z / ChunkSize);
    
    if (!contains(RequestedChunks, CurrentChunk))
        RequestChunk(CurrentChunk);
}

void LazyWorld::RequestChunk(Vector3 ChunkIndex) {
    // Send request to the server
    sf::Packet Packet;
    Packet << "Terrain pl0z" << ChunkIndex;
    Socket.Send(Packet);
    
    RequestedChunks.push_back(ChunkIndex);
}
