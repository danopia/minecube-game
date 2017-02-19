#include "client/ui/hud.h"
#include <cstdio>

char buf[10];
float Framerate;

HUD::HUD(Context *context) : context(context), ShowUsers(false) {
    DebugText.setFont(context->renderer->Font);
    //DebugText.setCharacterSize(context->renderer->Font.getCharacterSize());

    BacklogText.setFont(context->renderer->Font);
    //BacklogText.setCharacterSize(context->renderer->Font.getCharacterSize());

    Hair1 = sf::RectangleShape(sf::Vector2f(30.f, 2.5));
    Hair1.setFillColor(sf::Color(255, 255, 255, 127));
    Hair1 = sf::RectangleShape(sf::Vector2f(2.5, 30));
    Hair1.setFillColor(sf::Color(255, 255, 255, 127));

    Hair1.setPosition(sf::Vector2f(context->window->getSize().x / 2, context->window->getSize().y / 2));
    Hair2.setPosition(Hair1.getPosition());

    ContentFnt.loadFromFile("data/Inconsolata.ttf"); // 30 size
};

void HUD::Output(std::string Line) {
    Backlog.push_back(Line);
    BacklogClocks.push_back(sf::Clock());
}

void HUD::Draw() {
    Framerate = 69.f; // 1.f / context->window->GetFrameTime();

    // Draw FPS
    snprintf(buf, 10, "%.1f FPS", Framerate);
    //snprintf(buf, 10, "%f", player.Pos.Z);
    DebugText.setString(buf);
    context->window->draw(DebugText);

    // Draw backlog
    BacklogText.setPosition(sf::Vector2f(0, 10));
    for (std::deque<std::string>::iterator line = Backlog.begin(); line != Backlog.end(); line++) {
        BacklogText.move(0, 20);
        BacklogText.setString(*line);
        context->window->draw(BacklogText);
    }

    // Expire old backlog
    int expired = 0;
    for (std::deque<sf::Clock>::iterator clock = BacklogClocks.begin(); clock != BacklogClocks.end(); clock++)
        if (clock->getElapsedTime().asSeconds() > 5.f)
            expired++;

    for (int i = 0; i < expired; i++) {
        Backlog.pop_front();
        BacklogClocks.pop_front();
    }

    // Display chat input as you type (dumb textbox)
    if (context->inputHandler->InChat) {
        BacklogText.setColor(sf::Color::Blue);
        BacklogText.move(0, 30);
        BacklogText.setString("> " + chatEntry);

        context->window->draw(BacklogText);

        BacklogText.setColor(sf::Color::White);
    }

    if (ShowUsers) {
        DrawUsers();
    } else {
        // Draw crosshair
        context->window->draw(Hair1);
        context->window->draw(Hair2);
    }
}

void HUD::DrawUsers() {
    sf::RectangleShape Dimmer;
    Dimmer.setSize(static_cast<sf::Vector2f>(context->window->getSize()));
    Dimmer.setFillColor(sf::Color(0, 0, 0, 128));
    Dimmer.setOutlineThickness(2.f);
    Dimmer.setOutlineColor(sf::Color::Black);
    context->window->draw(Dimmer);

    sf::RectangleShape Button;
    Button.setSize(sf::Vector2f(400, 40));
    Button.setPosition(context->window->getSize().x / 2 - 200, 150 - 40);
    Button.setFillColor(sf::Color(127.f, 127.f, 127.f));
    Button.setOutlineThickness(2.f);
    Button.setOutlineColor(sf::Color::Black);

    sf::Text Label("User List", ContentFnt, 30);
    Label.setPosition(Button.getPosition());
    Label.move(-(Label.getLocalBounds().width / 2), -20);
    context->window->draw(Label);

    for (std::map<int, Player*>::iterator player = context->socket->Players.begin(); player != context->socket->Players.end(); player++) {
        Button.move(0, 60);

        if (player->first == context->socket->Number)
            Button.setFillColor(sf::Color(220.f, 220.f, 220.f, 180.f));
        else
            Button.setFillColor(sf::Color(127.f, 127.f, 127.f, 180.f));

        Label.setString(player->second->Name);
        Label.setPosition(Button.getPosition());
        Label.move(-(Label.getLocalBounds().width / 2), -20);

        context->window->draw(Button);
        context->window->draw(Label);
    }
}
