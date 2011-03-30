#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#include <SFML/Graphics.hpp>

#include "client/renderer.h"
#include "client/input.h"
#include "client/socket.h"
#include "client/ui/hud.h"
#include "common/player.h"
#include "common/terrain.h"

class Socket;
class Player;
class Terrain;
class Renderer;
class InputHandler;
class HUD;

class Context
{
    public:
        Context() : inGame(false) {}
        Context(sf::RenderWindow *window) : window(window), inGame(false) {}

        sf::RenderWindow *window;
        Socket *socket;
        Player *player;
        Terrain *world;
        Renderer *renderer;
        InputHandler *inputHandler;
        HUD *hud;
        
        bool inGame;
};

#endif
