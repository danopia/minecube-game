#include "common/lazyworld.h"
#include <cmath>

const double PI = 3.14159265358979323846264338329750288419716939937510582;

#include <stdio.h>

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
    float x, y, z, dx, dy, dz, xp, yp, zp, dp; // p means 'prime'
    
    x = player->Pos.X + 0.5;
    y = player->Pos.Y + 0.5;
    z = player->Pos.Z + 1.5;
    
    dx =  sin((PI * player->Rotation.Z) / 180);
    dy =  cos((PI * player->Rotation.Z) / 180);
    dz = -tan((PI * player->Rotation.Y) / 180);
    
    if (dx == 0.f) dx = 0.0001f;
    if (dy == 0.f) dy = 0.0001f;
    if (dz == 0.f) dz = 0.0001f;
    
    for (int i = 0; i < Blocks.size(); i++) {
        block = Blocks[i];
        
        // x is fixed at block.x
        dp = (block->pos.X - x) / dx;
        yp = y + dy * dp;
        zp = z + dz * dp;
        if (yp >= block->pos.Y && yp <= block->pos.Y + block->sideLength
         && zp >= block->pos.Z && zp <= block->pos.Z + block->sideLength)
            block->marked = true;
        
        // y is fixed at block.y
        dp = (block->pos.Y - y) / dy;
        xp = x + dx * dp;
        zp = z + dz * dp;
        if (xp >= block->pos.X && xp <= block->pos.X + block->sideLength
         && zp >= block->pos.Z && zp <= block->pos.Z + block->sideLength)
            block->marked = true;
        
        // z is fixed at block.z
        dp = (block->pos.Z - z) / dz;
        xp = x + dx * dp;
        yp = y + dy * dp;
        if (yp >= block->pos.Y && yp <= block->pos.Y + block->sideLength
         && xp >= block->pos.X && xp <= block->pos.X + block->sideLength)
            block->marked = true;
        
        
        // x is fixed at block.x + block.size
        dp = (block->pos.X + block->sideLength - x) / dx;
        yp = y + dy * dp;
        zp = z + dz * dp;
        if (yp >= block->pos.Y && yp <= block->pos.Y + block->sideLength
         && zp >= block->pos.Z && zp <= block->pos.Z + block->sideLength)
            block->marked = true;
        
        // y is fixed at block.y + block.size
        dp = (block->pos.Y - y) / dy;
        xp = x + dx * dp;
        zp = z + dz * dp;
        if (xp >= block->pos.X && xp <= block->pos.X + block->sideLength
         && zp >= block->pos.Z && zp <= block->pos.Z + block->sideLength)
            block->marked = true;
        
        // z is fixed at block.z + block.size
        dp = (block->pos.Z - z) / dz;
        xp = x + dx * dp;
        yp = y + dy * dp;
        if (yp >= block->pos.Y && yp <= block->pos.Y + block->sideLength
         && xp >= block->pos.X && xp <= block->pos.X + block->sideLength)
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
