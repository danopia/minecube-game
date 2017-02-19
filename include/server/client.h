#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>

#include "common/terrain.h"
#include "common/player.h"
#include "common/vector3.h"
#include "server/server.h"

class Server;

class Client
{
    public:
        static Client *Accept(sf::TcpListener &Listener, Server *Host);

        Client(Server *Host);
        //Client(sf::TcpSocket *Socket, sf::IpAddress Address, Server *Host, int Number);

        void SendWelcome();
        void SendPlayerList();
        bool handlePacket(sf::Packet &Packet);
        void sendTerrain(const Vector3 ChunkIndex);

        sf::TcpSocket Socket;
        sf::IpAddress Address;
        Server *Host;
        Player *Avatar;

        int Number;
};

#endif
