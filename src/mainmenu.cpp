#include "mainmenu.h"

MainMenu::MainMenu(sf::RenderWindow* app) : app(app) {}

void MainMenu::Loop() {
    // Set some stuff
    app->PreserveOpenGLStates(true);
    
    //char buf[10];
    //float Framerate;
    
    sf::String Text;
    Text.SetFont(sf::Font::GetDefaultFont());
    Text.SetPosition((app->GetWidth() / 2) - 65, 50);
    Text.SetText("MineCube");

    sf::Shape Button1 = sf::Shape::Rectangle(-200, -20, 200, 20, sf::Color::Black, 2, sf::Color::White);
    Button1.SetPosition(app->GetWidth() / 2, app->GetHeight() / 2);

    // Start menu loop
    while (app->IsOpened())
    {
        //Framerate = 1.f / app->GetFrameTime();
        
        // Handle mouse and keyboard stuff
        sf::Event Event;
        while (app->GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                app->Close();

            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                app->Close();

            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Return))
                return;

            // Resize event : adjust viewport
            if (Event.Type == sf::Event::Resized)
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);
        }
        
        // Clear the screen (fill it with black color)
        app->Clear();

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        //App.SetActive();

        // Draw entire scene
        //renderer.render();
        
        // Draw FPS
        //snprintf(buf, 10, "%.1f FPS", Framerate);
        //snprintf(buf, 10, "%f", player.Pos.Z);
        app->Draw(Text);
        
        // Draw crosshair
        app->Draw(Button1);
        //app->Draw(Hair2);

        // Finally, display rendered frame on screen
        app->Display();
    }
}

