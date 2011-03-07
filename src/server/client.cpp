#include "server/client.h"

Client::Client(sf::SocketTCP *Socket, sf::IPAddress Address, int Number) : Socket(Socket), Address(Address), Number(Number) {
    sf::Packet Packet;
    Packet << (sf::Uint8) 2 << Number;
    Socket->Send(Packet);
}

