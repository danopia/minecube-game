#include "server/server.h"

Server::Server() : Port(28997), NextNumber(1) {
    Regenerate();
};
Server::Server(unsigned short Port) : Port(Port), NextNumber(1) {
    Regenerate();
};

void Server::Regenerate() {
    std::cout << "Generating terrain... ";
    terrain = new Terrain(3, 3, 1,1,1, 16);
    terrain->Regenerate();
    std::cout << "done" << std::endl;
}

void Server::listBlocks(std::vector<PositionedBlock> *Blocks, const Octree<Block*> octree, const float x, const float y, const float z, const float size) {
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
    } else {
        Blocks->push_back(PositionedBlock(octree.value, Vector3(x, y, z), size));
    }
}

void Server::beat() {
    if (beater.Beat())
        std::cout << "Heartbeat successful." << std::endl;
    else
        std::cout << "Error while sending heartbeat!" << std::endl;
    
    BeatTimer.Reset();
}

void Server::broadcast(sf::Packet &Packet) {
    for (std::map<sf::SocketTCP, Client*>::iterator client = clients.begin(); client != clients.end(); client++)
        client->second->Socket.Send(Packet);
}

void Server::broadcastExcept(const Client *Except, sf::Packet &Packet) {
    for (std::map<sf::SocketTCP, Client*>::iterator client = clients.begin(); client != clients.end(); client++)
        if (client->second->Number != Except->Number)
            client->second->Socket.Send(Packet);
}

void Server::broadcastLog(const std::string &Line) {
    sf::Packet Packet;
    Packet << (sf::Uint8) 3 << Line;
    broadcast(Packet);
    
    std::cout << "[LOG] " << Line << std::endl;
}

bool Server::Listen() {
    if (!Listener.Listen(Port))
        return false;
    
    Selector.Add(Listener);
    std::cout << "Listening to port " << Port << std::endl;
}

void Server::Loop() {
    // Send heartbeat to say that server is up
    beat();
    
    std::cout << "Server ready." << std::endl;
    
    // Loop until never ;D
    while (true) {
    
        // Get the sockets ready for reading
        unsigned int NbSockets = Selector.Wait(60.f);

        // We can read from each returned socket
        for (unsigned int i = 0; i < NbSockets; ++i) {
            // Get the current socket
            sf::SocketTCP Socket = Selector.GetSocketReady(i);

            if (Socket == Listener) {
                // If the listening socket is ready, it means that we can accept a new connection
                Client *client = Client::Accept(Socket, this);
            } else {
                // Else, it is a client socket so we can read the data he sent
        
                Client *client;
                for (std::map<sf::SocketTCP, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
                    if (it->first == Socket)
                        client = it->second;

                sf::Packet Packet;
                if (Socket.Receive(Packet) == sf::Socket::Done) {
                    // Extract the message and display it
                    client->handlePacket(Packet);
                    
                } else {
                    // Error: we'd better remove the socket from the selector
                    Selector.Remove(Socket);
                    clients.erase(Socket);
                    
                    sf::Packet OutPacket;
                    OutPacket << (sf::Uint8) 8 << client->Number;
                    broadcast(OutPacket);
                    
                    std::cout << "Client disconnected: " << client->Address.ToString() << std::endl;
                }
            }
        }
        
        // Send heartbeat after at least 5 minutes
        if (BeatTimer.GetElapsedTime() > 300.f)
            beat();
    }
}

