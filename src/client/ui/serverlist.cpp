#include "client/ui/serverlist.h"

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

ServerList::ServerList(Context *context) : ButtonPage(context, "Server List", true) {
    Buttons = FetchServers();
    Buttons.push_back("localhost:28997");
}

void multiplayer(Context *context, std::string hostname, int port) {
    context->socket = new Socket(context, port, hostname);
    
    // Load up a game and run it
    Game game(context);
    game.Loop();

    // Close the socket
    context->socket->Close();
}

void ServerList::ItemSelected(std::string Label) {
    std::stringstream ss(Label);
    
    std::string ip;
    std::getline(ss, ip, ':');
    
    std::string port;
    std::getline(ss, port, ':'); // should go to the end
    
    int iPort = atoi(port.c_str());
    
    multiplayer(context, ip, iPort);
    
    InitGraphics();
}

