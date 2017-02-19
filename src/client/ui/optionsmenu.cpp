#include "client/ui/optionsmenu.h"

OptionsMenu::OptionsMenu(Context *context, std::string Subtitle, bool Background) : UIPage(context, Subtitle, Background), Current(0) {
    Options.push_back(Option("window", "Fullscreen", "fullscreen", "false", 0));
    Options.push_back(Option("window", "Weird rendering", "weird", "false", 0));
    Options.push_back(Option("user",   "Username", "username", "anon", 2));
}

void OptionsMenu::ItemChanged(Option &option) {
    if (option.Type == 0)
        option.Value = ((option.Value.compare("on") == 0) ? "off" : "on");
    else if (option.Type == 2)
        option.Value = "danopia";
}

void OptionsMenu::HandleEvent(sf::Event &Event) {
    UIPage::HandleEvent(Event);

    // Arrow keys...
    if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up)) {
        if (Current <= 0) Current = Options.size();
        Current--;

        return;
    }

    if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down)) {
        Current++;
        if (Current >= Options.size()) Current = 0;

        return;
    }

    // Handle mouse movement
    if (Event.type == sf::Event::MouseMoved) {
        float middle = context->window->getSize().x / 2;

        if ((Event.mouseMove.x < middle - 200) || (Event.mouseMove.x > middle + 200))
            return;

        float y = 150;
        for (int i = 0; i < Options.size(); i++) {
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
        return ItemChanged(Options[Current]);

    // Handle enter
    if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Return))
        return ItemChanged(Options[Current]);
}

void OptionsMenu::Render() {
    UIPage::Render();

    sf::Text Label("Label", ContentFnt, 30);
    sf::Text Label2("Value", ContentFnt, 30);

    sf::RectangleShape Button;
    Button.setSize(sf::Vector2f(400, 40));
    Button.setPosition(context->window->getSize().x / 2 - 200, 150 - 40);
    Button.setFillColor(sf::Color(127.f, 127.f, 127.f));
    Button.setOutlineThickness(2.f);
    Button.setOutlineColor(sf::Color::Black);

    for (int i = 0; i < Options.size(); i++) {
        Button.move(0, 60);

        if (i == Current)
            Button.setFillColor(sf::Color(220.f, 220.f, 220.f, 180.f));
        else
            Button.setFillColor(sf::Color(127.f, 127.f, 127.f, 180.f));

        Label.setString(Options[i].Label);

        if (Options[i].Type == 0)
            Label2.setString((Options[i].Value.compare("on") == 0) ? "On" : "Off");
        else if (Options[i].Type == 2)
            Label2.setString(Options[i].Value);

        Label.setPosition(Button.getPosition().x - 190, Button.getPosition().y - 20);
        Label2.setPosition(Button.getPosition().x + 190 - Label2.getLocalBounds().width, Button.getPosition().y - 20);

        context->window->draw(Button);
        context->window->draw(Label);
        context->window->draw(Label2);
    }
}
