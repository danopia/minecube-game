#include "client/ui/buttonpage.h"

ButtonPage::ButtonPage(Context *context, std::string Subtitle, bool Background) : UIPage(context, Subtitle, Background), Current(0) {}

void ButtonPage::HandleEvent(sf::Event &Event) {
    UIPage::HandleEvent(Event);

    // Arrow keys...
    if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up)) {
        if (Current <= 0) Current = Buttons.size();

        Current--;

        return;
    }

    if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down)) {
        Current++;

        if (Current >= Buttons.size())
            Current = 0;

        return;
    }

    // Handle mouse movement
    if (Event.type == sf::Event::MouseMoved) {
        float middle = context->window->getSize().x / 2;

        if ((Event.mouseMove.x < middle - 200) || (Event.mouseMove.x > middle + 200))
            return;

        float y = 150;
        for (int i = 0; i < Buttons.size(); i++) {
            y += 60;

            if ((Event.mouseMove.y > y - 20) && (Event.mouseMove.y < y + 20)) {
                Current = i;
                return;
            }
        }

        return;
    }

    // Handle click (assume the mouse has moved recently. I expect a bug report
    // on this eventually)
    if ((Event.type == sf::Event::MouseButtonReleased) && (Event.mouseButton.button == sf::Mouse::Left))
        return ItemSelected(Buttons[Current]);

    // Handle enter
    if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Return))
        return ItemSelected(Buttons[Current]);
}

void ButtonPage::Render() {
    UIPage::Render();

    sf::Text Label("Button", ContentFnt, 30);
    sf::RectangleShape Button;
    Button.setSize(sf::Vector2f(400, 40));
    Button.setPosition(context->window->getSize().x / 2 - 200, 150 - 40);
    Button.setFillColor(sf::Color(127.f, 127.f, 127.f));
    Button.setOutlineThickness(2.f);
    Button.setOutlineColor(sf::Color::Black);

    for (int i = 0; i < Buttons.size(); i++) {
        Button.move(0, 60);

        if (i == Current)
            Button.setFillColor(sf::Color(220.f, 220.f, 220.f, 180.f));
        else
            Button.setFillColor(sf::Color(127.f, 127.f, 127.f, 180.f));

        Label.setString(Buttons[i]);
        Label.setPosition(Button.getPosition());
        Label.move(-(Label.getLocalBounds().width / 2), -20);
        Label.move(200, 20);

        context->window->draw(Button);
        context->window->draw(Label);
    }
}
