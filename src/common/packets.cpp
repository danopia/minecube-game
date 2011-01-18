#include "common/packets.h"

sf::Packet& operator <<(sf::Packet& Packet, const Vector3& V)
{
    return Packet << V.X << V.Y << V.Z;
}

sf::Packet& operator >>(sf::Packet& Packet, Vector3& V)
{
    return Packet >> V.X >> V.Y >> V.Z;
}


sf::Packet& operator <<(sf::Packet& Packet, const Player *P)
{
    return Packet // << "have a player"
        << P->Name << P->Speed << P->Jumping
        << P->Backward << P->Backward << P->Left << P->Right
        << P->Walking << P->Crouching << P->Crawling << P->Running
        << P->Pos << P->Rotation << P->Hitbox;
}

sf::Packet& operator >>(sf::Packet& Packet, Player *P)
{
    return Packet
        >> P->Name >> P->Speed >> P->Jumping
        >> P->Backward >> P->Backward >> P->Left >> P->Right
        >> P->Walking >> P->Crouching >> P->Crawling >> P->Running
        >> P->Pos >> P->Rotation >> P->Hitbox;
}

