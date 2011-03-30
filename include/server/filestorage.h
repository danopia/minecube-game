#pragma once
#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include "common/worldstorage.h"

#include <iostream>
#include <fstream>

class FileStorage
{
    public:
        FileStorage(Terrain &Callback) : WorldStorage(Callback) {};
        
        void SaveToFile(std::string filename);
        void LoadFromFile(std::string filename);
        
    private:
        Chunk GenerateChunk(Vector3 Index);
};

#endif

/*

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

*/

