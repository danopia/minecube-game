#include <SFML/Graphics.hpp>

#include "mainmenu.h"
#include "game.h"

int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "MineCube");
    
    //App.UseVerticalSync(true);
    
    // Create TEH MENU
    MainMenu menu(&App);
    menu.Loop();
    
    // Create TEH GAME
    Game game(&App);
    game.Loop();
    
    return EXIT_SUCCESS;
}

