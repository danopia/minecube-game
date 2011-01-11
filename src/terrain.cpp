#include "terrain.h"
#include <SFML/System.hpp>
#include <iostream>

Terrain::Terrain() : Maxlevel(5), Minlevel(0), sizeX(1), sizeY(1), sizeZ(1), chunkSize(50) { Regenerate(); } /* Some sort of default values. TODO Discuss and decide whether these should be different */

Terrain::Terrain(int maxlevel, int minlevel, int initsizeX, int initsizeY, int initsizeZ, int initChunkSize) : Maxlevel(maxlevel), Minlevel(minlevel), sizeX(initsizeX), sizeY(initsizeY), sizeZ(initsizeZ), chunkSize(initChunkSize) { Regenerate(); }

void Terrain::Regenerate()
{
    GeneratedTerrain = std::map<Vector3, Octree<Block*> >();
    int i, j, k;
    for(i = 0; i < sizeX; i++)
    {
        for(j = 0; j < sizeY; j++)
        {
            for(k = 0; k < sizeZ; k++)
            {
                GeneratedTerrain[Vector3(i,j,k)] = makeTerrainFrom(0);
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

void SerializeOctree(std::ofstream *out, Octree<Block*> octree) {
    if (octree.hasChildren) {
        char bitfield = 0;
        
        for (int i = 0; i < 8; i++)
            if (octree.children[i].hasChildren)
                bitfield |= (1 << i);
        
        out->write(&bitfield, 1);
        
        for (int i = 0; i < 8; i++)
            SerializeOctree(out, octree.children[i]);
    } else {
        out->write(&octree.value->Type, 1);
    }
}

void Terrain::Serialize() {
    std::ofstream out("terrain.bin", std::ios::binary);
    
    out.write("MCworld-0\n", 10); // Magic number
    
    out.write((char*)&Maxlevel, sizeof(Maxlevel));
    out.write((char*)&Minlevel, sizeof(Minlevel));
    out.write((char*)&sizeX, sizeof(sizeX));
    out.write((char*)&sizeY, sizeof(sizeY));
    out.write((char*)&sizeZ, sizeof(sizeZ));
    out.write((char*)&chunkSize, sizeof(chunkSize));
    
    int chunks = GeneratedTerrain.size();
    out.write((char*)&chunks, sizeof(chunks));
    
    std::map<Vector3, Octree<Block*> >::iterator Iter;
    for (Iter = GeneratedTerrain.begin(); Iter != GeneratedTerrain.end(); ++Iter) {
        out.write((char*)&Iter->first.X, sizeof(Iter->first.X));
        out.write((char*)&Iter->first.Y, sizeof(Iter->first.Y));
        out.write((char*)&Iter->first.Z, sizeof(Iter->first.Z));
        
        out.write((char*)&Iter->second.hasChildren, sizeof(Iter->second.hasChildren));
        
        SerializeOctree(&out, Iter->second);
    }

    out.close();
}

//    ifstream in("binary.txt", ios::binary); 
//    in.read((char*)&read, sizeof(read)); 
