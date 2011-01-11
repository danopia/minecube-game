#include <SFML/Graphics.hpp>

#include "client/mainmenu.h"

int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "MineCube");
    
    //App.UseVerticalSync(true);
    
    // Create TEH MENU
    MainMenu menu(&App);
    menu.Loop();
    
    return EXIT_SUCCESS;
}

