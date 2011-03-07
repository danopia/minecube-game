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
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Up)) {
        if (Current <= 0) Current = Options.size();
        Current--;
        
        return;
    }
    
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Down)) {
        Current++;
        if (Current >= Options.size()) Current = 0;
        
        return;
    }
    
    // Handle mouse movement
    if ((Event.Type == sf::Event::MouseMoved)) {
        float middle = context->window->GetWidth() / 2;
        
        if ((Event.MouseMove.X < middle - 200) || (Event.MouseMove.X > middle + 200))
            return;
        
        float y = 150;
        for (int i = 0; i < Options.size(); i++) {
            y += 60;
            
            if ((Event.MouseMove.Y > y - 20) && (Event.MouseMove.Y < y + 20)) {
                Current = i;
                return;
            }
        }
        
        return;
    }
    
    // Handle click (assume the mouse has moved recently. I expect a bug report
    // on this eventually)
    if ((Event.Type == sf::Event::MouseButtonReleased) && (Event.MouseButton.Button == sf::Mouse::Left))
        return ItemChanged(Options[Current]);

    // Handle enter
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Return))
        return ItemChanged(Options[Current]);
}

void OptionsMenu::Render() {
    UIPage::Render();
    
    sf::String Label("Label", ContentFnt, 30);
    sf::String Label2("Value", ContentFnt, 30);
    sf::Shape Button = sf::Shape::Rectangle(-200, -20, 200, 20, sf::Color(127.f, 127.f, 127.f), 2, sf::Color::Black);
    Button.SetPosition(context->window->GetWidth() / 2, 150);
    
    for (int i = 0; i < Options.size(); i++) {
        Button.Move(0, 60);
        
        if (i == Current)
            Button.SetColor(sf::Color(220.f, 220.f, 220.f, 180.f));
        else
            Button.SetColor(sf::Color(127.f, 127.f, 127.f, 180.f));
        
        Label.SetText(Options[i].Label);
        
        if (Options[i].Type == 0)
            Label2.SetText((Options[i].Value.compare("on") == 0) ? "On" : "Off");
        else if (Options[i].Type == 2)
            Label2.SetText(Options[i].Value);
        
        Label.SetPosition(Button.GetPosition().x - 190, Button.GetPosition().y - 20);
        Label2.SetPosition(Button.GetPosition().x + 190 - Label2.GetRect().GetWidth(), Button.GetPosition().y - 20);
        
        context->window->Draw(Button);
        context->window->Draw(Label);
        context->window->Draw(Label2);
    }
}
