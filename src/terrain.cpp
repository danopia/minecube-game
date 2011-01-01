#include "terrain.h"
#include <SFML/System.hpp>

Terrain::Terrain() : Maxlevel(5), Minlevel(0) { Regenerate(); } /* Some sort of default values. TODO Discuss and decide whether these should be different */

Terrain::Terrain(int maxlevel, int minlevel) : Maxlevel(maxlevel), Minlevel(minlevel) { Regenerate(); }

void Terrain::Regenerate()
{
    GeneratedTerrain = makeTerrainFrom(0);
}
Octree<bool> Terrain::makeTerrainFrom(int level)
{
    vector<Octree<bool> >blocks;
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


