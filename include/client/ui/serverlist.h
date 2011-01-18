#pragma once
#ifndef SERVERLIST_H
#define SERVERLIST_H

#include <SFML/Network.hpp>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "client/ui/uipage.h"
#include "client/game.h"
#include "client/socket.h"

class ServerList : public UIPage
{
    public:
        ServerList(sf::RenderWindow *app);
        void ItemSelected(std::string Label);
};

#endif

