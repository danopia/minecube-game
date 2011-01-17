#include "client/serverlist.h"

std::string doHTTP() {
    using namespace sf;
    
    Http Client("mc-heartbeat.heroku.com");

    // Prepare a request to beat the heart
    Http::Request Request;
    Request.SetURI("/servers.txt");

    // Send it and get the response returned by the server
    Http::Response Page = Client.SendRequest(Request);

    if (Page.GetStatus() != 200) {
        std::cerr << "Error while grabbing the server list. (HTTP " << Page.GetStatus() << ")" << std::endl;
        return "";
    }
    
    return Page.GetBody();
}

std::vector<std::string> FetchServers() {
    using namespace std;
    
    string raw = doHTTP();
    vector<string> combos;
    
    istringstream iss(raw);
    copy(istream_iterator<string>(iss),
        istream_iterator<string>(),
        back_inserter<vector<string> >(combos));
    
    return combos;
}

ServerList::ServerList(sf::RenderWindow* App) : UIPage(App, "Server List", true) {
    Buttons = FetchServers();
    Buttons.push_back("localhost:28997");
}

void multiplayer(sf::RenderWindow* App, std::string hostname, int port) {
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
    if (Socket.Connect(port, hostname) != sf::Socket::Done)
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
    
    // Load up a game
    Game game(App, &Socket);
    
    // Grab the secret (the chunk size)
    sf::Packet Packet;
    Socket.Receive(Packet);
    std::string command;
    Packet >> command;
    if (command == "First, I have to let you in on this secret.")
        Packet >> game.world.ChunkSize;
    
    // Run the game
    game.Loop();

    // Close the socket
    Socket.Close();
}

void ServerList::ItemSelected(std::string Label) {
    std::stringstream ss(Label);
    
    std::string ip;
    std::getline(ss, ip, ':');
    
    std::string port;
    std::getline(ss, port, ':'); // should go to the end
    
    int iPort = atoi(port.c_str());
    
    multiplayer(App, ip, iPort);
    
    InitGraphics();
}

