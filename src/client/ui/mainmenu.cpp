#include "client/ui/mainmenu.h"

MainMenu::MainMenu(Context *context) : ButtonPage(context, "Choose your adventure", true) {
    Buttons.push_back("Start server");
    Buttons.push_back("Connect");
    Buttons.push_back("Options");
    Buttons.push_back("Exit");
}

void MainMenu::ItemSelected(std::string Label) {
    if (Label == "Connect")
        RunSubpage(new ServerList(context));
    else if (Label == "Options")
        RunSubpage(new OptionsMenu(context));
    else if (Label == "Exit")
        Running = false;
}

