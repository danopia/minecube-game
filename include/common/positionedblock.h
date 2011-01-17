#pragma once
#ifndef POSITIONEDBLOCK_H
#define POSITIONEDBLOCK_H

#include "common/block.h"
#include "common/vector3.h"

class PositionedBlock
{
    public:
        PositionedBlock(Block *block, Vector3 pos, float sideLength) : block(block), pos(pos), sideLength(sideLength) {};
        
        Block *block;
        Vector3 pos;
        float sideLength;
};

#endif

