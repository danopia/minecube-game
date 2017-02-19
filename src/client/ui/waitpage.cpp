#include "client/ui/waitpage.h"

void WaitPage::Render() {
    UIPage::Render();

    sf::RectangleShape Button;
    Button.setSize(sf::Vector2f(400, 100));
    Button.setPosition(context->window->getSize().x / 2, context->window->getSize().y / 2);
    Button.move(-200, -50);
    Button.setFillColor(sf::Color(220.f, 220.f, 220.f, 180.f));
    Button.setOutlineThickness(2.f);
    Button.setOutlineColor(sf::Color::Black);

    sf::Text Label("Waiting for a response\nfrom the server.\nPlease wait...", ContentFnt, 30);
    Label.setPosition(Button.getPosition());
    Label.move(-(Label.getLocalBounds().width / 2), -(Label.getLocalBounds().height / 2));

    context->window->draw(Button);
    context->window->draw(Label);

    context->socket->DoStep();
    if (context->inGame)
        Running = false;
}
