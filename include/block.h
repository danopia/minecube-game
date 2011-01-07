#pragma once
#ifndef BLOCK_H
#define BLOCK_H

class Block
{
    public:
        Block() {};
        virtual Block *newBlock() {};
        
        char faces; // Faces to render (using 6 bits)
        char Type;
        int DamageTaken;
        int DamageThreshold;
};

class AirBlock: public Block
{
public:
    AirBlock() : DamageTaken(0), faces(0x3F), Type(0), DamageThreshold(-1) {};

    AirBlock *newBlock()
    {
        return new AirBlock();
    }

    char faces;
    int DamageTaken;
    char Type;
    int DamageThreshold;
};

class StoneBlock: public Block
{
    public:

        StoneBlock() : DamageTaken(0), faces(0x3F), Type(1), DamageThreshold(50) {};

        StoneBlock *newBlock()
        {
            return new StoneBlock();
        }
        
        char faces;
        char Type;
        int DamageTaken;
        int DamageThreshold;
};

class DirtBlock: public Block
{
    public:

        DirtBlock() : DamageTaken(0), faces(0x3F), Type(2), DamageThreshold(10) {};

        DirtBlock *newBlock()
        {
            return new DirtBlock();
        }
        
        char faces;
        char Type;
        int DamageTaken;
        int DamageThreshold;
};

class GrassBlock: public Block
{
    public:

        GrassBlock() : DamageTaken(0), faces(0x3F), Type(3), DamageThreshold(15) {};

        GrassBlock *newBlock()
        {
            return new GrassBlock();
        }
        
        char faces;
        char Type;
        int DamageTaken;
        int DamageThreshold;
};
#endif
