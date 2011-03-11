#include "client/socket.h"

#include <stdio.h>

Socket::Socket(Context *context, int port, std::string hostname) : context(context), Number(0) {
    // Connect to the server
    Connected = (socket.Connect(port, hostname) == sf::Socket::Done);
    
    socket.SetBlocking(false);
}

Socket::Socket(Context *context, sf::SocketTCP socket) : context(context), socket(socket), Number(0) {
    socket.SetBlocking(false);
}

void Socket::SendChat(const std::string Line) {
    sf::Packet Packet;
    Packet << (sf::Uint8) 8 << Line;
    socket.Send(Packet);
}

void Socket::DoStep() {
    sf::Packet In;
    
    while (socket.Receive(In) == sf::Socket::Done) {
        sf::Uint8 command;
        In >> command;
        //printf("Got packet: %i\n", command);
        
        if (command == 1)
            In >> context->world->ChunkSize;
        else if (command == 7) {
            context->hud->Output("Got a chunk");
            context->world->LoadChunk(In);
        } else if (command == 2) {
            In >> Number;
            Players[Number] = context->player;
        } else if (command == 6) {
            int who;
            In >> who;
            
            // RAH RAH RAH STL
            Player *player = (*((Players.insert(std::make_pair(who, new Player))).first)).second;
            
            if (who != Number) In >> player; // TODO: yea I had no idea what I was doing there...
        } else if (command == 3) {
            std::string Line;
            In >> Line;
            context->hud->Output(Line);
        } else
            printf("Got strange packet: %i\n", command);
    }

    if ((context->player->Dirty && (updateTimer.GetElapsedTime() > 0.5f))
     || (updateTimer.GetElapsedTime() > 5.f)) {
        sf::Packet Out;
        Out << (sf::Uint8) 5 << context->player;
        socket.Send(Out);
        
        context->player->Dirty = false;
        updateTimer.Reset();
    }
    
    float ElapsedTime = Clock.GetElapsedTime();
    Clock.Reset();
    
    for (std::map<int, Player*>::iterator player = Players.begin(); player != Players.end(); player++) {
        player->second->LocatedOn = NULL;
        context->world->CheckCollision(player->second);
        player->second->DoStep(ElapsedTime);
    }
}

void Socket::Close() {
    socket.Close();
}
