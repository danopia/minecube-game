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

void ServerList::Connect(std::string hostname, int port) {
    Context *con = new Context(context->window);
    con->socket = new Socket(con, port, hostname);
    
    // Set up a game and make sure it's ready
    Game game(con);
    WaitPage wait = WaitPage(con);
    RunSubpage(&wait);
    
    // Run the game
    game.Loop();

    // Close the socket
    con->socket->Close();
    
    // Pull back window (TODO: why does context get messed up if it's sent to the game?)
    //context->window = con->window;
}

void ServerList::ItemSelected(std::string Label) {
    std::stringstream ss(Label);
    
    std::string ip;
    std::getline(ss, ip, ':');
    
    std::string port;
    std::getline(ss, port, ':'); // should go to the end
    
    int iPort = atoi(port.c_str());
    
    Connect(ip, iPort);
    
    InitGraphics();
}

