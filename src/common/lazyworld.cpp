#include "common/lazyworld.h"

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

