#include <SFML/Network.hpp>
#include <iostream>
#include <vector>

#include "common/terrain.h"
#include "common/vector3.h"
#include "common/octree.h"
#include "common/player.h"
#include "common/packets.h"
#include "server/heartbeat.h"

// Choose a random port for opening sockets (ports < 1024 are reserved)
const unsigned short Port = 28997;

Terrain *terrain;
Heartbeat beater;

void listBlocks(std::vector<PositionedBlock> *Blocks, Octree<Block*> octree, float x, float y, float z, float size) {
    if (octree.hasChildren) {
        float subsize = size / 2;
        listBlocks(Blocks, octree.children[0], x,         y,         z,         subsize);
        listBlocks(Blocks, octree.children[1], x+subsize, y,         z,         subsize);
        listBlocks(Blocks, octree.children[2], x,         y+subsize, z,         subsize);
        listBlocks(Blocks, octree.children[3], x+subsize, y+subsize, z,         subsize);
        listBlocks(Blocks, octree.children[4], x,         y,         z+subsize, subsize);
        listBlocks(Blocks, octree.children[5], x+subsize, y,         z+subsize, subsize);
        listBlocks(Blocks, octree.children[6], x,         y+subsize, z+subsize, subsize);
        listBlocks(Blocks, octree.children[7], x+subsize, y+subsize, z+subsize, subsize);
    } else if (octree.value->Type > 0) {
        Blocks->push_back(PositionedBlock(octree.value, Vector3(x, y, z), size));
    }
}

void sendTerrain(sf::SocketTCP Client, Vector3 ChunkIndex) {
    printf("(%f,%f,%f)\n", ChunkIndex.X, ChunkIndex.Y, ChunkIndex.Z);
    
    Octree<Block*> Chunk = terrain->GeneratedTerrain[ChunkIndex];
    
    if (!Chunk) {
        sf::Packet Packet;
        Packet << "Take this chunk. It will be useful in times of need.";
        Packet << (int) 0;
        Client.Send(Packet);
        
        return;
    }
    
    std::vector<PositionedBlock> Blocks;
    listBlocks(&Blocks, Chunk,
               ChunkIndex.X * terrain->chunkSize,
               ChunkIndex.Y * terrain->chunkSize,
               ChunkIndex.Z * terrain->chunkSize,
                              terrain->chunkSize);
    
    sf::Packet Packet;
    Packet << "Take this chunk. It will be useful in times of need.";
    Packet << (int) Blocks.size();
    
    for (int i = 0; i < Blocks.size(); i++) // << (char)Blocks[i].block->Type 
        Packet << Blocks[i].pos << Blocks[i].sideLength;

    Client.Send(Packet);
}

std::map<sf::SocketTCP, Player> players();

// Launch a server and receive incoming messages
int main() {
    if (beater.Beat())
        std::cout << "Heartbeat successful." << std::endl;
    else
        std::cout << "Error while sending heartbeat!" << std::endl;
    
    std::cout << "Setting up terrain..." << std::endl;
    terrain = new Terrain(3, 0, 10,10,10, 25);
    terrain->Regenerate();
    terrain->SaveToFile("server.mcube");
    
    // Create a socket for listening to incoming connections
    sf::SocketTCP Listener;
    if (!Listener.Listen(Port))
        return EXIT_FAILURE;
    std::cout << "Listening to port " << Port << ", waiting for connections..." << std::endl;

    // Create a selector for handling several sockets (the listener + the socket associated to each client)
    sf::SelectorTCP Selector;

    // Add the listener
    Selector.Add(Listener);

    // Loop while... we close the program :)
    while (true)
    {
        // Get the sockets ready for reading
        unsigned int NbSockets = Selector.Wait();

        // We can read from each returned socket
        for (unsigned int i = 0; i < NbSockets; ++i)
        {
            // Get the current socket
            sf::SocketTCP Socket = Selector.GetSocketReady(i);

            if (Socket == Listener)
            {
                // If the listening socket is ready, it means that we can accept a new connection
                sf::IPAddress Address;
                sf::SocketTCP Client;
                Listener.Accept(Client, &Address);
                std::cout << "Client connected: " << Address << std::endl;
                
                sf::Packet Packet;
                Packet << "First, I have to let you in on this secret.";
                Packet << terrain->chunkSize;
                Client.Send(Packet);

                // Add it to the selector
                Selector.Add(Client);
            }
            else
            {
                // Else, it is a client socket so we can read the data he sent
                sf::Packet Packet;
                if (Socket.Receive(Packet) == sf::Socket::Done)
                {
                    // Extract the message and display it
                    std::string Message;
                    Packet >> Message;
                    
                    if (Message == "Terrain pl0z") {
                        Vector3 ChunkIndex;
                        Packet >> ChunkIndex;
                        sendTerrain(Socket, ChunkIndex);
                    } else
                        std::cout << "A client says: \"" << Message << "\"" << std::endl;
                }
                else
                {
                    // Error: we'd better remove the socket from the selector
                    std::cout << "Client disconnected" << std::endl;
                    Selector.Remove(Socket);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

