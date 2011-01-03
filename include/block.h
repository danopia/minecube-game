#pragma once
#ifndef BLOCK_H
#define BLOCK_H

class Block
{
    public:
        Block() : DamageTaken(0) {};
        virtual Block *newBlock();
    protected:
        const static int Type;
        int DamageTaken;
        const static int DamageThreshold;
};

class DirtBlock: public Block
{
    public:

        DirtBlock()
        {
            DamageTaken = 0;
        }

        DirtBlock *newBlock()
        {
            return new DirtBlock();
        }
    protected:
        const static int Type = 1;
        int DamageTaken;
        const static int DamageThreshold = 10;
};

#endif
