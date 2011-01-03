#pragma once
#ifndef BLOCKS_H
#define BLOCKS_H

#include "block.h"
#include <map>

class AllBlocks
{
    public:
        static std::map<int, Block*> Blocks;

        AllBlocks()
        {
            Blocks[1] = new DirtBlock();
        }
};

#endif
