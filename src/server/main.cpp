#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <map>

#include "common/terrain.h"
#include "common/vector3.h"
#include "common/octree.h"
#include "common/player.h"
#include "common/packets.h"
#include "server/heartbeat.h"
#include "server/client.h"

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
    Octree<Block*> Chunk = terrain->GeneratedTerrain[ChunkIndex];
    
    if (!Chunk.hasChildren) { // TODO: This is a stupid check to see if the chunk was just created by the stupid map thing.
        sf::Packet Packet;
        Packet << (sf::Uint8) 7 << (int) 0;
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
    Packet << (sf::Uint8) 7 << (int) Blocks.size();
    
    for (int i = 0; i < Blocks.size(); i++) // << (char)Blocks[i].block->Type 
        Packet << Blocks[i].pos << Blocks[i].sideLength;

    Client.Send(Packet);
}

sf::Clock BeatTimer;
void beat() {
    if (beater.Beat())
        std::cout << "Heartbeat successful." << std::endl;
    else
        std::cout << "Error while sending heartbeat!" << std::endl;
    
    BeatTimer.Reset();
}

int NextNumber = 1;
std::map<sf::SocketTCP, Client> clients;

void broadcast(sf::Packet &Packet) {
    for (std::map<sf::SocketTCP, Client>::iterator client = clients.begin(); client != clients.end(); client++)
        client->second.Socket->Send(Packet);
}

void broadcastExcept(const Client &Except, sf::Packet &Packet) {
    for (std::map<sf::SocketTCP, Client>::iterator client = clients.begin(); client != clients.end(); client++)
        if (client->second.Number != Except.Number)
            client->second.Socket->Send(Packet);
}

void broadcastLog(const std::string &Line) {
    sf::Packet Packet;
    Packet << (sf::Uint8) 3 << Line;
    broadcast(Packet);
    
    std::cout << "[LOG] " << Line << std::endl;
}

void handlePacket(Client &client, const sf::Uint8 Message, sf::Packet &Packet) {
    //printf("Got packet: %i\n", Message);
    if (Message == 4) {
        Vector3 ChunkIndex;
        Packet >> ChunkIndex;
        sendTerrain(*client.Socket, ChunkIndex);

    } else if (Message == 5) {
        Player player;
        Packet >> &player;
        
        sf::Packet Out;
        Out << (sf::Uint8) 6 << client.Number;
        Out << &player;
        broadcast(Out);

    } else if (Message == 8) {
        std::string Line;
        Packet >> Line;
        broadcastLog("<" + client.Address.ToString() + "> " + Line);

    } else
        printf("Got strange packet: %i\n", Message);
        //std::cout << "A client says: \"" << Message << "\"" << std::endl;
}

// Launch a server and receive incoming messages
int main() {
    std::cout << "Setting up terrain... ";
    terrain = new Terrain(3, 3, 5,5,1, 25);
    terrain->Regenerate();
    terrain->SaveToFile("server.mcube");
    std::cout << "done" << std::endl;
    
    // Create a socket for listening to incoming connections
    sf::SocketTCP Listener;
    if (!Listener.Listen(Port))
        return EXIT_FAILURE;
    std::cout << "Listening to port " << Port << std::endl;
    
    // Send heartbeat now that server is up
    beat();
    
    std::cout << "Ready." << std::endl;

    // Create a selector for handling several sockets (the listener + the socket associated to each client)
    sf::SelectorTCP Selector;

    // Add the listener
    Selector.Add(Listener);

    // Loop while... we close the program :)
    while (true) {
    
        // Get the sockets ready for reading
        unsigned int NbSockets = Selector.Wait(60.f);

        // We can read from each returned socket
        for (unsigned int i = 0; i < NbSockets; ++i) {
            // Get the current socket
            sf::SocketTCP Socket = Selector.GetSocketReady(i);

            if (Socket == Listener) {
                // If the listening socket is ready, it means that we can accept a new connection
                // Using pointers until this is in it's own function
                sf::IPAddress *Address = new sf::IPAddress;
                sf::SocketTCP *Newcomer = new sf::SocketTCP;
                Listener.Accept(*Newcomer, Address);
                
                broadcastLog("Client connected: " + (*Address).ToString());
                
                sf::Packet Packet;
                Packet << (sf::Uint8) 1;
                Packet << terrain->chunkSize;
                Newcomer->Send(Packet);

                // Add it to the selector
                Selector.Add(*Newcomer);
                
                clients[*Newcomer] = Client(Newcomer, *Address, NextNumber++);
            } else {
                // Else, it is a client socket so we can read the data he sent
        
                Client client;
                for (std::map<sf::SocketTCP, Client>::iterator it = clients.begin(); it != clients.end(); it++)
                    if (it->first == Socket)
                        client = it->second;

                sf::Packet Packet;
                if (Socket.Receive(Packet) == sf::Socket::Done) {
                    // Extract the message and display it
                    sf::Uint8 Message;
                    Packet >> Message;
                    handlePacket(client, Message, Packet);
                    
                } else {
                    // Error: we'd better remove the socket from the selector
                    Selector.Remove(Socket);
                    clients.erase(Socket);
                    
                    broadcastLog("Client disconnected: " + client.Address.ToString());
                }
            }
        }
        
        // Send heartbeat after at least 5 minutes
        if (BeatTimer.GetElapsedTime() > 300.f)
            beat();
    }

    return EXIT_SUCCESS;
}

