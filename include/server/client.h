#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>

#include "common/player.h"
#include "common/vector3.h"
#include "server/server.h"

class Server;

class Client
{
    public:
        Client(sf::SocketTCP *Socket, sf::IPAddress Address, Server *Host, int Number);
        
        bool handlePacket(sf::Packet &Packet);
        void sendTerrain(const Vector3 ChunkIndex);
        
        sf::SocketTCP *Socket;
        sf::IPAddress Address;
        Server *Host;
        Player *Avatar;
        int Number;
};

#endif

