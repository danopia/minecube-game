#include "client/ui/waitpage.h"

void WaitPage::Render() {
    UIPage::Render();
    
    sf::Shape Button = sf::Shape::Rectangle(-200, -50, 200, 50, sf::Color(127.f, 127.f, 127.f), 2, sf::Color::Black);
    Button.SetPosition(context->window->GetWidth() / 2, context->window->GetHeight() / 2);
    Button.SetColor(sf::Color(220.f, 220.f, 220.f, 180.f));
    
    sf::String Label("Waiting for a response\nfrom the server.\nPlease wait...", ContentFnt, 30);
    Label.SetPosition(Button.GetPosition().x - (Label.GetRect().GetWidth() / 2), Button.GetPosition().y - (Label.GetRect().GetHeight() / 2));
    
    context->window->Draw(Button);
    context->window->Draw(Label);
    
    context->socket->DoStep();
    if (context->inGame)
        Running = false;
}

