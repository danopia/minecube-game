#pragma once
#ifndef BLOCK_H
#define BLOCK_H

class Block
{
    public:
        Block(char Type = 0, int DamageTaken = 0, int DamageThreshold = 0) : Type(Type), faces(0x3F), DamageTaken(DamageTaken), DamageThreshold(DamageThreshold) {};
        virtual Block *newBlock() {
            return new Block(); // TODO: throw
        };

        char Type;
        char faces;
        int DamageTaken;
        int DamageThreshold;
};

class AirBlock: public Block
{
    public:
        AirBlock() : Block(0) {};

        AirBlock *newBlock()
        {
            return new AirBlock();
        }
};

class StoneBlock: public Block
{
    public:
        StoneBlock() : Block(1, 0, 50) {};

        StoneBlock *newBlock()
        {
            return new StoneBlock();
        }
};

class DirtBlock: public Block
{
    public:
        DirtBlock() : Block(2, 0, 10) {};

        DirtBlock *newBlock()
        {
            return new DirtBlock();
        }
};

class GrassBlock: public Block
{
    public:
        GrassBlock() : Block(3, 0, 15) {};

        GrassBlock *newBlock()
        {
            return new GrassBlock();
        }
};

#endif
