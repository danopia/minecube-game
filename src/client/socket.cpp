#include "client/socket.h"

#include <stdio.h>

Socket::Socket(Context *context, int port, std::string hostname) : context(context), Number(0) {
    // Connect to the server
    sf::IpAddress host(hostname);
    Connected = (socket->connect(host, port) == sf::Socket::Done);

    socket->setBlocking(false);
}

Socket::Socket(Context *context, sf::TcpSocket *socket) : context(context), socket(socket), Number(0) {
    socket->setBlocking(false);
}

void Socket::SendChat(const std::string Line) {
    sf::Packet Packet;
    Packet << (sf::Uint8) 3 << Line;
    socket->send(Packet);
}

void Socket::SendBlock(sf::Uint8 type, Vector3 chunkIndex, Vector3 blockIndex) {
    sf::Packet Packet;
    Packet << (sf::Uint8) 9 << type << chunkIndex << blockIndex;
    socket->send(Packet);
}

void Socket::DoStep() {
    std::cout << "=== jkl;" << std::endl;
    
    sf::Packet In;

    while (socket->receive(In) == sf::Socket::Done) {
        sf::Uint8 command;
        In >> command;
        //printf("Got packet: %i\n", command);

        // TODO: switch to a switch
        if (command == 1) {
            In >> context->world->ChunkSize;
        } else if (command == 2) {
            In >> Number >> context->player->Name >> context->player;
            Players[Number] = context->player;

            context->world->HandleRequests(context->player->Pos); // Request new chunks immediately when connecting
        } else if (command == 3) {
            std::string Line;
            In >> Line;
            context->hud->Output(Line);
        } else if (command == 4) {
            // TODO!!! TODO!!!
            ((SocketStorage*) context->world->Storage)->ReadChunk(In);

            //int BlockCount;
            //Vector3 ChunkIndex; // TODO: remove from request list, add to loaded list
            //In >> ChunkIndex >> BlockCount;

            //Chunk chunk(ChunkIndex, 16);
            //chunk.FillWith(3);
            //context->world->LoadedChunks[ChunkIndex] = chunk;

            context->inGame = true; // Ready to play TEH GAME! ;D
        } else if (command == 5) {
            int who;
            In >> who;

            // RAH RAH RAH STL
            Player *player = (*((Players.insert(std::make_pair(who, new Player))).first)).second;

            if (who != Number) In >> player; // TODO: yea I had no idea what I was doing there...
        } else if (command == 6) {
            int PlayerCount;
            In >> PlayerCount;

            int who;
            std::string username;
            Player *player;

            Players.clear();
            for (int i = 0; i < PlayerCount; i++) {
                player = new Player();
                In >> who >> username >> player;
                player->Name = username;

                Players[who] = player;
            }

            Players[Number] = context->player;
        } else if (command == 7) {
            int who;
            std::string username;
            In >> who >> username;

            Player *player = new Player;
            In >> player;
            player->Name = username;
            Players[who] = player;

            context->hud->Output(username + " joined");
        } else if (command == 8) {
            int who;
            In >> who;

            // RAH RAH RAH STL
            Player *player = (*((Players.insert(std::make_pair(who, new Player))).first)).second;

            context->hud->Output(player->Name + " left");
            Players.erase(who);
        } else if (command == 9) {
            Vector3 chunk, block;
            sf::Uint8 type;
            In >> type >> chunk >> block;

            context->world->PlaceBlock(type, chunk, block);
        } else
            printf("Got strange packet: %i\n", command);
    }

    if (!context->inGame) return;

    if ((context->player->Dirty && (updateTimer.getElapsedTime().asSeconds() > 0.5f))
     || (updateTimer.getElapsedTime().asSeconds() > 5.f)) {
        sf::Packet Out;
        Out << (sf::Uint8) 5 << context->player;
        socket->send(Out);

        context->player->Dirty = false;
        updateTimer.restart();
    }

    float ElapsedTime = Clock.getElapsedTime().asSeconds();
    Clock.restart();

    for (std::map<int, Player*>::iterator player = Players.begin(); player != Players.end(); player++) {
        player->second->LocatedOn = NULL;
        context->world->CheckCollision(player->second);
        player->second->DoStep(ElapsedTime);
    }
}

void Socket::Close() {
    socket->disconnect();
}
