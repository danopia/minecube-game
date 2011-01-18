#pragma once
#ifndef UIPAGE_H
#define UIPAGE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdio>

class UIPage
{
    public:
        UIPage(sf::RenderWindow *App, std::string Subtitle = "", bool Background = true);
        void InitGraphics();
        void Loop();
        virtual void ItemSelected(std::string Label) {};
        
        // TODO: another class for button pages
        std::vector<std::string> Buttons;
        
        std::string Subtitle;
        bool Background;

    protected:
        void DrawBackground();
        
        sf::RenderWindow *App;
        
        sf::Image Tiles;
        sf::Image BG;
        sf::Sprite Sprite;
        
        sf::Font TitleFnt;
        sf::Font ButtonFnt;
        
        // index of highlighted button
        int Current;

        // static so that the background cube is smooth across pages
        static sf::Clock Clock;
};

#endif

