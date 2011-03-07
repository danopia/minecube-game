#include "client/ui/mainmenu.h"

MainMenu::MainMenu(Context *context) : ButtonPage(context, "Choose your adventure", true) {
    Buttons.push_back("Start server");
    Buttons.push_back("Connect");
    Buttons.push_back("Options");
    Buttons.push_back("Exit");
}

void MainMenu::ItemSelected(std::string Label) {
    if (Label == "Connect") {
        ServerList page = ServerList(context);
        RunSubpage(&page);
    } else if (Label == "Options") {
        OptionsMenu page = OptionsMenu(context);
        RunSubpage(&page);
    } else if (Label == "Exit")
        Running = false;
}

