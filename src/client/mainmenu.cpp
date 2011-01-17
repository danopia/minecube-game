#include "client/mainmenu.h"

MainMenu::MainMenu(sf::RenderWindow* App) : UIPage(App, "Choose your adventure", true) {
    //Buttons.push_back("New game");
    //Buttons.push_back("Continue");
    Buttons.push_back("Connect");
    Buttons.push_back("Options");
    Buttons.push_back("Exit");
}

// Create TEH GAME
void startGame(sf::RenderWindow* App, bool loadprevious) {
    Game game(App);
    
    if (loadprevious)
        game.Load("terrain.bin");
    else {
        game.Generate();
        game.Save("terrain.bin");
    }
    
    game.Loop();
    game.Save();
}

// Create server list screen and show it
void startServerList(sf::RenderWindow* App) {
    ServerList submenu(App);
    submenu.Loop();
}


void MainMenu::ItemSelected(std::string Label) {
    if (Label == "New game")
        startGame(App, false);
    else if (Label == "Continue")
        startGame(App, true);
    else if (Label == "Multiplayer")
        startServerList(App);
    else if (Label == "Exit")
        App->Close();
    
    InitGraphics();
}

