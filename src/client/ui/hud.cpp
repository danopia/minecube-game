#include "client/ui/hud.h"
#include <cstdio>

char buf[10];
float Framerate;

HUD::HUD(Context *context) : context(context), ShowUsers(false) {
    DebugText.SetFont(context->renderer->Font);
    DebugText.SetSize(context->renderer->Font.GetCharacterSize());
    
    BacklogText.SetFont(context->renderer->Font);
    BacklogText.SetSize(context->renderer->Font.GetCharacterSize());
    
    Hair1 = sf::Shape::Line(-15, 0, 15, 0, 2.5, sf::Color(255, 255, 255, 127));
    Hair2 = sf::Shape::Line(0, -15, 0, 15, 2.5, sf::Color(255, 255, 255, 127));
    
    Hair1.SetPosition(context->window->GetWidth() / 2, context->window->GetHeight() / 2);
    Hair2.SetPosition(Hair1.GetPosition());
    
    ContentFnt.LoadFromFile("data/Inconsolata.ttf", 30);
};

void HUD::Output(std::string Line) {
    Backlog.push_back(Line);
    BacklogClocks.push_back(sf::Clock());
}

void HUD::Draw() {
    Framerate = 1.f / context->window->GetFrameTime();
    
    // Draw FPS
    snprintf(buf, 10, "%.1f FPS", Framerate);
    //snprintf(buf, 10, "%f", player.Pos.Z);
    DebugText.SetText(buf);
    context->window->Draw(DebugText);
    
    // Draw backlog
    BacklogText.SetPosition(0, 10);
    for (std::deque<std::string>::iterator line = Backlog.begin(); line != Backlog.end(); line++) {
        BacklogText.Move(0, 20);
        BacklogText.SetText(*line);
        context->window->Draw(BacklogText);
    }
    
    // Expire old backlog
    int expired = 0;
    for (std::deque<sf::Clock>::iterator clock = BacklogClocks.begin(); clock != BacklogClocks.end(); clock++)
        if (clock->GetElapsedTime() > 5.f)
            expired++;
    
    for (int i = 0; i < expired; i++) {
        Backlog.pop_front();
        BacklogClocks.pop_front();
    }
    
    // Display chat input as you type (dumb textbox)
    if (context->inputHandler->InChat) {
        BacklogText.SetColor(sf::Color::Blue);
        BacklogText.Move(0, 30);
        BacklogText.SetText("> " + chatEntry);
        
        context->window->Draw(BacklogText);
        
        BacklogText.SetColor(sf::Color::White);
    }
    
    if (ShowUsers) {
        DrawUsers();
    } else {
        // Draw crosshair
        context->window->Draw(Hair1);
        context->window->Draw(Hair2);
    }
}

void HUD::DrawUsers() {
    sf::Shape Dimmer = sf::Shape::Rectangle(0, 0, context->window->GetWidth(), context->window->GetHeight(), sf::Color(0, 0, 0, 128));
    context->window->Draw(Dimmer);
    
    sf::Shape Button = sf::Shape::Rectangle(-200, -20, 200, 20, sf::Color(127.f, 127.f, 127.f), 2, sf::Color::Black);
    Button.SetPosition(context->window->GetWidth() / 2, 150);
    
    sf::String Label("User List", ContentFnt, 30);
    Label.SetPosition(Button.GetPosition().x - (Label.GetRect().GetWidth() / 2), Button.GetPosition().y - 20);
    context->window->Draw(Label);
    
    for (std::map<int, Player*>::iterator player = context->socket->Players.begin(); player != context->socket->Players.end(); player++) {
        Button.Move(0, 60);
        
        if (player->first == context->socket->Number)
            Button.SetColor(sf::Color(220.f, 220.f, 220.f, 180.f));
        else
            Button.SetColor(sf::Color(127.f, 127.f, 127.f, 180.f));
        
        Label.SetText(player->second->Name);
        Label.SetPosition(Button.GetPosition().x - (Label.GetRect().GetWidth() / 2), Button.GetPosition().y - 20);
        
        context->window->Draw(Button);
        context->window->Draw(Label);
    }
}

