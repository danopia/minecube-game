#include "client/ui/mainmenu.h"

MainMenu::MainMenu(Context *context) : ButtonPage(context, "Choose your adventure", true) {
    Buttons.push_back("Start server");
    Buttons.push_back("Connect");
    Buttons.push_back("Options");
    Buttons.push_back("Exit");
}

// Create server list screen and show it
void startServerList(Context *context) {
    ServerList submenu(context);
    submenu.Loop();
}

void MainMenu::ItemSelected(std::string Label) {
    if (Label == "Connect")
        startServerList(context);
    else if (Label == "Exit")
        context->window->Close();
    
    InitGraphics();
}

