#include "server/client.h"

Client::Client(sf::SocketTCP *Socket, sf::IPAddress Address, Server *Host, int Number) : Socket(Socket), Address(Address), Host(Host), Number(Number) {
    sf::Packet Packet;
    Packet << (sf::Uint8) 1;
    Packet << Host->terrain->chunkSize;
    Socket->Send(Packet);
    
    sf::Packet Packet2;
    Packet2 << (sf::Uint8) 2 << Number;
    Socket->Send(Packet2);
    
    Avatar = new Player();
    Avatar->Name = Address.ToString();
}

bool Client::handlePacket(sf::Packet &Packet) {
    sf::Uint8 Message;
    Packet >> Message;
    printf("Got packet: %i\n", Message);
    
    if (Message == 4) {
        Vector3 ChunkIndex;
        Packet >> ChunkIndex;
        sendTerrain(ChunkIndex);

    } else if (Message == 5) {
        Packet >> Avatar;
        
        sf::Packet Out;
        Out << (sf::Uint8) 6 << Number;
        Out << Avatar;
        Host->broadcast(Out);

    } else if (Message == 8) {
        std::string Line;
        Packet >> Line;
        Host->broadcastLog("<" + Address.ToString() + "> " + Line);

    } else
        printf("Got strange packet: %i\n", Message);
        //std::cout << "A client says: \"" << Message << "\"" << std::endl;
    
    return true;
}

void Client::sendTerrain(const Vector3 ChunkIndex) {
    Octree<Block*> Chunk = Host->terrain->GeneratedTerrain[ChunkIndex];
    
    if (!Chunk.hasChildren) { // TODO: This is a stupid check to see if the chunk was just created by the stupid map thing.
        sf::Packet Packet;
        Packet << (sf::Uint8) 7 << (int) 0;
        Socket->Send(Packet);
        
        return;
    }
    
    std::vector<PositionedBlock> Blocks;
    Host->listBlocks(&Blocks, Chunk,
               ChunkIndex.X * Host->terrain->chunkSize,
               ChunkIndex.Y * Host->terrain->chunkSize,
               ChunkIndex.Z * Host->terrain->chunkSize,
                              Host->terrain->chunkSize);
    
    sf::Packet Packet;
    Packet << (sf::Uint8) 7 << (int) Blocks.size();
    
    for (int i = 0; i < Blocks.size(); i++) // << (char)Blocks[i].block->Type 
        Packet << Blocks[i].pos << Blocks[i].sideLength;

    Socket->Send(Packet);
}


