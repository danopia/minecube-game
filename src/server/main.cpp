#include <SFML/Network.hpp>
#include <iostream>

#include "common/terrain.h"
#include "common/player.h"

// Choose a random port for opening sockets (ports < 1024 are reserved)
const unsigned short Port = 28997;

Terrain *terrain;

void sendTerrain(sf::SocketTCP Client) {
    std::ifstream file("server.mcube", std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type size;
    char *memblock;
    
    size = file.tellg();
    memblock = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(memblock, size);
    file.close();

    sf::Packet Packet;
    Packet << std::string(memblock, size);
    Client.Send(Packet);

    delete[] memblock;
}

const int KeySize = 32;

void saveKey(std::string key) {
    if (key.length() != KeySize) return; // TODO: throw error
    
    std::ofstream file("key.txt");
    file << key;
}

// TODO: error handling
std::string readKey() {
    std::ifstream file("key.txt");

    if (!file) return ""; // No key is saved; it'll grab a new one
    
    char memblock[KeySize];
    file.read(memblock, KeySize);

    return std::string(memblock, KeySize);
}

sf::Http Http;
void sendBeat() {
    Http.SetHost("mc-heartbeat.heroku.com");

    // Prepare a request to beat the heart
    sf::Http::Request Request;
    Request.SetMethod(sf::Http::Request::Post);
    Request.SetBody("hey=thar");//"port=" << Port);
    Request.SetHttpVersion(1, 0);
    
    // Load the key (if any)
    std::string key = readKey();
    std::cout << "Key: " << key << std::endl;
    
    if (key == "")
        Request.SetURI("/beat");
    else
        Request.SetURI("/beat/" + key);

    // Send it and get the response returned by the server
    sf::Http::Response Page = Http.SendRequest(Request);

    if (Page.GetStatus() != 200) {
        std::cout << "Error while beating the heart. (HTTP " << Page.GetStatus() << ")" << std::endl;
        return;
    }

    // Display the response
    std::cout << "The heart has been beat." << std::endl;
    
    // Save the key
    saveKey(Page.GetBody());
}

std::map<sf::SocketTCP, Player> players();

// Launch a server and receive incoming messages
int main() {
    std::cout << "Beating..." << std::endl;
    sendBeat();
    
    std::cout << "Setting up terrain..." << std::endl;
    terrain = new Terrain(5, 0, 1,1,1, 50);
    terrain->Regenerate();
    terrain->SaveToFile("server.mcube");
    
    // Create a socket for listening to incoming connections
    sf::SocketTCP Listener;
    if (!Listener.Listen(Port))
        return EXIT_FAILURE;
    std::cout << "Listening to port " << Port << ", waiting for connections..." << std::endl;

    // Create a selector for handling several sockets (the listener + the socket associated to each client)
    sf::SelectorTCP Selector;

    // Add the listener
    Selector.Add(Listener);

    // Loop while... we close the program :)
    while (true)
    {
        // Get the sockets ready for reading
        unsigned int NbSockets = Selector.Wait();

        // We can read from each returned socket
        for (unsigned int i = 0; i < NbSockets; ++i)
        {
            // Get the current socket
            sf::SocketTCP Socket = Selector.GetSocketReady(i);

            if (Socket == Listener)
            {
                // If the listening socket is ready, it means that we can accept a new connection
                sf::IPAddress Address;
                sf::SocketTCP Client;
                Listener.Accept(Client, &Address);
                std::cout << "Client connected: " << Address << std::endl;

                // Add it to the selector
                Selector.Add(Client);
            }
            else
            {
                // Else, it is a client socket so we can read the data he sent
                sf::Packet Packet;
                if (Socket.Receive(Packet) == sf::Socket::Done)
                {
                    // Extract the message and display it
                    std::string Message;
                    Packet >> Message;
                    
                    if (Message == "Terrain pl0z")
                        sendTerrain(Socket);
                    else
                        std::cout << "A client says: \"" << Message << "\"" << std::endl;
                }
                else
                {
                    // Error: we'd better remove the socket from the selector
                    std::cout << "Client disconnected" << std::endl;
                    Selector.Remove(Socket);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

