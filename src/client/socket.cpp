#include "client/socket.h"

#include <stdio.h>

Socket::Socket(sf::SocketTCP *socket) : socket(socket), Number(0) {
    socket->SetBlocking(false);
}

void Socket::DoStep() {
    sf::Packet In;
    
    if (socket->Receive(In) == sf::Socket::Done) {
        std::string command;
        In >> command;
        
        if (command == "First, I have to let you in on this secret.")
            In >> world->ChunkSize;
        else if (command == "Take this chunk. It will be useful in times of need.")
            world->LoadChunk(In);
        else if (command == "Take a number")
            In >> Number;
        else
            printf("Got strange packet: %s\n", command.c_str());
    }

    if ((player->Dirty && (updateTimer.GetElapsedTime() > 0.5f))
     || (updateTimer.GetElapsedTime() > 5.f)) {
        sf::Packet Out;
        Out << "Move me or ELSE!" << player;
        socket->Send(Out);
        
        player->Dirty = false;
        updateTimer.Reset();
    }
}

