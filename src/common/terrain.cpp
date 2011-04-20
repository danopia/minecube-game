#include "common/terrain.h"

//int ChunkSize; // Side length of each chunk, in blocks

//WorldStorage *Storage;
//std::map<Vector3, std::map<Vector3, Block*> > LoadedChunks;
//std::list<PositionedBlock*> VisibleBlocks;
//std::list<Entity> Entities;


bool contains(std::vector<Vector3> vector, Vector3 value) {
    for (int i = 0; i < vector.size(); i++)
        if (vector[i] == value) return true;
    
    return false;
}


void Terrain::DoStep() {
    // Collision check against player, or something
}

void Terrain::CheckCollision(Entity *entity) {
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

PositionedBlock *Terrain::CheckAim(Player *player) {
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

void Terrain::DestroyTarget(Player *player) {
    PositionedBlock *block = CheckAim(player);
    if (!block) return;
    
    DestroyBlock(block);
}

void Terrain::DestroyBlock(PositionedBlock *block) {
    Vector3 chunk, pos;
    
    chunk.X = floor(block->pos.X / ChunkSize);
    chunk.Y = floor(block->pos.Y / ChunkSize);
    chunk.Z = floor(block->pos.Z / ChunkSize);
    pos = block->pos - (chunk * ChunkSize);
    
    PlaceBlock(0, chunk, pos);
}

void Terrain::PlaceBlock(char type, Vector3 chunkIndex, Vector3 blockIndex) {
    // chunk isn't loaded, don't bother. TODO: doublecheck
    if (!contains(RequestedChunks, chunkIndex)) return;
    
    Chunk *chunk = LoadedChunks[chunkIndex];
    
    Vector3 absolute = chunk->GetWorldPos(blockIndex);
    Block *block = chunk->GetBlock(blockIndex);
    
    if (block->Type > 0 && block->faces != 0) {
        for (std::list<PositionedBlock*>::iterator it = VisibleBlocks.begin(); it != VisibleBlocks.end(); ++it) {
            if ((*it)->pos == absolute) {
                VisibleBlocks.remove(*it);
                break;
            }
        }
    }
    
    block = chunk->PlaceBlock(type, blockIndex);
    Storage->PlaceBlock(type, chunkIndex, blockIndex);
    
    // TODO: handle placing blocks without just counting on a [reliable] glitch!
    
    Block *blk = NULL;
    if (blockIndex.X > 0 && chunk->GetBlock(blockIndex - Vector3(1, 0, 0))->Type > 0) {
        blk = chunk->GetBlock(blockIndex - Vector3(1, 0, 0));
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, absolute - Vector3(1, 0, 0), 1));
        blk->faces |= 0x08;
    }
    
    if (blockIndex.Y > 0 && chunk->GetBlock(blockIndex - Vector3(0, 1, 0))->Type > 0) {
        blk = chunk->GetBlock(blockIndex - Vector3(0, 1, 0));
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, absolute - Vector3(0, 1, 0), 1));
        blk->faces |= 0x10;
    }
    
    if (blockIndex.Z > 0 && chunk->GetBlock(blockIndex - Vector3(0, 0, 1))->Type > 0) {
        blk = chunk->GetBlock(blockIndex - Vector3(0, 0, 1));
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, absolute - Vector3(0, 0, 1), 1));
        blk->faces |= 0x20;
    }
    
    int Upper = chunk->SideLength - 1;
    if (blockIndex.X < Upper && chunk->GetBlock(blockIndex + Vector3(1, 0, 0))->Type > 0) {
        blk = chunk->GetBlock(blockIndex + Vector3(1, 0, 0));
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, absolute + Vector3(1, 0, 0), 1));
        blk->faces |= 0x01;
    }
    
    if (blockIndex.Y < Upper && chunk->GetBlock(blockIndex + Vector3(0, 1, 0))->Type > 0) {
        blk = chunk->GetBlock(blockIndex + Vector3(0, 1, 0));
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, absolute + Vector3(0, 1, 0), 1));
        blk->faces |= 0x02;
    }
    
    if (blockIndex.Z < Upper && chunk->GetBlock(blockIndex + Vector3(0, 0, 1))->Type > 0) {
        blk = chunk->GetBlock(blockIndex + Vector3(0, 0, 1));
        if (blk->faces == 0) VisibleBlocks.push_back(new PositionedBlock(blk, absolute + Vector3(0, 0, 1), 1));
        blk->faces |= 0x04;
    }
    
    if (type != 0 && blk->faces != 0)
        VisibleBlocks.push_back(new PositionedBlock(block, absolute, 1));
}

//std::vector<Vector3> RequestedChunks;

Chunk *Terrain::GetChunk(Vector3 index) {
    if (contains(RequestedChunks, index)) {
        return LoadedChunks[index];
    } else {
        Chunk *chunk = Storage->RequestChunk(index);
        RequestedChunks.push_back(index); // TODO: use a RequestChunk helper
        
        if (chunk != NULL) {
            LoadedChunks[index] = chunk;
        }
        
        return chunk;
    }
}

void Terrain::LoadChunk(Chunk *chunk) {
    sf::Uint8 type;
    Block *block;
    Vector3 Pos;
    
    int Upper = chunk->SideLength - 1;
    
    for (std::map<Vector3, Block*>::iterator it = chunk->Blocks.begin(); it != chunk->Blocks.end(); ++it) {
        if (it->second->Type == 0) continue;
        Pos = it->first;
        
        char sides = 0x3F;
        
        if (Pos.X > 0 && chunk->GetBlock(Pos - Vector3(1, 0, 0))->Type > 0)
            sides &= (0xFE); // 0b00000001
        
        if (Pos.Y > 0 && chunk->GetBlock(Pos - Vector3(0, 1, 0))->Type > 0)
            sides &= (0xFD); // 0b00000010
        
        if (Pos.Z > 0 && chunk->GetBlock(Pos - Vector3(0, 0, 1))->Type > 0)
            sides &= (0xFB); // 0b00000100
        
        if (Pos.X < Upper && chunk->GetBlock(Pos + Vector3(1, 0, 0))->Type > 0)
            sides &= (0xF7); // 0b00001000
        
        if (Pos.Y < Upper && chunk->GetBlock(Pos + Vector3(0, 1, 0))->Type > 0)
            sides &= (0xEF); // 0b00010000
        
        if (Pos.Z < Upper && chunk->GetBlock(Pos + Vector3(0, 0, 1))->Type > 0)
            sides &= (0xDF); // 0b00100000
        
        it->second->faces = sides;
        
        if (sides > 0)
            VisibleBlocks.push_back(new PositionedBlock(it->second, chunk->GetWorldPos(Pos), 1));
    }
    
    LoadedChunks[chunk->Offset] = chunk;
}

void Terrain::HandleRequests(Vector3 Pos) {
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
    
    RequestChunk(CurrentChunk + Vector3(0,  0,  1));
    RequestChunk(CurrentChunk + Vector3(0,  0, -1));
}

void Terrain::RequestChunk(Vector3 index) {
    if (contains(RequestedChunks, index)) return;
    
    // Send request to the server
    //sf::Packet Packet;
    //Packet << (sf::Uint8) 4 << index;
    //Socket.Send(Packet);
    
    Storage->RequestChunk(index);
    
    RequestedChunks.push_back(index);
}

