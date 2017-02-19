// File adopted by Nick Markwell (duckinator)

#include "client/input.h"

InputHandler::InputHandler(Context *context) : context(context), InChat(false) {
    context->window->setMouseCursorVisible(false);
}

bool inputWaiting;

bool fullscreen = false;
void InputHandler::handleEvent(sf::Event Event) {
    float time;

    // Close window : exit
    if (Event.type == sf::Event::Closed)
        context->inGame = false;

    // Handle text input for chat
    if (InChat && Event.type == sf::Event::TextEntered) {

        // Ignore the first char (which is t)
        if (inputWaiting) {
            inputWaiting = false;
            return;
        }

        // Handle ASCII characters only
        if (Event.text.unicode == 8) { // TODO: use KeyPressed?
            if (context->hud->chatEntry.size() > 0)
                context->hud->chatEntry.erase(context->hud->chatEntry.size() - 1, 1);
        } else if (Event.text.unicode < 128) {
            context->hud->chatEntry += static_cast<char>(Event.text.unicode);
        }
    }

    // Handle enter to send message
    if (InChat && Event.type == sf::Event::KeyPressed) {
        switch(Event.key.code) {
            // F11 : toggle fullscreen
            case sf::Keyboard::F11:
                toggleFullscreen();
                break;

            // Escape key : cancel message
            case sf::Keyboard::Escape:
                context->hud->chatEntry = "";
                InChat = false;
                return; // don't run the handlers from down there

            // Enter : send message
            case sf::Keyboard::Return:
                context->socket->SendChat(context->hud->chatEntry);
                context->hud->chatEntry = "";
                InChat = false;
                return; // don't run the handlers from down there

            default:
                return;
        }
    }

    if (!InChat && Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Left) {
        context->world->DestroyTarget(context->player);
    }

    if (!InChat && Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Right) {
        context->world->PlaceAboveTarget(context->player);
    }

    // Otherwise....
    if (!InChat && Event.type == sf::Event::KeyPressed) {
        switch(Event.key.code) {
            // Escape key : exit
            case sf::Keyboard::Escape:
                context->inGame = false;
                break;
            // F11 : toggle fullscreen
            case sf::Keyboard::F11:
                toggleFullscreen();
                break;

            // T : chat
            case sf::Keyboard::T:
                context->hud->chatEntry = ""; // TODO: needed?
                InChat = true;
                inputWaiting = true;
                break;

            // tab: userlist
            case sf::Keyboard::Tab:
                context->hud->ShowUsers = true;
                break;

            // W : forward
            case sf::Keyboard::W:
                context->player->Forward = true;
                break;
            // S : backward
            case sf::Keyboard::S:
                context->player->Backward = true;
                break;
            // A : left
            case sf::Keyboard::A:
                context->player->Left = true;
                break;
            // D : right
            case sf::Keyboard::D:
                context->player->Right = true;
                break;

            // Left Control, C : crouch
            case sf::Keyboard::LControl:
            case sf::Keyboard::C:
                MovementTimer.restart();
                WasCrouching = context->player->Crouching;
                context->player->Crouching = true;
                break;

            // V : crawl
            case sf::Keyboard::V:
                MovementTimer.restart();
                WasCrawling = context->player->Crawling;
                context->player->Crawling = true;
                break;

            // Left Shift : run
            case sf::Keyboard::LShift:
                MovementTimer.restart();
                WasRunning = context->player->Running;
                context->player->Running = true;
                break;

            // Space : jump
            case sf::Keyboard::Space:
                context->player->Jumping = true;
                break;

            default:
                return;
        }
    }

    if (Event.type == sf::Event::KeyReleased) {
        time = MovementTimer.getElapsedTime().asSeconds();

        switch(Event.key.code) {
            // tab: userlist
            case sf::Keyboard::Tab:
                context->hud->ShowUsers = false;
                break;

            // W : forward
            case sf::Keyboard::W:
                context->player->Forward = false;
                break;
            // S : backward
            case sf::Keyboard::S:
                context->player->Backward = false;
                break;
            // A : left
            case sf::Keyboard::A:
                context->player->Left = false;
                break;
            // D : right
            case sf::Keyboard::D:
                context->player->Right = false;
                break;

            // Left Control, C : crouch
            case sf::Keyboard::LControl:
            case sf::Keyboard::C:
                if (time > 0.1)
                    context->player->Crouching = false;
                else
                    context->player->Crouching = !WasCrouching;
                break;

            // V : crawl
            case sf::Keyboard::V:
                if (time > 0.1)
                    context->player->Crawling = false;
                else
                    context->player->Crawling = !WasCrawling;
                break;

            // Left Shift : run
            case sf::Keyboard::LShift:
                if (time > 0.1)
                    context->player->Running = false;
                else
                    context->player->Running = !WasRunning;

            // Space : jump
            case sf::Keyboard::Space:
                context->player->Jumping = false;
                break;

            default:
                return;
        }
    }

    // Resize event : adjust viewport
    if (Event.type == sf::Event::Resized)
        glViewport(0, 0, Event.size.width, Event.size.height);
}

float mouseDeltaX, mouseDeltaY;

void InputHandler::handleEvents() {
    // const sf::Input& Input = context->window->GetInput();

    // Handle other events
    sf::Event Event;
    while (context->window->pollEvent(Event))
    {
        handleEvent(Event);
    }

    // Rotate view based on mouse movement
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*context->window);
    mouseDeltaX = mousePosition.x - 100;
    mouseDeltaY = mousePosition.y - 100;

    sf::Vector2i newPosition(100.f, 100.f);
    sf::Mouse::setPosition(newPosition, *context->window);

    if (!(mouseDeltaX == -100 && mouseDeltaY == -100) && !(mouseDeltaX == 0 && mouseDeltaY == 0))
        context->player->ChangeRotation((mouseDeltaY/10), (mouseDeltaX/10));
}

void InputHandler::toggleFullscreen() {
    fullscreen = !fullscreen;
    context->window->create(sf::VideoMode(800, 600, 32), "MineCube", (fullscreen ? sf::Style::Fullscreen : sf::Style::Resize|sf::Style::Close));

    context->renderer->InitGraphics();
    context->window->setMouseCursorVisible(false);
}
