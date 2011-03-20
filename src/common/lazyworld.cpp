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
    
    PositionedBlock *target = NULL;
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
    
    DestroyBlock(block);
}

void LazyWorld::DestroyBlock(PositionedBlock *block) {
    Vector3 chunk, pos;
    
    chunk.X = floor(block->pos.X / ChunkSize);
    chunk.Y = floor(block->pos.Y / ChunkSize);
    chunk.Z = floor(block->pos.Z / ChunkSize);
    
    // TODO: handle variable chunk sizes
    PlaceBlock(0, chunk, block->pos - (chunk * 16));
    
    // TODO
    //context->socket->SendBlock(0, chunkIndex, blockIndex);
    sf::Packet Packet;
    Packet << (sf::Uint8) 0 << chunk << pos;
    Socket.Send(Packet);
}

Block *MakeBlock(char type) {
    if (type == 3)
        return new GrassBlock();
    else
        return new AirBlock();
}

void LazyWorld::PlaceBlock(char type, Vector3 chunkIndex, Vector3 blockIndex) {
    // TODO: check if the block is visible before doing this loop
    // TODO: handle variable chunk sizes
    Vector3 absolute = (chunkIndex * 16) + blockIndex;
    for (std::list<PositionedBlock*>::iterator it = VisibleBlocks.begin(); it != VisibleBlocks.end(); ++it) {
        if ((*it)->pos == absolute) {
            VisibleBlocks.remove(*it);
            break;
        }
    }
    
    std::map<Vector3, Block*> chunk = LoadedChunks[chunkIndex];
    if (chunk.size() == 0) return; // chunk isn't loaded, don't bother
    
    Block *blk = MakeBlock(type);
    chunk[blockIndex] = blk;
    if (type != 0) VisibleBlocks.push_back(new PositionedBlock(blk, blockIndex, 1));
    
    // TODO: handle placing blocks without just counting on a [reliable] glitch!
    
    if (blockIndex.X > 0 && chunk[blockIndex - Vector3(1, 0, 0)]->Type > 0) {
        blk = chunk[blockIndex - Vector3(1, 0, 0)];
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, blockIndex - Vector3(1, 0, 0), 1));
        blk->faces |= 0x08;
    }
    
    if (blockIndex.Y > 0 && chunk[blockIndex - Vector3(0, 1, 0)]->Type > 0) {
        blk = chunk[blockIndex - Vector3(0, 1, 0)];
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, blockIndex - Vector3(0, 1, 0), 1));
        blk->faces |= 0x10;
    }
    
    if (blockIndex.Z > 0 && chunk[blockIndex - Vector3(0, 0, 1)]->Type > 0) {
        blk = chunk[blockIndex - Vector3(0, 0, 1)];
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, blockIndex - Vector3(0, 0, 1), 1));
        blk->faces |= 0x20;
    }
    
    // TODO: handle variable chunk sizes
    if (blockIndex.X < 15 && chunk[blockIndex + Vector3(1, 0, 0)]->Type > 0) {
        blk = chunk[blockIndex + Vector3(1, 0, 0)];
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, blockIndex + Vector3(1, 0, 0), 1));
        blk->faces |= 0x01;
    }
    
    if (blockIndex.Y < 15 && chunk[blockIndex + Vector3(0, 1, 0)]->Type > 0) {
        blk = chunk[blockIndex + Vector3(0, 1, 0)];
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, blockIndex + Vector3(0, 1, 0), 1));
        blk->faces |= 0x02;
    }
    
    if (blockIndex.Z < 15 && chunk[blockIndex + Vector3(0, 0, 1)]->Type > 0) {
        blk = chunk[blockIndex + Vector3(0, 0, 1)];
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, blockIndex + Vector3(0, 0, 1), 1));
        blk->faces |= 0x04;
    }
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
    
    for (int i = 0; i < BlockCount; i++) {
        Packet >> type >> Pos;
        
        chunk[Vector3(Pos)] = MakeBlock(type);
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
        
        // TODO: handle variable chunk sizes
        
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
    
    // TODO: lol.
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
