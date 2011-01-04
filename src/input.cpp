#include "input.h"
#include <iostream>

InputHandler::InputHandler(Player *initplayer, int initlastTick) : player(initplayer), lastTick(initlastTick) {}

void InputHandler::handleEvents() 
{
    while(SDL_PollEvent(&current_event))
    {
        switch(current_event.type)
        {
        case SDL_KEYDOWN:
            handleKeyDown();
            break;
        case SDL_KEYUP:
            handleKeyUp();
            break;
        case SDL_MOUSEMOTION:
            handleMouseMove();
            break;
        case SDL_MOUSEBUTTONDOWN:
            handleMouseClick();
            break;
        }
    }
    lastTick = SDL_GetTicks();
    return;
}

void InputHandler::handleKeyDown()
{
    switch(current_event.key.keysym.sym)
    {
    case SDLK_LEFT:
        std::cout << "LEFT" << std::endl;
        player->Strafe(SDL_GetTicks() - lastTick);
        break;
    case SDLK_RIGHT:
        std::cout << "RIGHT" << std::endl;
        player->Strafe(SDL_GetTicks() - lastTick);

    case SDLK_ESCAPE:
        SDL_Quit();
    }
}

void InputHandler::handleKeyUp()
{}

void InputHandler::handleMouseClick()
{}

void InputHandler::handleMouseMove()
{}

