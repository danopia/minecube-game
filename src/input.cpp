#include "input.h"

InputHandler::InputHandler(sf::Window* Window, Player* Player, Renderer* Renderer) : app(Window), player(Player), renderer(Renderer) {
    app->ShowMouseCursor(false);
}

bool fullscreen = false;
void InputHandler::handleEvent(sf::Event Event) {
    float time;

    // Close window : exit
    if (Event.Type == sf::Event::Closed)
        app->Close();

    if (Event.Type == sf::Event::KeyPressed) {
        switch(Event.Key.Code) {
            // Escape key : exit
            case sf::Key::Escape:
                app->Close();
                break;
            // F5 : regenerate terrain
            case sf::Key::F5:
                renderer->terrain.Regenerate();
                break;
            // F11 : toggle fullscreen
            case sf::Key::F11:
                toggleFullscreen();
                break;
                
            // W : forward
            case sf::Key::W:
                player->Forward = true;
                break;
            // S : backward
            case sf::Key::S:
                player->Backward = true;
                break;
            // A : left
            case sf::Key::A:
                player->Left = true;
                break;
            // D : right
            case sf::Key::D:
                player->Right = true;
                break;
            
            // Left Control, C : crouch
            case sf::Key::LControl:
            case sf::Key::C:
                MovementTimer.Reset();
                player->Crouching = true;
                break;
            
            // V : crawl
            case sf::Key::V:
                MovementTimer.Reset();
                player->Crawling = true;
                break;
            
            // Left Shift : run
            case sf::Key::LShift:
                MovementTimer.Reset();
                player->Running = true;
                break;
            
            // Space : jump
            case sf::Key::Space:
                player->Jump();
                break;
        }
    }

    if (Event.Type == sf::Event::KeyReleased) {
        time = MovementTimer.GetElapsedTime();
        std::cout << "Crouch/Crawl/Run time: " << time << std::endl;
        switch(Event.Key.Code) {
            // W : forward
            case sf::Key::W:
                player->Forward = false;
                break;
            // S : backward
            case sf::Key::S:
                player->Backward = false;
                break;
            // A : left
            case sf::Key::A:
                player->Left = false;
                break;
            // D : right
            case sf::Key::D:
                player->Right = false;
                break;
            
            // Left Control, C : crouch
            case sf::Key::LControl:
            case sf::Key::C:
                if (time > 0.2)
                    player->Crouching = false;
                else
                    player->toggleCrouch();
                break;
            
            // V : crawl
            case sf::Key::V:
                if (time > 0.2)
                    player->Crawling = false;
                else
                    player->toggleCrawl();
                break;
            
            // Left Shift : run
            case sf::Key::LShift:
                if (time > 0.2)
                    player->Running = false;
                else
                    player->toggleRun();
        }
    }

    // Resize event : adjust viewport
    if (Event.Type == sf::Event::Resized)
        glViewport(0, 0, Event.Size.Width, Event.Size.Height);
}

float ElapsedTime;
float mouseDeltaX, mouseDeltaY;

void InputHandler::handleEvents() {
    const sf::Input& Input = app->GetInput();
    
    // Constant movement speed
    ElapsedTime = Clock.GetElapsedTime();
    Clock.Reset();
        
    // Handle other events
    sf::Event Event;
    while (app->GetEvent(Event))
    {
        handleEvent(Event);
    }
        
    // Rotate view based on mouse movement 
    mouseDeltaX = Input.GetMouseX() - 100; 
    mouseDeltaY = Input.GetMouseY() - 100;
    app->SetCursorPosition(100, 100);
    
    if (!(mouseDeltaX == -100 && mouseDeltaY == -100) && !(mouseDeltaX == 0 && mouseDeltaY == 0)) 
        player->ChangeRotation((mouseDeltaY/10), (mouseDeltaX/10));
        
    player->DoStep(ElapsedTime);
}

void InputHandler::toggleFullscreen() {
    fullscreen = !fullscreen;
    app->Create(sf::VideoMode(800, 600, 32), "MineCube", (fullscreen ? sf::Style::Fullscreen : sf::Style::Resize|sf::Style::Close));
        
    renderer->InitGraphics();
    app->ShowMouseCursor(false);
}
