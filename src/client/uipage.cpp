#include "client/uipage.h"

sf::Clock UIPage::Clock;

UIPage::UIPage(sf::RenderWindow* App, std::string Subtitle, bool Background) : App(App), Subtitle(Subtitle), Background(Background), BG(64, 64), Sprite(BG), Current(0) {
    InitGraphics();
}

void UIPage::InitGraphics() {
    App->PreserveOpenGLStates(false);
    App->UseVerticalSync(true);
    App->ShowMouseCursor(true);
    
    // Load tiles texture
    if (!Tiles.LoadFromFile("data/tiles.png"))
        return;
    
    // Copy out the grass (first tile)
    BG.Copy(Tiles, 0, 0);
    BG.Bind();
    
    // Wrap it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Mainly done here for when you hit F11 or change window size
    Sprite.SetSubRect(sf::IntRect(0, 0, App->GetWidth(), App->GetHeight()));
    Sprite.SetColor(sf::Color(128, 128, 128));
    
    // Load fonts
    TitleFnt.LoadFromFile("data/Inconsolata.ttf", 90);
    ButtonFnt.LoadFromFile("data/Inconsolata.ttf", 30);
}

void UIPage::Loop() {
    sf::String Title("MineCube", TitleFnt, 90);
    Title.SetPosition((App->GetWidth() / 2) - (Title.GetRect().GetWidth() / 2), 20);

    
    sf::String STitle(Subtitle, ButtonFnt, 30);
    STitle.SetPosition((App->GetWidth() / 2) - (STitle.GetRect().GetWidth() / 2), 130);
    
    sf::String Label("Button", ButtonFnt, 30);
    sf::Shape Button = sf::Shape::Rectangle(-200, -20, 200, 20, sf::Color(127.f, 127.f, 127.f), 2, sf::Color::Black);

    float mouseX, mouseY;
    bool clicked, mightBeOver;
    sf::Event Event;
    
    // Start menu loop
    while (App->IsOpened())
    {
        mouseX = App->GetInput().GetMouseX();
        mouseY = App->GetInput().GetMouseY();
        clicked = false;
        
        // Handle mouse and keyboard stuff
        while (App->GetEvent(Event))
        {
            // Close window: exit
            if (Event.Type == sf::Event::Closed)
                App->Close();

            // Escape key: close menu
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                return;

            // Arrow keys...
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Up)) {
                if (Current <= 0) Current = Buttons.size();
                
                Current--;
            }

            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Down)) {
                Current++;
                
                if (Current >= Buttons.size())
                    Current = 0;
            }
            
            // Handle click
            if ((Event.Type == sf::Event::MouseButtonReleased) && (Event.MouseButton.Button == sf::Mouse::Left))
                clicked = true;

            // Handle enter
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Return))
                ItemSelected(Buttons[Current]);
        }
        
        // Set up the stage
        if (Background)
            DrawBackground();
        
        App->Draw(Title);
        App->Draw(STitle);
        
        Button.SetPosition(App->GetWidth() / 2, 150);
        
        mightBeOver = (mouseX > Button.GetPosition().x - 200) && (mouseX < Button.GetPosition().x + 200);
        
        for (int i = 0; i < Buttons.size(); i++) {
            Button.Move(0, 60);
            
            if (mightBeOver && (mouseY > Button.GetPosition().y - 20) && (mouseY < Button.GetPosition().y + 20)) {
                Current = i;
                
                if (clicked)
                    ItemSelected(Buttons[Current]);
            }
            
            if (i == Current)
                Button.SetColor(sf::Color(220.f, 220.f, 220.f, 180.f));
            else
                Button.SetColor(sf::Color(127.f, 127.f, 127.f, 180.f));
        
            Label.SetText(Buttons[i]);
            Label.SetPosition(Button.GetPosition().x - (Label.GetRect().GetWidth() / 2), Button.GetPosition().y - 20);
            
            App->Draw(Button);
            App->Draw(Label);
        }

        // Finally, display rendered frame on screen
        App->Display();
    }
}

//void UIPage::ItemSelected(std::string Label) {}

void UIPage::DrawBackground() {
    App->Draw(Sprite);
    
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
    
    glLoadIdentity();
}

