#include "server/client.h"

Client *Client::Accept(sf::TcpListener &Listener, Server *Host) {
    Client *client = new Client(Host);

    Listener.accept(*client->Socket);
    client->Address = client->Socket->getRemoteAddress();

    client->Avatar = new Player(5.f, Vector3(0.f, 0.f, 90.f), Vector3(8.f, 8.f, 15.9f), "");
    client->Avatar->Name = client->Address.toString();

    std::cout << "Client connected: " << client->Address.toString() << std::endl;

    sf::Packet Packet;
    Packet << (sf::Uint8) 7 << client->Number << client->Avatar->Name << client->Avatar;
    Host->broadcast(Packet);

    Host->clients[client->Socket] = client;
    Host->Selector.add(*client->Socket);

    client->SendWelcome();

    return client;
}

Client::Client(Server *Host) : Host(Host) {
    Number = Host->NextNumber++;
}

Client::Client(sf::TcpSocket *Socket, sf::IpAddress Address, Server *Host, int Number) : Socket(Socket), Address(Address), Host(Host), Number(Number) {
    Avatar = new Player();
    Avatar->Name = Address.toString();

    SendWelcome();
}

void Client::SendWelcome() {
    sf::Packet Packet;
    Packet << (sf::Uint8) 1;
    Packet << Host->terrain->ChunkSize;
    Socket->send(Packet);

    sf::Packet Packet2;
    Packet2 << (sf::Uint8) 2 << Number << Avatar->Name << Avatar;
    Socket->send(Packet2);

    SendPlayerList();
}

bool Client::handlePacket(sf::Packet &Packet) {
    sf::Uint8 Message;
    Packet >> Message;
    //printf("Got packet: %i\n", Message);

    if (Message == 3) {
        std::string Line;
        Packet >> Line;
        Host->broadcastLog("<" + Address.toString() + "> " + Line);

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

    } else if (Message == 9) {
        Vector3 chunk, block;
        sf::Uint8 type;
        Packet >> type >> chunk >> block;

        sf::Packet Out;
        Out << (sf::Uint8) 9 << type << chunk << block;
        Host->broadcast(Out);

        Host->terrain->PlaceBlock(type, chunk, block);

    } else
        printf("Got strange packet: %i\n", Message);
        //std::cout << "A client says: \"" << Message << "\"" << std::endl;

    return true;
}

void Client::sendTerrain(const Vector3 ChunkIndex) {
    Chunk *chunk = Host->terrain->GetChunk(ChunkIndex);

    sf::Packet Packet;
    Packet << (sf::Uint8) 4 << ChunkIndex << (int) chunk->Blocks.size();

    for (std::map<Vector3, Block*>::iterator it = chunk->Blocks.begin(); it != chunk->Blocks.end(); it++) {
        Packet << (sf::Uint8) it->second->Type << it->first;
    }

    Socket->send(Packet);
}

void Client::SendPlayerList() {
    sf::Packet Packet;
    Packet << (sf::Uint8) 6;
    Packet << (int) Host->clients.size();

    for (std::map<sf::TcpSocket*, Client*>::iterator client = Host->clients.begin(); client != Host->clients.end(); client++) {
        Packet << client->second->Number << client->second->Avatar->Name << client->second->Avatar;
    }

    Socket->send(Packet);
}
