#pragma once
#ifndef PACKETS_H
#define PACKETS_H

#include <SFML/Network.hpp>
#include <string>

#include "common/vector3.h"
#include "common/player.h"

sf::Packet& operator <<(sf::Packet& Packet, const Vector3& V);
sf::Packet& operator >>(sf::Packet& Packet, Vector3& V);

sf::Packet& operator <<(sf::Packet& Packet, const Player *P);
sf::Packet& operator >>(sf::Packet& Packet, Player *P);

#endif

