#pragma once
#ifndef BLOCKS_H
#define BLOCKS_H

#include "common/block.h"
#include <map>

class AllBlocks
{
    public:
        static std::map<int, Block*> Blocks;

        AllBlocks()
        {
            Blocks[0] = new AirBlock();
            Blocks[1] = new StoneBlock();
            Blocks[2] = new DirtBlock();
            Blocks[3] = new GrassBlock();
        }
};

#endif
