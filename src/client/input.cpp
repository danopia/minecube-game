// File adopted by Nick Markwell (duckinator)

#include "client/input.h"

InputHandler::InputHandler(Context *context) : context(context), InChat(false) {
    context->window->ShowMouseCursor(false);
}

bool inputWaiting;

bool fullscreen = false;
void InputHandler::handleEvent(sf::Event Event) {
    float time;

    // Close window : exit
    if (Event.Type == sf::Event::Closed)
        context->window->Close();
    
    // Handle text input for chat
    if (InChat && Event.Type == sf::Event::TextEntered) {

        // Ignore the first char (which is t)
        if (inputWaiting) {
            inputWaiting = false;
            return;
        }
        
        // Handle ASCII characters only
        if (Event.Text.Unicode == 8) { // TODO: use KeyPressed?
            if (context->hud->chatEntry.size() > 0)
                context->hud->chatEntry.erase(context->hud->chatEntry.size() - 1, 1);
        } else if (Event.Text.Unicode < 128) {
            context->hud->chatEntry += static_cast<char>(Event.Text.Unicode);
        }
    }
    
    // Handle enter to send message
    if (InChat && Event.Type == sf::Event::KeyPressed) {
        switch(Event.Key.Code) {
            // F11 : toggle fullscreen
            case sf::Key::F11:
                toggleFullscreen();
                break;
                
            // Escape key : cancel message
            case sf::Key::Escape:
                context->hud->chatEntry = "";
                InChat = false;
                return; // don't run the handlers from down there
                
            // Enter : send message
            case sf::Key::Return:
                context->socket->SendChat(context->hud->chatEntry);
                context->hud->chatEntry = "";
                InChat = false;
                return; // don't run the handlers from down there
                
        } 
    }
    
    if (!InChat && Event.Type == sf::Event::MouseButtonPressed && Event.MouseButton.Button == sf::Mouse::Left) {
        context->world->DestroyTarget(context->player);
    }

    // Otherwise....
    if (!InChat && Event.Type == sf::Event::KeyPressed) {
        switch(Event.Key.Code) {
            // Escape key : exit
            case sf::Key::Escape:
                context->inGame = false;
                break;
            // F11 : toggle fullscreen
            case sf::Key::F11:
                toggleFullscreen();
                break;
                
            // T : chat
            case sf::Key::T:
                context->hud->chatEntry = ""; // TODO: needed?
                InChat = true;
                inputWaiting = true;
                break;
                
            // W : forward
            case sf::Key::W:
                context->player->Forward = true;
                break;
            // S : backward
            case sf::Key::S:
                context->player->Backward = true;
                break;
            // A : left
            case sf::Key::A:
                context->player->Left = true;
                break;
            // D : right
            case sf::Key::D:
                context->player->Right = true;
                break;
            
            // Left Control, C : crouch
            case sf::Key::LControl:
            case sf::Key::C:
                MovementTimer.Reset();
                WasCrouching = context->player->Crouching;
                context->player->Crouching = true;
                break;
            
            // V : crawl
            case sf::Key::V:
                MovementTimer.Reset();
                WasCrawling = context->player->Crawling;
                context->player->Crawling = true;
                break;
            
            // Left Shift : run
            case sf::Key::LShift:
                MovementTimer.Reset();
                WasRunning = context->player->Running;
                context->player->Running = true;
                break;
            
            // Space : jump
            case sf::Key::Space:
                context->player->Jumping = true;
                break;
        }
    }

    if (Event.Type == sf::Event::KeyReleased) {
        time = MovementTimer.GetElapsedTime();

        switch(Event.Key.Code) {
            // W : forward
            case sf::Key::W:
                context->player->Forward = false;
                break;
            // S : backward
            case sf::Key::S:
                context->player->Backward = false;
                break;
            // A : left
            case sf::Key::A:
                context->player->Left = false;
                break;
            // D : right
            case sf::Key::D:
                context->player->Right = false;
                break;
            
            // Left Control, C : crouch
            case sf::Key::LControl:
            case sf::Key::C:
                if (time > 0.1)
                    context->player->Crouching = false;
                else
                    context->player->Crouching = !WasCrouching;
                break;
            
            // V : crawl
            case sf::Key::V:
                if (time > 0.1)
                    context->player->Crawling = false;
                else
                    context->player->Crawling = !WasCrawling;
                break;
            
            // Left Shift : run
            case sf::Key::LShift:
                if (time > 0.1)
                    context->player->Running = false;
                else
                    context->player->Running = !WasRunning;

            // Space : jump
            case sf::Key::Space:
                context->player->Jumping = false;
                break;
        }
    }

    // Resize event : adjust viewport
    if (Event.Type == sf::Event::Resized)
        glViewport(0, 0, Event.Size.Width, Event.Size.Height);
}

float mouseDeltaX, mouseDeltaY;

void InputHandler::handleEvents() {
    const sf::Input& Input = context->window->GetInput();
    
    // Handle other events
    sf::Event Event;
    while (context->window->GetEvent(Event))
    {
        handleEvent(Event);
    }
        
    // Rotate view based on mouse movement 
    mouseDeltaX = Input.GetMouseX() - 100; 
    mouseDeltaY = Input.GetMouseY() - 100;
    context->window->SetCursorPosition(100, 100);
    
    if (!(mouseDeltaX == -100 && mouseDeltaY == -100) && !(mouseDeltaX == 0 && mouseDeltaY == 0)) 
        context->player->ChangeRotation((mouseDeltaY/10), (mouseDeltaX/10));
}

void InputHandler::toggleFullscreen() {
    fullscreen = !fullscreen;
    context->window->Create(sf::VideoMode(800, 600, 32), "MineCube", (fullscreen ? sf::Style::Fullscreen : sf::Style::Resize|sf::Style::Close));
        
    context->renderer->InitGraphics();
    context->window->ShowMouseCursor(false);
}

