#include "server/client.h"

Client::Client(sf::SocketTCP *Socket, sf::IPAddress Address, int Number) : Socket(Socket), Address(Address), Number(Number) {
    sf::Packet Packet;
    Packet << "Take a number" << Number;
    Socket->Send(Packet);
}

