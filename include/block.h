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
    AirBlock() : DamageTaken(0), faces(0), Type(0), DamageThreshold(-1) {};

    AirBlock *newBlock()
    {
        return new AirBlock();
    }

    char faces;
    int DamageTaken;
    char Type;
    int DamageThreshold;
};

class DirtBlock: public Block
{
    public:

        DirtBlock() : DamageTaken(0), faces(0), Type(0), DamageThreshold(10) {};

        DirtBlock *newBlock()
        {
            return new DirtBlock();
        }
        
        char faces;
        char Type;
        int DamageTaken;
        int DamageThreshold;
};
#endif
