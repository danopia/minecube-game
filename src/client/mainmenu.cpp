#include "client/mainmenu.h"

#include "client/game.h"

MainMenu::MainMenu(sf::RenderWindow* App) : UIPage(App, true) {
    Buttons.push_back("New game");
    Buttons.push_back("Continue");
    Buttons.push_back("Multiplayer");
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

void multiplayer(sf::RenderWindow* App) {
    /* Ask for server address
    sf::IPAddress ServerAddress;
    do
    {
        std::cout << "Type the address or name of the server to connect to : ";
        std::cin  >> ServerAddress;
    }
    while (!ServerAddress.IsValid());
    std::cin.ignore(10000, '\n');*/

    // Create a socket for exchanging data with the server
    sf::SocketTCP Socket;

    // Connect to the server
    if (Socket.Connect(28997, "home.danopia.net") != sf::Socket::Done)
        return;

    /* Send messages until we are disconnected
    bool Connected = true;
    while (Connected)
    {
        // Let the user write a message
        std::string Message;
        std::cout << "Say something to the server : ";
        std::getline(std::cin, Message);

        // Send it to the server
        sf::Packet Packet;
        Packet << Message;
        Connected = (Socket.Send(Packet) == sf::Socket::Done);
    }*/
    
    // Send it to the server
    sf::Packet Packet;
    Packet << "Terrain pl0z";
    Socket.Send(Packet);
    //Connected = (Socket.Send(Packet) == sf::Socket::Done);
    
    // Extract the terrain and save it
    Socket.Receive(Packet);
    std::string data;
    Packet >> data;
    std::ofstream out("client.mcube", std::ios::binary);
    out.write(data.c_str(), data.size());
    out.close();
    
    // Load up a game
    Game game(App);
    game.Load("client.mcube");
    game.Loop();
    game.Save();

    // Close the socket
    Socket.Close();
}

void MainMenu::ItemSelected(std::string Label) {
    if (Label == "New game")
        startGame(App, false);
    else if (Label == "Continue")
        startGame(App, true);
    else if (Label == "Multiplayer")
        multiplayer(App);
    else if (Label == "Exit")
        App->Close();
    
    InitGraphics();
}

