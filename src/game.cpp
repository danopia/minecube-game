#include "game.h"

Game::Game(sf::RenderWindow* app) : app(app), player(5.f, Vector3(0.f, 0.f, 90.f), Vector3(5.f, 25.f, 60.f), "Foo"), terrain(5, 0, 1,1,1, 50), renderer(&terrain, &player), inputHandler(app, &player, &renderer) {}

void Game::Generate() {
    terrain.Regenerate();
}
void Game::Load(char* filename) {
    terrain.LoadFromFile(filename);
    file = filename;
}
void Game::Save() {
    if (file) Save(file);
}
void Game::Save(char* filename) {
    terrain.SaveToFile(filename);
}

void Game::Loop() {
    Running = true;
    
    // Set some stuff
    app->PreserveOpenGLStates(true);
    app->UseVerticalSync(false);
    
    char buf[10];
    float Framerate;
    
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
    }
}

