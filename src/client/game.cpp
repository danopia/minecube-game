#include "client/game.h"

Game::Game(Context *context) : context(context) {
    context->world = new LazyWorld(context->socket->socket);
    context->player = new Player(5.f, Vector3(0.f, 0.f, 90.f), Vector3(12.5f, 12.5f, 25.f), "Foo");
    context->renderer = new Renderer(context);
    context->inputHandler = new InputHandler(context);
    context->hud = new HUD(context);
}

void Game::Loop() {
    context->inGame = true;
    
    // Set some stuff
    context->window->PreserveOpenGLStates(true);
    context->window->UseVerticalSync(false);
    
    sf::Clock LoaderTimer;
    context->world->HandleRequests(context->player->Pos);

    // Start game loop
    while (context->window->IsOpened() && context->inGame)
    {
        // Handle input, logic, and networking
        context->inputHandler->handleEvents();
        context->world->DoStep();
        context->socket->DoStep();
        
        // Render/display view and HUD
        context->renderer->render();
        context->hud->Draw();
        context->window->Display();
        
        // Handle chunk loading (occasionally)
        if (LoaderTimer.GetElapsedTime() > 1.f) {
            context->world->HandleRequests(context->player->Pos);
            LoaderTimer.Reset();
        }
    }
}

