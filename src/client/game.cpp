#include "client/game.h"

Game::Game(sf::RenderWindow* app, Socket *socket) : app(app), socket(socket), world(socket->socket), player(5.f, Vector3(0.f, 0.f, 90.f), Vector3(5.f, 25.f, 49.f), "Foo"), renderer(&world, &player), inputHandler(app, &player, &renderer) {
    socket->world = &world;
    socket->player = &player;
}

void Game::Loop() {
    Running = true;
    
    // Set some stuff
    app->PreserveOpenGLStates(true);
    app->UseVerticalSync(false);
    
    char buf[10];
    float Framerate;
    
    sf::Clock LoaderTimer;

    sf::String Text;
    Text.SetFont(renderer.Font);
    Text.SetSize(renderer.Font.GetCharacterSize());
    
    sf::Shape Hair1 = sf::Shape::Line(-15, 0, 15, 0, 2.5, sf::Color(255, 255, 255, 127));
    sf::Shape Hair2 = sf::Shape::Line(0, -15, 0, 15, 2.5, sf::Color(255, 255, 255, 127));
    
    Hair1.SetPosition(app->GetWidth() / 2, app->GetHeight() / 2);
    Hair2.SetPosition(Hair1.GetPosition());

    // Start game loop
    while (app->IsOpened() && Running)
    {
        Framerate = 1.f / app->GetFrameTime();
        
        // Handle mouse and keyboard stuff
        if (!inputHandler.handleEvents())
            Running = false;

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        //App.SetActive();

        // Draw entire scene
        renderer.render();
        
        // Draw FPS
        snprintf(buf, 10, "%.1f FPS", Framerate);
        //snprintf(buf, 10, "%f", player.Pos.Z);
        Text.SetText(buf);
        app->Draw(Text);
        
        // Draw crosshair
        app->Draw(Hair1);
        app->Draw(Hair2);

        // Finally, display rendered frame on screen
        app->Display();
        
        if (LoaderTimer.GetElapsedTime() > 1.f) {
            world.HandleRequests(player.Pos);
            LoaderTimer.Reset();
        }
        
        socket->DoStep();
    }
}

