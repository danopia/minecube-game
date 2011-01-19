#include "client/ui/hud.h"

char buf[10];
float Framerate;

HUD::HUD(Context *context) : context(context) {
    Text.SetFont(context->renderer->Font);
    Text.SetSize(context->renderer->Font.GetCharacterSize());
    
    Hair1 = sf::Shape::Line(-15, 0, 15, 0, 2.5, sf::Color(255, 255, 255, 127));
    Hair2 = sf::Shape::Line(0, -15, 0, 15, 2.5, sf::Color(255, 255, 255, 127));
    
    Hair1.SetPosition(context->window->GetWidth() / 2, context->window->GetHeight() / 2);
    Hair2.SetPosition(Hair1.GetPosition());
};

void HUD::Draw() {
    Framerate = 1.f / context->window->GetFrameTime();
    
    // Draw FPS
    snprintf(buf, 10, "%.1f FPS", Framerate);
    //snprintf(buf, 10, "%f", player.Pos.Z);
    Text.SetText(buf);
    context->window->Draw(Text);
    
    // Draw crosshair
    context->window->Draw(Hair1);
    context->window->Draw(Hair2);
}

