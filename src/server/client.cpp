#include "server/client.h"

Client::Client(sf::SocketTCP *Socket, int Number) : Socket(Socket), Number(Number) {
    sf::Packet Packet;
    Packet << "Take a number" << Number;
    Socket->Send(Packet);
}

