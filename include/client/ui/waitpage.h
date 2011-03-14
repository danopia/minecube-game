#pragma once
#ifndef WAITPAGE_H
#define WAITPAGE_H

#include <SFML/Graphics.hpp>

#include "client/context.h"
#include "client/ui/uipage.h"

class WaitPage : public UIPage
{
    public:
        WaitPage(Context *context) : UIPage(context, "Please wait...", true) {};
        void Render();
};

#endif

