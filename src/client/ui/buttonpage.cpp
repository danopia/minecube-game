#include "client/ui/buttonpage.h"

ButtonPage::ButtonPage(Context *context, std::string Subtitle, bool Background) : UIPage(context, Subtitle, Background), Current(0) {}

void ButtonPage::HandleEvent(sf::Event &Event) {
    UIPage::HandleEvent(Event);

    // Arrow keys...
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Up)) {
        if (Current <= 0) Current = Buttons.size();
        
        Current--;
        
        return;
    }
    
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Down)) {
        Current++;
        
        if (Current >= Buttons.size())
            Current = 0;
        
        return;
    }
    
    // Handle mouse movement
    if ((Event.Type == sf::Event::MouseMoved)) {
        float middle = context->window->GetWidth() / 2;
        
        if ((Event.MouseMove.X < middle - 200) || (Event.MouseMove.X > middle + 200))
            return;
        
        float y = 150;
        for (int i = 0; i < Buttons.size(); i++) {
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
        return ItemSelected(Buttons[Current]);

    // Handle enter
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Return))
        return ItemSelected(Buttons[Current]);
}

void ButtonPage::Render() {
    UIPage::Render();
    
    sf::String Label("Button", ContentFnt, 30);
    sf::Shape Button = sf::Shape::Rectangle(-200, -20, 200, 20, sf::Color(127.f, 127.f, 127.f), 2, sf::Color::Black);
    Button.SetPosition(context->window->GetWidth() / 2, 150);
    
    for (int i = 0; i < Buttons.size(); i++) {
        Button.Move(0, 60);
        
        if (i == Current)
            Button.SetColor(sf::Color(220.f, 220.f, 220.f, 180.f));
        else
            Button.SetColor(sf::Color(127.f, 127.f, 127.f, 180.f));
    
        Label.SetText(Buttons[i]);
        Label.SetPosition(Button.GetPosition().x - (Label.GetRect().GetWidth() / 2), Button.GetPosition().y - 20);
        
        context->window->Draw(Button);
        context->window->Draw(Label);
    }
}
