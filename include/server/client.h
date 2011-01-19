#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>

#include "common/player.h"

class Client
{
    public:
        Client() : Socket(NULL), Number(0) {};
        Client(sf::SocketTCP *Socket, int Number) : Socket(Socket), Number(Number) {};
        
        sf::SocketTCP *Socket;
        int Number;
};

#endif

