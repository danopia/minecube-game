#include "client/mainmenu.h"

#include "client/game.h"

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
    Items.push_back("New game");
    Items.push_back("Continue");
    Items.push_back("Multiplayer");
    Items.push_back("Options");
    Items.push_back("Exit");
    
    int Current = 0;
    
    sf::Font TitleFnt;
    TitleFnt.LoadFromFile("data/Inconsolata.ttf", 90);
    
    sf::String Title("MineCube", TitleFnt, 90);
    Title.SetPosition((app->GetWidth() / 2) - (Title.GetRect().GetWidth() / 2), 30);


    sf::Shape Button = sf::Shape::Rectangle(-200, -20, 200, 20, sf::Color(127.f, 127.f, 127.f), 2, sf::Color::Black);
    
    sf::Font ButtonFnt;
    ButtonFnt.LoadFromFile("data/Inconsolata.ttf", 30);
    
    sf::String Label;
    Label.SetFont(ButtonFnt);
    Label.SetSize(30);
    
   
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
            Label.SetPosition(Button.GetPosition().x - (Label.GetRect().GetWidth() / 2), Button.GetPosition().y - 20);
            
            app->Draw(Button);
            app->Draw(Label);
        }
        
        clicked = false;

        // Finally, display rendered frame on screen
        app->Display();
    }
}

// Create TEH GAME
void startGame(sf::RenderWindow* app, bool loadprevious) {
    Game game(app);
    
    if (loadprevious)
        game.Load("terrain.bin");
    else {
        game.Generate();
        game.Save("terrain.bin");
    }
    
    game.Loop();
    game.Save();
}

void multiplayer(sf::RenderWindow* app) {
    /* Ask for server address
    sf::IPAddress ServerAddress;
    do
    {
        std::cout << "Type the address or name of the server to connect to : ";
        std::cin  >> ServerAddress;
    }
    while (!ServerAddress.IsValid());
    std::cin.ignore(10000, '\n');*/

    // Create a socket for exchanging data with the server
    sf::SocketTCP Socket;

    // Connect to the server
    if (Socket.Connect(28997, "home.danopia.net") != sf::Socket::Done)
        return;

    /* Send messages until we are disconnected
    bool Connected = true;
    while (Connected)
    {
        // Let the user write a message
        std::string Message;
        std::cout << "Say something to the server : ";
        std::getline(std::cin, Message);

        // Send it to the server
        sf::Packet Packet;
        Packet << Message;
        Connected = (Socket.Send(Packet) == sf::Socket::Done);
    }*/
    
    // Send it to the server
    sf::Packet Packet;
    Packet << "Ohai thar";
    Socket.Send(Packet);
    //Connected = (Socket.Send(Packet) == sf::Socket::Done);
    
    // Extract the terrain and save it
    Socket.Receive(Packet);
    std::string data;
    Packet >> data;
    std::ofstream out("client.mcube", std::ios::binary);
    out.write(data.c_str(), data.size());
    out.close();
    
    // Load up a game
    Game game(app);
    game.Load("client.mcube");
    game.Loop();
    game.Save();

    // Close the socket
    Socket.Close();
}

void MainMenu::ItemSelected(std::string Label) {
    if (Label == "New game")
        startGame(app, false);
    else if (Label == "Continue")
        startGame(app, true);
    else if (Label == "Multiplayer")
        multiplayer(app);
    else if (Label == "Exit")
        app->Close();
    
    app->ShowMouseCursor(true); // MainMenu needs a way to re-setup the graphics
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
    
    glLoadIdentity();
}

