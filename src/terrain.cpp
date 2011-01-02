#include "terrain.h"
#include <SFML/System.hpp>
#include <iostream>

Terrain::Terrain() : Maxlevel(5), Minlevel(0), sizeX(1), sizeY(1), sizeZ(1), chunkSize(50) { Regenerate(); } /* Some sort of default values. TODO Discuss and decide whether these should be different */

Terrain::Terrain(int maxlevel, int minlevel, int initsizeX, int initsizeY, int initsizeZ, int initChunkSize) : Maxlevel(maxlevel), Minlevel(minlevel), sizeX(initsizeX), sizeY(initsizeY), sizeZ(initsizeZ), chunkSize(initChunkSize) { Regenerate(); }

void Terrain::Regenerate()
{
    GeneratedTerrain = std::map<Coord, Octree<bool> >();
    for(int i = 0; i < sizeX; i++)
    {
        for(int j = 0; j < sizeY; j++)
        {
            for(int k = 0; k < sizeZ; k++)
            {
                GeneratedTerrain[Coord(i,j,k)] = makeTerrainFrom(0);
            }
        }
    }
}
Octree<bool> Terrain::makeTerrainFrom(int level)
{
    std::vector<Octree<bool> >blocks;
    for(int i = 0; i < 8; i++)
    {
        bool leaf = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
        if(level < Maxlevel)
        {

            if(leaf && level > Minlevel)
            {
                bool type = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
                blocks.push_back(Octree<bool>(type));
            } 
            else
            {
                blocks.push_back(makeTerrainFrom(level + 1));
            }
        }
        else if(level == Maxlevel)
        {
            bool type = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
            blocks.push_back(Octree<bool>(type));
        }
    }
    Octree<bool> terrain(blocks);
    return terrain;
}

