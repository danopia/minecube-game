#pragma once
#ifndef BUTTONPAGE_H
#define BUTTONPAGE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdio>

#include "client/context.h"
#include "client/ui/uipage.h"

class ButtonPage : public UIPage
{
    public:
        ButtonPage(Context *context, std::string Subtitle = "", bool Background = true);
        void Render();
        void HandleEvent(sf::Event &Event);
        virtual void ItemSelected(std::string Label) {};
        
        std::vector<std::string> Buttons;

    protected:
        // index of highlighted button
        int Current;
};

#endif

