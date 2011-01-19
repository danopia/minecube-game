#include "client/ui/hud.h"

char buf[10];
float Framerate;

HUD::HUD(Context *context) : context(context) {
    DebugText.SetFont(context->renderer->Font);
    DebugText.SetSize(context->renderer->Font.GetCharacterSize());
    
    BacklogText.SetFont(context->renderer->Font);
    BacklogText.SetSize(context->renderer->Font.GetCharacterSize());
    
    Hair1 = sf::Shape::Line(-15, 0, 15, 0, 2.5, sf::Color(255, 255, 255, 127));
    Hair2 = sf::Shape::Line(0, -15, 0, 15, 2.5, sf::Color(255, 255, 255, 127));
    
    Hair1.SetPosition(context->window->GetWidth() / 2, context->window->GetHeight() / 2);
    Hair2.SetPosition(Hair1.GetPosition());
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
        BacklogText.Move(0, 30);
        BacklogText.SetText(chatEntry);
        context->window->Draw(BacklogText);
    }
    
    // Draw crosshair
    context->window->Draw(Hair1);
    context->window->Draw(Hair2);
}

