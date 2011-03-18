#include "common/lazyworld.h"
#include <cstdio>

void LazyWorld::DoStep() {
    // Collision check against player
}

void LazyWorld::CheckCollision(Entity *entity) {
    PositionedBlock *block;
    
    for (std::list<PositionedBlock*>::iterator it = VisibleBlocks.begin(); it != VisibleBlocks.end(); ++it) {
        block = *it;
        if (block->block->Type == 0) continue;
        
        if (entity->Pos.X + entity->Hitbox.X >= block->pos.X && entity->Pos.X <= block->pos.X + 1
         && entity->Pos.Y + entity->Hitbox.Y >= block->pos.Y && entity->Pos.Y <= block->pos.Y + 1
         && entity->Pos.Z + entity->Hitbox.Z >= block->pos.Z && entity->Pos.Z <= block->pos.Z + 1)
            entity->Collide(*block);
    }
}

PositionedBlock *LazyWorld::CheckAim(Player *player) {
    PositionedBlock *block;
    Ray ray = Ray(player);
    
    PositionedBlock *target;
    float dist, best = 5.f;
    
    for (std::list<PositionedBlock*>::iterator it = VisibleBlocks.begin(); it != VisibleBlocks.end(); ++it) {
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
    
    VisibleBlocks.remove(block);
}

bool contains(std::vector<Vector3> vector, Vector3 value) {
    for (int i = 0; i < vector.size(); i++)
        if (vector[i] == value) return true;
    
    return false;
}

void LazyWorld::LoadChunk(sf::Packet Packet) {
    int BlockCount;
    Vector3 ChunkIndex; // TODO: remove from request list, add to loaded list
    Packet >> ChunkIndex >> BlockCount;
    
    std::map<Vector3, Block*> chunk;
    
    sf::Uint8 type;
    Block *block;
    Vector3 Pos;
    float SideLength;
    
    printf("0 (%f, %f, %f)\n", ChunkIndex.X, ChunkIndex.Y, ChunkIndex.Z);
    
    for (int i = 0; i < BlockCount; i++) {
        Packet >> type >> Pos;
        
        if (type == 3)
            block = new GrassBlock();
        else
            block = new AirBlock();
        
        chunk[Vector3(Pos)] = block;
        //printf("1 (%f, %f, %f): %i\n", Pos.X, Pos.Y, Pos.Z, type);
    }
    
    for (std::map<Vector3, Block*>::iterator it = chunk.begin(); it != chunk.end(); ++it) {
        if (it->second->Type == 0) continue;
        Pos = it->first;
        
        char sides = 0x3F;
        
        if (Pos.X > 0 && chunk[Pos - Vector3(1, 0, 0)]->Type > 0)
            sides &= (0xFE); // 0b00000001
        
        if (Pos.Y > 0 && chunk[Pos - Vector3(0, 1, 0)]->Type > 0)
            sides &= (0xFD); // 0b00000010
        
        if (Pos.Z > 0 && chunk[Pos - Vector3(0, 0, 1)]->Type > 0)
            sides &= (0xFB); // 0b00000100
        
        
        if (Pos.X < 15 && chunk[Pos + Vector3(1, 0, 0)]->Type > 0)
            sides &= (0xF7); // 0b00001000
        
        if (Pos.Y < 15 && chunk[Pos + Vector3(0, 1, 0)]->Type > 0)
            sides &= (0xEF); // 0b00010000
        
        if (Pos.Z < 15 && chunk[Pos + Vector3(0, 0, 1)]->Type > 0)
            sides &= (0xDF); // 0b00100000
        
        it->second->faces = sides;
        
        if (sides > 0)
            VisibleBlocks.push_back(new PositionedBlock(it->second, (ChunkIndex * 16) + Pos, 1));
    }
    
    LoadedChunks[ChunkIndex] = chunk;
}

void LazyWorld::HandleRequests(Vector3 Pos) {
    Vector3 CurrentChunk;
    
    CurrentChunk.X = floor(Pos.X / ChunkSize);
    CurrentChunk.Y = floor(Pos.Y / ChunkSize);
    CurrentChunk.Z = floor(Pos.Z / ChunkSize);
    
    RequestChunk(CurrentChunk);
    RequestChunk(CurrentChunk + Vector3(-1, 0,  0));
    RequestChunk(CurrentChunk + Vector3(-1, -1, 0));
    RequestChunk(CurrentChunk + Vector3(0,  -1, 0));
    RequestChunk(CurrentChunk + Vector3(1,  -1, 0));
    RequestChunk(CurrentChunk + Vector3(1,  0,  0));
    RequestChunk(CurrentChunk + Vector3(1,  1,  0));
    RequestChunk(CurrentChunk + Vector3(0,  1,  0));
    RequestChunk(CurrentChunk + Vector3(-1, 1,  0));
}

void LazyWorld::RequestChunk(Vector3 ChunkIndex) {
    if (contains(RequestedChunks, ChunkIndex)) return;
    
    // Send request to the server
    sf::Packet Packet;
    Packet << (sf::Uint8) 4 << ChunkIndex;
    Socket.Send(Packet);
    
    RequestedChunks.push_back(ChunkIndex);
}
