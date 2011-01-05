#include "input.h"

InputHandler::InputHandler(sf::Window* Window, Player* Player) : app(Window), player(Player) {
    app->ShowMouseCursor(false);
}

void InputHandler::handleEvent(sf::Event Event) {
    // Close window : exit
    if (Event.Type == sf::Event::Closed)
        app->Close();

    // Escape key : exit
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
        app->Close();

    // Resize event : adjust viewport
    if (Event.Type == sf::Event::Resized)
        glViewport(0, 0, Event.Size.Width, Event.Size.Height);
}

float ElapsedTime;
float mouseDeltaX, mouseDeltaY;

void InputHandler::handleEvents() {
    const sf::Input& Input = app->GetInput();
    
    ElapsedTime = Clock.GetElapsedTime();
    Clock.Reset();
    
    // Handle held keys
    if ((Input.IsKeyDown(sf::Key::S))) player->Forward(-ElapsedTime);
    if ((Input.IsKeyDown(sf::Key::W))) player->Forward( ElapsedTime);
    if ((Input.IsKeyDown(sf::Key::D))) player->Strafe(-ElapsedTime);
    if ((Input.IsKeyDown(sf::Key::A))) player->Strafe( ElapsedTime);
    if ((Input.IsKeyDown(sf::Key::Z))) player->Speed++;
    if ((Input.IsKeyDown(sf::Key::X))) player->Speed--;
        
    // Rotate view based on mouse movement 
    mouseDeltaX = Input.GetMouseX() - 100; 
    mouseDeltaY = Input.GetMouseY() - 100;
    app->SetCursorPosition(100, 100);
    
    if (!(mouseDeltaX == -100 && mouseDeltaY == -100) && !(mouseDeltaX == 0 && mouseDeltaY == 0)) 
        player->ChangeRotation((mouseDeltaY/10), (mouseDeltaX/10));
        
    // Handle other events
    sf::Event Event;
    while (app->GetEvent(Event))
    {
        handleEvent(Event);
    }
}

