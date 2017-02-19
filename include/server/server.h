#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <map>

#include "common/terrain.h"
#include "common/vector3.h"
#include "common/player.h"
#include "common/packets.h"
#include "server/heartbeat.h"
#include "server/filestorage.h"
#include "server/client.h"

class Client;

class Server
{
    public:
        Server();
        Server(unsigned short Port);

        void beat();

        void broadcast(sf::Packet &Packet);
        void broadcastExcept(const Client *Except, sf::Packet &Packet);
        void broadcastLog(const std::string &Line);

        bool Listen();
        void Loop();

        unsigned short Port;

        Terrain *terrain;
        Heartbeat beater;
        sf::Clock BeatTimer;

        int NextNumber;
        std::map<sf::TcpSocket*, Client*> clients;
        sf::TcpListener Listener;
        sf::SocketSelector Selector;
};

#endif
