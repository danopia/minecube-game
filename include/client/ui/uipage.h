#pragma once
#ifndef UIPAGE_H
#define UIPAGE_H

#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <cstdio>

#include "client/context.h"

class UIPage
{
    public:
        UIPage(Context *context, std::string Subtitle = "", bool Background = true);
        void InitGraphics();
        void Loop();
        virtual void Render();
        virtual void HandleEvent(sf::Event &Event);

        void RunSubpage(UIPage *page);

        std::string Subtitle;
        bool Background;

        bool Running;

    protected:
        void DrawBackground();

        Context *context;

        sf::Image Tiles;
        sf::Texture BG;
        sf::Sprite Sprite;

        sf::Font TitleFnt, ContentFnt;

        // static so that the background cube is smooth across pages
        static sf::Clock Clock;
};

#endif
