#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

#include<SDL/SDL.h>

#include "player.h"

class InputHandler {
public:
    InputHandler(Player *initplayer, int initlastTick);
    void handleEvents();
private:
    int lastTick;
    Player *player;
    SDL_Event current_event;
    void handleKeyUp();
    void handleKeyDown();
    void handleMouseMove();
    void handleMouseClick();
};

#endif
