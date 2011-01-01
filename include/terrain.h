#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "octree.h"

using namespace std;

class Terrain
{
    public:
        Terrain();
        Terrain(int maxlevel, int minlevel);

        int Maxlevel; // Lowest level in the octree possible
        int Minlevel; // Minimum level at which we start getting leaves instead of nodes
        Octree<bool> GeneratedTerrain;

        void Regenerate();

    private:
        Octree<bool> makeTerrainFrom(int level); //Generates the terrain from the specified level downwards. Almost always 0.
};

#endif
