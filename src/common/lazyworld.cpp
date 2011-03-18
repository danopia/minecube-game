#include "common/lazyworld.h"

void LazyWorld::DoStep() {
    // Collision check against player
}

void LazyWorld::CheckCollision(Entity *entity) {
    PositionedBlock *block;
    
    //for (int i = 0; i < Blocks.size(); i++) {
    for (std::list<PositionedBlock*>::iterator it = Blocks.begin(); it != Blocks.end(); ++it) {
        block = *it; // Blocks[i];
        if (entity->Pos.X + entity->Hitbox.X >= block->pos.X && entity->Pos.X <= block->pos.X + block->sideLength
         && entity->Pos.Y + entity->Hitbox.Y >= block->pos.Y && entity->Pos.Y <= block->pos.Y + block->sideLength
         && entity->Pos.Z + entity->Hitbox.Z >= block->pos.Z && entity->Pos.Z <= block->pos.Z + block->sideLength)
            entity->Collide(*block);
    }
}

PositionedBlock *LazyWorld::CheckAim(Player *player) {
    PositionedBlock *block;
    Ray ray = Ray(player);
    
    PositionedBlock *target;
    float dist, best = 5.f;
    
    //for (int i = 0; i < Blocks.size(); i++) {
    for (std::list<PositionedBlock*>::iterator it = Blocks.begin(); it != Blocks.end(); ++it) {
        block = *it; // Blocks[i];
        
        dist = ray.CheckCollision(block);
        if (0.f < dist && dist < best) {
            best = dist;
            target = block;
        }
    }
    
    if (target != NULL)
        target->marked = true;
    
    return target;
}

void LazyWorld::DestroyTarget(Player *player) {
    PositionedBlock *block = CheckAim(player);
    if (!block) return;
    
    Blocks.remove(block);
}

bool contains(std::vector<Vector3> vector, Vector3 value) {
    for (int i = 0; i < vector.size(); i++)
        if ((vector[i].X == value.X) && (vector[i].Y == value.Y) && (vector[i].Z == value.Z))
            return true;
    
    return false;
}
  
void LazyWorld::LoadChunk(sf::Packet Packet) {
    int BlockCount;
    Vector3 Chunk; // TODO: remove from request list, add to loaded list
    Packet >> Chunk >> BlockCount;
    
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
    
    Blocks2.clear();
    PositionedBlock *pblock, *ppblock;
    for (std::list<PositionedBlock*>::iterator it = Blocks.begin(); it != Blocks.end(); ++it) {
        pblock = *it;
        char sides = 0x3F;
        
        for (std::list<PositionedBlock*>::iterator it2 = Blocks.begin(); it2 != Blocks.end(); ++it2) {
            ppblock = *it2;
            
            if (ppblock->pos.X + ppblock->sideLength == pblock->pos.X
             && ppblock->pos.Y == pblock->pos.Y && ppblock->pos.Z == pblock->pos.Z)
                sides &= (0xFE); // 0b00000001
            
            else if (ppblock->pos.Y + ppblock->sideLength == pblock->pos.Y
             && ppblock->pos.X == pblock->pos.X && ppblock->pos.Z == pblock->pos.Z)
                sides &= (0xFD); // 0b00000010
            
            else if (ppblock->pos.Z + ppblock->sideLength == pblock->pos.Z
             && ppblock->pos.Y == pblock->pos.Y && ppblock->pos.Y == pblock->pos.Y)
                sides &= (0xFB); // 0b00000100
            
            
            else if (ppblock->pos.X == pblock->pos.X + pblock->sideLength
             && ppblock->pos.Y == pblock->pos.Y && ppblock->pos.Z == pblock->pos.Z)
                sides &= (0xF7); // 0b00001000
            
            else if (ppblock->pos.Y == pblock->pos.Y + pblock->sideLength
             && ppblock->pos.X == pblock->pos.X && ppblock->pos.Z == pblock->pos.Z)
                sides &= (0xEF); // 0b00010000
            
            else if (ppblock->pos.Z == pblock->pos.Z + pblock->sideLength
             && ppblock->pos.Y == pblock->pos.Y && ppblock->pos.Y == pblock->pos.Y)
                sides &= (0xDF); // 0b00100000
        }
        
        if (sides > 0)
            Blocks2.push_back(pblock);
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
    Packet << (sf::Uint8) 4 << ChunkIndex;
    Socket.Send(Packet);
    
    RequestedChunks.push_back(ChunkIndex);
}
