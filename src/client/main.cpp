#include <SFML/Graphics.hpp>

#include "client/ui/mainmenu.h"
#include "client/context.h"

int main()
{
    // Create the main window and a context for it
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "MineCube");
    Context context(&App);
    
    // Create TEH MENU
    MainMenu menu(&context);
    menu.Loop();
    
    return EXIT_SUCCESS;
}

