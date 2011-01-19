#pragma once
#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>

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

        Context *context;
        sf::String Text;
        sf::Shape Hair1, Hair2;
};

#endif
