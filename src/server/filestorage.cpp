#include "server/filestorage.h"

// Terrain &Callback;
// std::map<Vector3, Chunk*> Loaded;

Chunk *FileStorage::RequestChunk(Vector3 Index) {
    return GenerateChunk(Index); // TODO: use cache for persistance
}

// TODO: goes elsewhere!
Block *MakeBlock3(char type) {
    if (type == 3)
        return new GrassBlock();
    else
        return new AirBlock();
}

Chunk *FileStorage::GenerateChunk(Vector3 Index) {
    Chunk *chunk = new Chunk(Index, Callback->ChunkSize);
    printf("size: %i\n", Callback->ChunkSize);
    chunk->FillWith(3);
    return chunk;
}

/*
Chunk FileStorage::ReadChunk(sf::Packet &Packet) {
    int BlockCount;
    Vector3 ChunkIndex;
    Packet >> ChunkIndex >> BlockCount;
    
    Chunk chunk(ChunkIndex, 16);
    
    sf::Uint8 type;
    Block *block;
    Vector3 Pos;
    float brb;
    
    for (int i = 0; i < BlockCount; i++) {
        Packet >> type >> Pos;
        
        chunk.Blocks[Vector3(Pos)] = MakeBlock2(type);
    }
    
    Loaded[ChunkIndex] = &chunk;
    Callback->LoadChunk(chunk);
    
    return chunk;
}
*/

/*#include "common/terrain.h"
#include <SFML/System.hpp>
#include <iostream>

Terrain::Terrain() : Maxlevel(5), Minlevel(0), sizeX(1), sizeY(1), sizeZ(1), chunkSize(50) {} // Some sort of default values. TODO Discuss and decide whether these should be different

Terrain::Terrain(int maxlevel, int minlevel, int initsizeX, int initsizeY, int initsizeZ, int initChunkSize) : Maxlevel(maxlevel), Minlevel(minlevel), sizeX(initsizeX), sizeY(initsizeY), sizeZ(initsizeZ), chunkSize(initChunkSize) {}

void Terrain::Regenerate()
{
    GeneratedTerrain = std::map<Vector3, Octree<Block*> >();
    
    for(int i = 0; i < sizeX; i++)
    {
        for(int j = 0; j < sizeY; j++)
        {
            for(int k = 0; k < sizeZ; k++)
            {
                GeneratedTerrain[Vector3(i,j,k)] = makeTerrainFrom(0, 0);
            }
        }
    }
}

Octree<Block*> Terrain::GenerateChunk(Vector3 ChunkIndex) {
    Octree<Block*> chunk = makeTerrainFrom(0, 0);
    GeneratedTerrain[ChunkIndex] = chunk;
    return chunk;
}

Octree<Block*> Terrain::makeTerrainFrom(int level, int type)
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
                //bool type = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
                if (type == 0)
                    type = (i < 4) ? 1 : 2;
                
                if (type == 1)
                    blocks.push_back(Octree<Block*>(new GrassBlock()));
                else
                    blocks.push_back(Octree<Block*>(new AirBlock()));
            } 
            else
            {
                if (type == 0)
                    blocks.push_back(makeTerrainFrom(level + 1, (i < 4) ? 1 : 2));
                else
                    blocks.push_back(makeTerrainFrom(level + 1, type));
            }
        }
        else if(level == Maxlevel)
        {
            // TODO: Same as above
            //bool type = (sf::Randomizer::Random(-1.f, 1.f) <= 0.0f ? true : false);
            if (type == 0)
                type = (i < 4) ? 1 : 2;
            
            if (type == 1)
                blocks.push_back(Octree<Block*>(new GrassBlock()));
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

void Terrain::SaveToFile(std::string filename) {
    std::ofstream out(filename.c_str(), std::ios::binary);
    
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



Octree<Block*> UnserializeOctree(std::ifstream *in, bool parent) {
    if (parent) {
        char bitfield = 0;
        in->read(&bitfield, 1);
        
        std::vector<Octree<Block*> >blocks;
        for (int i = 0; i < 8; i++) {
            bool parent = (bitfield & (1 << i)) > 0;
            blocks.push_back(UnserializeOctree(in, parent));
        }
        
        return Octree<Block*>(blocks);
        
    } else {
        char type = 0;
        in->read(&type, 1);
        
        if (type == 3)
            return Octree<Block*>(new GrassBlock());
        else
            return Octree<Block*>(new AirBlock());
    }
}

void Terrain::LoadFromFile(std::string filename) {
    std::ifstream in(filename.c_str(), std::ios::binary);
    
    char buf[11];
    in.read(buf, 10); // Magic number
    if (buf != "MCworld-0\n") int MUAHAHAHAHAHAHAHAHA = 1/0;
    
    in.read((char*)&Maxlevel, sizeof(Maxlevel));
    in.read((char*)&Minlevel, sizeof(Minlevel));
    in.read((char*)&sizeX, sizeof(sizeX));
    in.read((char*)&sizeY, sizeof(sizeY));
    in.read((char*)&sizeZ, sizeof(sizeZ));
    in.read((char*)&chunkSize, sizeof(chunkSize));
    
    int chunks = 0;
    in.read((char*)&chunks, sizeof(chunks));
    
    GeneratedTerrain = std::map<Vector3, Octree<Block*> >();
    for (int i = 0; i < chunks; i++) {
        Vector3 coord;
        in.read((char*)&coord.X, sizeof(coord.X));
        in.read((char*)&coord.Y, sizeof(coord.Y));
        in.read((char*)&coord.Z, sizeof(coord.Z));
        
        char parent;
        in.read(&parent, 1);
        
        GeneratedTerrain[coord] = UnserializeOctree(&in, parent == 1);
    }

    in.close();
}
*/

