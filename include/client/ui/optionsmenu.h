#pragma once
#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdio>

#include "client/context.h"
#include "client/ui/uipage.h"

class OptionsMenu : public UIPage
{
    public:
        OptionsMenu(Context *context, std::string Subtitle = "", bool Background = true);
        void Render();
        void HandleEvent(sf::Event &Event);
        void ItemChanged(std::string Label);
        
        std::vector<std::string> Options;

    protected:
        // index of highlighted button
        int Current;
};

#endif

