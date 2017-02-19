#include "server/server.h"

Server::Server() : Port(28997), NextNumber(1) {
    terrain = new Terrain(NULL);
    terrain->Storage = new FileStorage(terrain);
};

Server::Server(unsigned short Port) : Port(Port), NextNumber(1) {
    terrain = new Terrain(NULL);
    terrain->Storage = new FileStorage(terrain);
};


void Server::beat() {
    if (beater.Beat())
        std::cout << "Heartbeat successful." << std::endl;
    else
        std::cout << "Error while sending heartbeat!" << std::endl;

    BeatTimer.restart();
}

void Server::broadcast(sf::Packet &Packet) {
    for (std::list<Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
        Client* client = *it;
        client->Socket.send(Packet);
    }
}

void Server::broadcastExcept(const Client *Except, sf::Packet &Packet) {
    for (std::list<Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
        Client* client = *it;
        if (client->Number != Except->Number)
            client->Socket.send(Packet);
  }
}

void Server::broadcastLog(const std::string &Line) {
    sf::Packet Packet;
    Packet << (sf::Uint8) 3 << Line;
    broadcast(Packet);

    std::cout << "[LOG] " << Line << std::endl;
}

bool Server::Listen() {
    std::cout << "Starting server..." << std::endl;

    if (Listener.listen(Port) != sf::Socket::Done) {
        std::cout << "Unable to listen on port " << Port << std::endl;
        return false;
    }

    Selector.add(Listener);
    std::cout << "Listening to port " << Port << std::endl;
    return true;
}

void Server::Loop() {
    // Send heartbeat to say that server is up
    beat();

    std::cout << "Server ready." << std::endl;

    // Loop until never ;D
    while (true) {

        // Get the sockets ready for reading
        if (Selector.wait(sf::seconds(60.f))) {

            if (Selector.isReady(Listener)) {
                // If the listening socket is ready, it means that we can accept a new connection
                Client *client = Client::Accept(Listener, this);
            }

            for (std::list<Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
                Client *client = *it;

                if (Selector.isReady(client->Socket)) {
                    // Read the data the client sent

                    sf::Packet Packet;
                    if (client->Socket.receive(Packet) == sf::Socket::Done) {
                        // Extract the message and display it
                        client->handlePacket(Packet);

                    } else {
                        // Error: we'd better remove the socket from the selector
                        Selector.remove(client->Socket);
                        clients.erase(it);

                        sf::Packet OutPacket;
                        OutPacket << (sf::Uint8) 8 << client->Number;
                        broadcast(OutPacket);

                        std::cout << "Client disconnected: " << client->Address.toString() << std::endl;
                    }
                }
            }
        }

        // Send heartbeat after at least 5 minutes
        if (BeatTimer.getElapsedTime().asSeconds() > 300.f)
            beat();
    }
}
