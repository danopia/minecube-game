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
            context->world->LoadChunk(In);
            
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
