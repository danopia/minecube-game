#include "server/client.h"

Client *Client::Accept(sf::SocketTCP &Listener, Server *Host) {
    Client *client = new Client(Host);
    
    Listener.Accept(client->Socket, &client->Address);
    
    client->Avatar = new Player(5.f, Vector3(0.f, 0.f, 90.f), Vector3(8.f, 8.f, 16.f), "");
    client->Avatar->Name = client->Address.ToString();
    
    std::cout << "Client connected: " << client->Address.ToString() << std::endl;
    
    sf::Packet Packet;
    Packet << (sf::Uint8) 7 << client->Number << client->Avatar->Name << client->Avatar;
    Host->broadcast(Packet);
    
    Host->clients[client->Socket] = client;
    Host->Selector.Add(client->Socket);
    
    client->SendWelcome();
    
    return client;
}

Client::Client(Server *Host) : Host(Host) {
    Number = Host->NextNumber++;
}

Client::Client(sf::SocketTCP Socket, sf::IPAddress Address, Server *Host, int Number) : Socket(Socket), Address(Address), Host(Host), Number(Number) {
    Avatar = new Player();
    Avatar->Name = Address.ToString();
    
    SendWelcome();
}

void Client::SendWelcome() {
    sf::Packet Packet;
    Packet << (sf::Uint8) 1;
    Packet << Host->terrain->chunkSize;
    Socket.Send(Packet);
    
    sf::Packet Packet2;
    Packet2 << (sf::Uint8) 2 << Number << Avatar->Name << Avatar;
    Socket.Send(Packet2);
    
    SendPlayerList();
}

bool Client::handlePacket(sf::Packet &Packet) {
    sf::Uint8 Message;
    Packet >> Message;
    //printf("Got packet: %i\n", Message);
    
    if (Message == 3) {
        std::string Line;
        Packet >> Line;
        Host->broadcastLog("<" + Address.ToString() + "> " + Line);

    } else if (Message == 4) {
        Vector3 ChunkIndex;
        Packet >> ChunkIndex;
        sendTerrain(ChunkIndex);

    } else if (Message == 5) {
        Packet >> Avatar;
        
        sf::Packet Out;
        Out << (sf::Uint8) 5 << Number;
        Out << Avatar;
        Host->broadcast(Out);

    } else if (Message == 6) {
        SendPlayerList();
        
    } else
        printf("Got strange packet: %i\n", Message);
        //std::cout << "A client says: \"" << Message << "\"" << std::endl;
    
    return true;
}

void Client::sendTerrain(const Vector3 ChunkIndex) {
    std::map<Vector3, Octree<Block*> >::iterator it = Host->terrain->GeneratedTerrain.begin();
    it = Host->terrain->GeneratedTerrain.find(ChunkIndex);

    // No chunk? Just say it's empty
    if (it == Host->terrain->GeneratedTerrain.end()) {
        sf::Packet Packet;
        Packet << (sf::Uint8) 4 << ChunkIndex << (int) 0;
        Socket.Send(Packet);
        
        return;
    }
    
    Octree<Block*> Chunk = it->second;
    
    std::vector<PositionedBlock> Blocks;
    Host->listBlocks(&Blocks, Chunk, 0, 0, 0, Host->terrain->chunkSize);
    
    sf::Packet Packet;
    Packet << (sf::Uint8) 4 << ChunkIndex << (int) Blocks.size();
    
    for (int i = 0; i < Blocks.size(); i++) {
        Packet << (sf::Uint8) Blocks[i].block->Type << Blocks[i].pos;
    }

    Socket.Send(Packet);
}

void Client::SendPlayerList() {
    sf::Packet Packet;
    Packet << (sf::Uint8) 6;
    Packet << (int) Host->clients.size();
    
    for (std::map<sf::SocketTCP, Client*>::iterator client = Host->clients.begin(); client != Host->clients.end(); client++) {
        Packet << client->second->Number << client->second->Avatar->Name << client->second->Avatar;
    }

    Socket.Send(Packet);
}

