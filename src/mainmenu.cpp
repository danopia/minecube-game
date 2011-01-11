#include "mainmenu.h"

#include "game.h"

MainMenu::MainMenu(sf::RenderWindow* app) : app(app) {}

//GLuint Texture2 = 0;

sf::Image Tiles;
sf::Image Grass(64, 64);
sf::Sprite Sprite(Grass);

sf::Clock Clock;

void MainMenu::Loop() {
    // Set some stuff
    app->PreserveOpenGLStates(false);
    app->UseVerticalSync(true);
    app->ShowMouseCursor(true);
    
    std::vector<std::string> Items;
    Items.push_back("Singleplayer");
    Items.push_back("Multiplayer");
    Items.push_back("Options");
    Items.push_back("Exit");
    
    int Current = 0;
    
    sf::String Title("MineCube");
    Title.SetFont(sf::Font::GetDefaultFont());
    Title.SetSize(50);
    Title.SetPosition((app->GetWidth() / 2) - (Title.GetRect().GetWidth() / 2), 50);


    sf::Shape Button = sf::Shape::Rectangle(-200, -20, 200, 20, sf::Color(127.f, 127.f, 127.f), 2, sf::Color::Black);
    
    sf::String Label;
    Label.SetFont(sf::Font::GetDefaultFont());
    
   
    // Load texture
    if (!Tiles.LoadFromFile("data/tiles.png"))
        return;
    
    Grass.Copy(Tiles, 0, 0);
    Grass.Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    Sprite.SetSubRect(sf::IntRect(0, 0, app->GetWidth(), app->GetHeight()));
    Sprite.SetColor(sf::Color(128, 128, 128));


    // Start menu loop
    while (app->IsOpened())
    {
        float mouseX = app->GetInput().GetMouseX();
        float mouseY = app->GetInput().GetMouseY();
        bool clicked = false;
        
        // Handle mouse and keyboard stuff
        sf::Event Event;
        while (app->GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                app->Close();

            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                app->Close();

            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Up)) {
                if (Current <= 0)
                    Current = Items.size();
                
                Current--;
            }

            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Down)) {
                Current++;
                
                if (Current >= Items.size())
                    Current = 0;
            }
            
            if ((Event.Type == sf::Event::MouseButtonReleased) && (Event.MouseButton.Button == sf::Mouse::Left))
                clicked = true;

            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Return))
                ItemSelected(Items[Current]);
        }
        
        // Set up the stage
        app->Draw(Sprite);
        drawBackground();
        app->Draw(Title);
        
        Button.SetPosition(app->GetWidth() / 2, 150);
        
        bool mightBeOver = (mouseX > Button.GetPosition().x - 200) && (mouseX < Button.GetPosition().x + 200);
        
        for (int i = 0; i < Items.size(); i++) {
            Button.Move(0, 60);
            
            if (mightBeOver && (mouseY > Button.GetPosition().y - 20) && (mouseY < Button.GetPosition().y + 20)) {
                Current = i;
                
                if (clicked) {
                    ItemSelected(Items[Current]);
                }
            }
            
            if (i == Current)
                Button.SetColor(sf::Color(220.f, 220.f, 220.f, 180.f));
            else
                Button.SetColor(sf::Color(127.f, 127.f, 127.f, 180.f));
        
            Label.SetText(Items[i]);
            Label.SetPosition(Button.GetPosition().x - (Label.GetRect().GetWidth() / 2), (Button.GetPosition().y - (Label.GetRect().GetHeight() / 2)));
            
            app->Draw(Button);
            app->Draw(Label);
        }
        
        clicked = false;

        // Finally, display rendered frame on screen
        app->Display();
    }
}

// Create TEH GAME
void startGame(sf::RenderWindow* app) {
    Game game(app);
    game.Loop();
    app->ShowMouseCursor(true); // MainMenu needs a way to re-setup the graphics
}

void MainMenu::ItemSelected(std::string Label) {
    if (Label == "Singleplayer")
        startGame(app);
    else if (Label == "Exit")
        app->Close();
}

void MainMenu::drawBackground() {
    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Apply some transformations
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.f, 0.f, -25.f);
    glScalef(10.f, 10.f, 10.f);
    
    glRotatef(Clock.GetElapsedTime() * 60, 1.f, 0.f, 0.f);						// Rotate On The X Axis
    glRotatef(Clock.GetElapsedTime() * 30, 0.f, 1.f, 0.f);						// Rotate On The Y Axis
    //glRotatef(Clock.GetElapsedTime()*30, 0.0f,0.0f,1.0f);						// Rotate On The Z Axis

    Tiles.Bind();
    
    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
        // Front Face
        glTexCoord2f(0.0f, 0.25f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
        glTexCoord2f(0.125f, 0.25f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
        glTexCoord2f(0.125f, 0.375f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 0.375f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
        // Back Face
        glTexCoord2f(0.125f, 0.25f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
        glTexCoord2f(0.125f, 0.375f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 0.375f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.25f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
        // Top Face
        glTexCoord2f(0.0f, 0.375f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.25f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
        glTexCoord2f(0.125f, 0.25f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
        glTexCoord2f(0.125f, 0.375f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
        // Bottom Face
        glTexCoord2f(0.125f, 0.375f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 0.375f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.25f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
        glTexCoord2f(0.125f, 0.25f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
        // Right face
        glTexCoord2f(0.125f, 0.25f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
        glTexCoord2f(0.125f, 0.375f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 0.375f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.25f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
        // Left Face
        glTexCoord2f(0.0f, 0.25f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
        glTexCoord2f(0.125f, 0.25f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
        glTexCoord2f(0.125f, 0.375f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 0.375f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glEnd();
    
    glDisable(GL_DEPTH_TEST);
}

