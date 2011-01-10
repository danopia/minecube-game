#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <cstdio>

#ifdef _WIN32
#define snprintf sprintf_s
#endif

class MainMenu
{
    public:
        MainMenu(sf::RenderWindow *app);
        void Loop();

        sf::RenderWindow *app;
};

#endif
