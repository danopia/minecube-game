#include "common/chunk.h"

//Chunk::Chunk(Vector3 Offset, std::map<Vector3, Block*> Blocks) : Index(Index), Blocks(Blocks) {};
//Chunk::Chunk(Vector3 Offset, int SideLength) : Offset(Offset), SideLength(SideLength) {};

// Vector3 Offset;
// int SideLength;
// std::list<Entity> Entities;
// std::map<Vector3, Block*> Blocks;

// TODO: goes elsewhere?
Block *MakeBlock(char type) {
    if (type == 3)
        return new GrassBlock();
    else
        return new AirBlock();
}


#include <cstdio>

void Chunk::FillWith(char type) {
    int x, y, z;
    
    printf("(%f,\t%f,\t%f)\n", Offset.X, Offset.Y, Offset.Z);
    
    for  (x = 0; x < SideLength; x++)
        for (y = 0; y < SideLength; y++)
            for (z = 0; z < SideLength; z++) {
                //printf("(%i,\t%i,\t%i):\t%i\n", x, y, z, type);
                Blocks[Vector3(x, y, z)] = MakeBlock(type);
            }
    
    RecalcSides();
}

void Chunk::RecalcSides() {
    Block *block;
    Vector3 Pos;
    int Upper = SideLength - 1;
    
    for (std::map<Vector3, Block*>::iterator it = Blocks.begin(); it != Blocks.end(); ++it) {
        if (it->second->Type == 0) continue;
        Pos = it->first;
        
        char sides = 0x3F;
        
        if (Pos.X > 0 && Blocks[Pos - Vector3(1, 0, 0)]->Type > 0)
            sides &= (0xFE); // 0b00000001
        
        if (Pos.Y > 0 && Blocks[Pos - Vector3(0, 1, 0)]->Type > 0)
            sides &= (0xFD); // 0b00000010
        
        if (Pos.Z > 0 && Blocks[Pos - Vector3(0, 0, 1)]->Type > 0)
            sides &= (0xFB); // 0b00000100
        
        // TODO: handle variable chunk sizes
        
        if (Pos.X < Upper && Blocks[Pos + Vector3(1, 0, 0)]->Type > 0)
            sides &= (0xF7); // 0b00001000
        
        if (Pos.Y < Upper && Blocks[Pos + Vector3(0, 1, 0)]->Type > 0)
            sides &= (0xEF); // 0b00010000
        
        if (Pos.Z < Upper && Blocks[Pos + Vector3(0, 0, 1)]->Type > 0)
            sides &= (0xDF); // 0b00100000
        
        it->second->faces = sides;
        
        //if (sides > 0)
        //    VisibleBlocks.push_back(new PositionedBlock(it->second, (ChunkIndex * 16) + Pos, 1));
    }
}


Block *Chunk::GetBlock(Vector3 blockIndex) {
    // TODO: check bounds?
    
    return Blocks[blockIndex];
}

Block *Chunk::DestroyBlock(Vector3 blockIndex) {
    return PlaceBlock(0, blockIndex);
}

Block *Chunk::PlaceBlock(char type, Vector3 index) {
    /*Vector3 absolute = (chunkIndex * 16) + blockIndex;
    for (std::list<PositionedBlock*>::iterator it = VisibleBlocks.begin(); it != VisibleBlocks.end(); ++it) {
        if ((*it)->pos == absolute) {
            VisibleBlocks.remove(*it);
            break;
        }
    }*/
    
    Block *block = MakeBlock(type);
    Blocks[index] = block;
    //if (type != 0) VisibleBlocks.push_back(new PositionedBlock(block, absolute, 1));
    
    return block;
}


Vector3 Chunk::GetWorldPos(Vector3 blockIndex) {
    return (Offset * SideLength) + blockIndex;
}

Vector3 Chunk::GetChunkPos(Vector3 worldPos) {
    return worldPos - (Offset * SideLength);
}

