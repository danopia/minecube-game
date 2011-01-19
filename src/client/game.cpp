#include "client/game.h"

Game::Game(Context *context) : context(context) {
    context->world = new LazyWorld(context->socket->socket);
    context->player = new Player(5.f, Vector3(0.f, 0.f, 90.f), Vector3(5.f, 25.f, 49.f), "Foo");
    context->renderer = new Renderer(context);
    context->inputHandler = new InputHandler(context);
}

void Game::Loop() {
    context->inGame = true;
    
    // Set some stuff
    context->window->PreserveOpenGLStates(true);
    context->window->UseVerticalSync(false);
    
    char buf[10];
    float Framerate;
    
    sf::Clock LoaderTimer;

    sf::String Text;
    Text.SetFont(context->renderer->Font);
    Text.SetSize(context->renderer->Font.GetCharacterSize());
    
    sf::Shape Hair1 = sf::Shape::Line(-15, 0, 15, 0, 2.5, sf::Color(255, 255, 255, 127));
    sf::Shape Hair2 = sf::Shape::Line(0, -15, 0, 15, 2.5, sf::Color(255, 255, 255, 127));
    
    Hair1.SetPosition(context->window->GetWidth() / 2, context->window->GetHeight() / 2);
    Hair2.SetPosition(Hair1.GetPosition());

    // Start game loop
    while (context->window->IsOpened() && context->inGame)
    {
        Framerate = 1.f / context->window->GetFrameTime();
        
        // Handle input, network, and renderering
        context->inputHandler->handleEvents();
        context->socket->DoStep();
        context->renderer->render();
        
        // Draw FPS
        snprintf(buf, 10, "%.1f FPS", Framerate);
        //snprintf(buf, 10, "%f", player.Pos.Z);
        Text.SetText(buf);
        context->window->Draw(Text);
        
        // Draw crosshair
        context->window->Draw(Hair1);
        context->window->Draw(Hair2);

        // Finally, display rendered frame on screen
        context->window->Display();
        
        if (LoaderTimer.GetElapsedTime() > 1.f) {
            context->world->HandleRequests(context->player->Pos);
            LoaderTimer.Reset();
        }
    }
}

