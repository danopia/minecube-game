#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#include <SFML/Graphics.hpp>

#include "client/renderer.h"
#include "client/input.h"
#include "client/socket.h"
#include "common/player.h"
#include "common/lazyworld.h"

class Socket;
class Player;
class LazyWorld;
class Renderer;
class InputHandler;

class Context
{
    public:
        Context() : inGame(false) {}
        Context(sf::RenderWindow *window) : window(window), inGame(false) {}

        sf::RenderWindow *window;
        Socket *socket;
        Player *player;
        LazyWorld *world;
        Renderer *renderer;
        InputHandler *inputHandler;
        
        bool inGame;
};

#endif
