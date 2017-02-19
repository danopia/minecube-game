#include "server/heartbeat.h"

const int KeySize = 32;

bool Heartbeat::Beat() {
    sf::Http Http("mc-heartbeat.herokuapp.com");

    // Prepare a request to beat the heart
    sf::Http::Request Request;
    Request.setMethod(sf::Http::Request::Post);
    Request.setBody("hey=thar");//"port=" << Port);

    // Load the key (if any)
    std::string key = ReadKey();
    if (key == "")
        Request.setUri("/beat");
    else
        Request.setUri("/beat/" + key);

    // Send it and get the response returned by the server
    sf::Http::Response Page = Http.sendRequest(Request);

    if (Page.getStatus() != 200) {
        std::cerr << "Error while beating the heart. (HTTP " << Page.getStatus() << ")" << std::endl;
        return false;
    }

    // Save the key
    SaveKey(Page.getBody());

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
