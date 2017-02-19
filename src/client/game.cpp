#include "client/game.h"

Game::Game(Context *context) : context(context) {
    // TODO
    context->world = new Terrain(NULL);
    context->world->Storage = new SocketStorage(context->world, context);

    context->player = new Player(5.f, Vector3(0.f, 0.f, 90.f), Vector3(12.5f, 12.5f, 25.f), "Foo");
    context->renderer = new Renderer(context);
    context->hud = new HUD(context);
}

void Game::Loop() {
    context->inGame = true;

    // Set some stuff
    // TODO: context->window->PreserveOpenGLStates(true);
    context->window->setVerticalSyncEnabled(false);
    context->renderer->InitGraphics();

    context->inputHandler = new InputHandler(context);

    sf::Clock LoaderTimer;
    context->world->HandleRequests(context->player->Pos);

    // Start game loop
    while (context->window->isOpen() && context->inGame)
    {
        // Handle input, logic, and networking
        context->inputHandler->handleEvents();
        context->world->DoStep();
        context->world->CheckAim(context->player);
        context->socket->DoStep();

        // Render/display view and HUD
        context->renderer->render();
        context->hud->Draw();
        context->window->display();

        // Handle chunk loading (occasionally)
        if (LoaderTimer.getElapsedTime().asSeconds() > 1.f) {
            context->world->HandleRequests(context->player->Pos);
            LoaderTimer.restart();
        }
    }
}
