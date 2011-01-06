#include "terrain.h"
#include <SFML/System.hpp>
#include <iostream>

Terrain::Terrain() : Maxlevel(5), Minlevel(0), sizeX(1), sizeY(1), sizeZ(1), chunkSize(50) { Regenerate(); } /* Some sort of default values. TODO Discuss and decide whether these should be different */

Terrain::Terrain(int maxlevel, int minlevel, int initsizeX, int initsizeY, int initsizeZ, int initChunkSize) : Maxlevel(maxlevel), Minlevel(minlevel), sizeX(initsizeX), sizeY(initsizeY), sizeZ(initsizeZ), chunkSize(initChunkSize) { Regenerate(); }

void Terrain::Regenerate()
{
    GeneratedTerrain = std::map<Coord, Octree<Block*> >();
    int i, j, k;
    for(i = 0; i < sizeX; i++)
    {
        for(j = 0; j < sizeY; j++)
        {
            for(k = 0; k < sizeZ; k++)
            {
                GeneratedTerrain[Coord(i,j,k)] = makeTerrainFrom(0);
            }
        }
    }
}
Octree<Block*> Terrain::makeTerrainFrom(int level)
{
    std::vector<Octree<Block*> >blocks;
    for(int i = 0; i < 8; i++)
    {
        if(level < Maxlevel)
        {
            bool leaf = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
            
            if(leaf && level > Minlevel)
            {
                // TODO: Change to make a random block type, not only dirt or air
                bool type = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
                if (type)
                    blocks.push_back(Octree<Block*>(new DirtBlock()));
                else
                    blocks.push_back(Octree<Block*>(new AirBlock()));
            } 
            else
            {
                blocks.push_back(makeTerrainFrom(level + 1));
            }
        }
        else if(level == Maxlevel)
        {
            // TODO: Same as above
            bool type = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
            if (type)
                    blocks.push_back(Octree<Block*>(new DirtBlock()));
            else
                blocks.push_back(Octree<Block*>(new AirBlock()));
        }
    }
    Octree<Block*> terrain(blocks);
    return terrain;
}

