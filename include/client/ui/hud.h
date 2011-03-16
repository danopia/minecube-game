#pragma once
#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <deque>
#include <string>

#include "client/socket.h"
#include "client/context.h"

#ifdef _WIN32
#define snprintf sprintf_s
#endif

class Context;

class HUD
{
    public:
        HUD(Context *context);
        
        void Draw();
        void Output(std::string Line);
        
        void DrawUsers();
        bool ShowUsers;
        sf::Font ContentFnt;

        Context *context;
        sf::String DebugText, BacklogText;
        sf::Shape Hair1, Hair2;
        
        std::deque<std::string> Backlog;
        std::deque<sf::Clock> BacklogClocks;
        
        std::string chatEntry;
};

#endif
