#include "server/heartbeat.h"

const int KeySize = 32;

bool Heartbeat::Beat() {
    sf::Http Http("mc-heartbeat.heroku.com");

    // Prepare a request to beat the heart
    sf::Http::Request Request;
    Request.SetMethod(sf::Http::Request::Post);
    Request.SetBody("hey=thar");//"port=" << Port);
    
    // Load the key (if any)
    std::string key = ReadKey();
    if (key == "")
        Request.SetURI("/beat");
    else
        Request.SetURI("/beat/" + key);

    // Send it and get the response returned by the server
    sf::Http::Response Page = Http.SendRequest(Request);

    if (Page.GetStatus() != 200) {
        std::cerr << "Error while beating the heart. (HTTP " << Page.GetStatus() << ")" << std::endl;
        return false;
    }
    
    // Save the key
    SaveKey(Page.GetBody());
    
    return true;
}

void Heartbeat::SaveKey(std::string key) {
    if (key.length() != KeySize) return; // TODO: throw error
    
    std::ofstream file("key.txt");
    file << key;
}

// TODO: error handling
std::string Heartbeat::ReadKey() {
    std::ifstream file("key.txt");

    if (!file) return ""; // No key is saved; it'll grab a new one
    
    char memblock[KeySize];
    file.read(memblock, KeySize);

    return std::string(memblock, KeySize);
}

