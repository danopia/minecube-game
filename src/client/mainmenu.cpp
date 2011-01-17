#include "client/mainmenu.h"

MainMenu::MainMenu(sf::RenderWindow* App) : UIPage(App, "Choose your adventure", true) {
    Buttons.push_back("Start server");
    Buttons.push_back("Connect");
    Buttons.push_back("Options");
    Buttons.push_back("Exit");
}

// Create server list screen and show it
void startServerList(sf::RenderWindow* App) {
    ServerList submenu(App);
    submenu.Loop();
}

void MainMenu::ItemSelected(std::string Label) {
    if (Label == "Connect")
        startServerList(App);
    else if (Label == "Exit")
        App->Close();
    
    InitGraphics();
}

