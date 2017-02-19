#include "client/ui/uipage.h"

sf::Clock UIPage::Clock;

UIPage::UIPage(Context *context, std::string Subtitle, bool Background) : context(context), Subtitle(Subtitle), Background(Background), Sprite(BG) {
    BG.create(64, 64);
    InitGraphics();
}

void UIPage::InitGraphics() {
    // TODO: context->window->PreserveOpenGLStates(false);
    context->window->setVerticalSyncEnabled(true);
    context->window->setMouseCursorVisible(true);

    // Load tiles texture
    if (!Tiles.loadFromFile("data/tiles.png"))
        return;

    // Copy out the grass (first tile)
    BG.loadFromImage(Tiles, sf::IntRect(0, 0, 64, 64));
    // BG.Bind(); TODO: BG isn't used?

    // Wrap it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Mainly done here for when you hit F11 or change window size
    Sprite.setTextureRect(sf::IntRect(0, 0, context->window->getSize().x, context->window->getSize().y));
    Sprite.setColor(sf::Color(128, 128, 128));

    // Load fonts
    TitleFnt.loadFromFile("data/Inconsolata.ttf"); // 90
    ContentFnt.loadFromFile("data/Inconsolata.ttf"); // 30
}

void UIPage::Loop() {
    Running = true;

    sf::Event Event;

    // Start menu loop
    while (Running && context->window->isOpen())
    {
        // Handle mouse and keyboard stuff
        while (context->window->pollEvent(Event))
            HandleEvent(Event);

        // Render and display menu
        Render();
        context->window->display();
    }

    Running = false;
}

void UIPage::HandleEvent(sf::Event &Event) {
    // Close window: exit
    if (Event.type == sf::Event::Closed) {
        //context->window->Close(); TODO: goes elsewhere
        Running = false;
        return;
    }

    // Escape key: close menu
    if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) {
        Running = false;
        return;
    }
}

void UIPage::Render() {
    sf::Text Title("MineCube", TitleFnt, 90);
    Title.setPosition(sf::Vector2f((context->window->getSize().x / 2) - (Title.getLocalBounds().width / 2), 20));

    sf::Text STitle(Subtitle, ContentFnt, 30);
    STitle.setPosition(sf::Vector2f((context->window->getSize().x / 2) - (STitle.getLocalBounds().width / 2), 130));

    // Set up the stage
    if (Background)
        DrawBackground();

    context->window->draw(Title);
    context->window->draw(STitle);
}

void UIPage::RunSubpage(UIPage *page) {
    page->Loop();
    InitGraphics();
}

void UIPage::DrawBackground() {
    context->window->draw(Sprite);

    return;

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

    glRotatef(Clock.getElapsedTime().asSeconds() * 60, 1.f, 0.f, 0.f);						// Rotate On The X Axis
    glRotatef(Clock.getElapsedTime().asSeconds() * 30, 0.f, 1.f, 0.f);						// Rotate On The Y Axis
    //glRotatef(Clock.getElapsedTime().asSeconds()*30, 0.0f,0.0f,1.0f);						// Rotate On The Z Axis

    sf::Texture::bind(&BG);

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
