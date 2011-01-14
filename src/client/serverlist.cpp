#include "client/serverlist.h"

ServerList::ServerList(sf::RenderWindow* App) : UIPage(App, true) {
    Buttons.push_back("localhost");
    Buttons.push_back("danopia");
    Buttons.push_back("duckinator");
    Buttons.push_back("somewhere else...");
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

void ServerList::ItemSelected(std::string Label) {
    multiplayer(App);
    
    InitGraphics();
}

