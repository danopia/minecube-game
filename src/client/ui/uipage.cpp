#include "client/ui/uipage.h"

sf::Clock UIPage::Clock;

UIPage::UIPage(Context *context, std::string Subtitle, bool Background) : context(context), Subtitle(Subtitle), Background(Background), BG(64, 64), Sprite(BG) {
    InitGraphics();
}

void UIPage::InitGraphics() {
    context->window->PreserveOpenGLStates(false);
    context->window->UseVerticalSync(true);
    context->window->ShowMouseCursor(true);
    
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
    Sprite.SetSubRect(sf::IntRect(0, 0, context->window->GetWidth(), context->window->GetHeight()));
    Sprite.SetColor(sf::Color(128, 128, 128));
    
    // Load fonts
    TitleFnt.LoadFromFile("data/Inconsolata.ttf", 90);
    ContentFnt.LoadFromFile("data/Inconsolata.ttf", 30);
}

void UIPage::Loop() {
    Running = true;
    
    sf::Event Event;
    
    // Start menu loop
    while (Running && context->window->IsOpened())
    {
        // Handle mouse and keyboard stuff
        while (context->window->GetEvent(Event))
        {
            HandleEvent(Event);
        }
        
        // Render menu
        Render();

        // Display rendered frame on screen
        context->window->Display();
    }
    
    Running = false;
}

void UIPage::HandleEvent(sf::Event &Event) {
    // Close window: exit
    if (Event.Type == sf::Event::Closed) {
        context->window->Close();
        Running = false;
        return;
    }

    // Escape key: close menu
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape)) {
        Running = false;
        return;
    }
}

void UIPage::Render() {
    sf::String Title("MineCube", TitleFnt, 90);
    Title.SetPosition((context->window->GetWidth() / 2) - (Title.GetRect().GetWidth() / 2), 20);

    sf::String STitle(Subtitle, ContentFnt, 30);
    STitle.SetPosition((context->window->GetWidth() / 2) - (STitle.GetRect().GetWidth() / 2), 130);

    // Set up the stage
    if (Background)
        DrawBackground();
    
    context->window->Draw(Title);
    context->window->Draw(STitle);
}

void UIPage::RunSubpage(UIPage *page) {
    page->Loop();
    InitGraphics();
}

void UIPage::DrawBackground() {
    context->window->Draw(Sprite);
    
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

