#include <SFML/Network.hpp>
#include <iostream>

#include "common/terrain.h"

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

// Launch a server and receive incoming messages
int main() {
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
                
                sendTerrain(Client);
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

