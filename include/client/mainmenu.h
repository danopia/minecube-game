#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H

#include "client/uipage.h"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <cmath>
#include <iostream>
#include <cstdio>

#include "client/game.h"
#include "client/serverlist.h"

#ifdef _WIN32
#define snprintf sprintf_s
#endif

class MainMenu : public UIPage
{
    public:
        MainMenu(sf::RenderWindow *app);
        void ItemSelected(std::string Label);
};

#endif

