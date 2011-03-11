#include "server/server.h"

// Launch a server and receive incoming messages
int main() {
    Server instance;

    if (!instance.Listen())
        return EXIT_FAILURE;
    
    instance.Loop();

    return EXIT_SUCCESS;
}

