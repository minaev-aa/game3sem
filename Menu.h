#pragma once
#include "Button.h"


class Menu : public sf::Drawable
{
public:
    std::map <Button::btnType, Button*> buttons;
    Menu() {
        background.setSize(sf::Vector2f(400.f, 450.f));
        background.setPosition(300.f, 125.f);
        background.setFillColor(sf::Color(0, 100, 0, 100));

        font.loadFromFile(".Abbieshire.ttf");
        text.setFont(font);
        text.setCharacterSize(30);
        text.setString("Game over");
        text.setPosition(sf::Vector2f(
            background.getPosition().x + (background.getLocalBounds().width / 2.f - text.getLocalBounds().width / 2.f),
            background.getPosition().y + (background.getLocalBounds().height / 2.f - text.getLocalBounds().height)));

        Button* btnContinue = new Button("Continue", sf::Vector2f(400.f, 150.f));
        Button* btnNewGame = new Button("New Game", sf::Vector2f(400.f, 250.f));
        Button* btnExit = new Button("Exit", sf::Vector2f(400.f, 450.f));
        buttons.insert(std::pair<Button::btnType, Button*>(Button::CONTINUE, btnContinue));
        buttons.insert(std::pair<Button::btnType, Button*>(Button::NEW_GAME, btnNewGame));
        buttons.insert(std::pair<Button::btnType, Button*>(Button::EXIT, btnExit));
    }
    ~Menu()
    {
        buttons.clear();
    }

    void set_gameover(bool game_over)
    {
        gameover = game_over;
    }

    virtual void update(sf::RenderWindow* window, bool& isPaused, bool& isRestart, sf::Vector2f mouse, sf::Event event)
    {
            for (auto btn : buttons)
            {
                if (btn.first == Button::EXIT && btn.second->pressed && isPaused)
                {
                    window->close();
                }
                if (btn.first == Button::CONTINUE && btn.second->pressed && isPaused)
                {
                    isPaused = false;
                }
                if (btn.first == Button::NEW_GAME && btn.second->pressed && isPaused)
                {
                    isRestart = true;
                }
            	btn.second->update(mouse, event, isPaused);
                
            }
        
    }

private:
    sf::RectangleShape background;
    sf::Text text;
    sf::Font font;
    bool gameover = false;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(background);
        for (auto btn : buttons)
        {
            if(!gameover)
				target.draw(*btn.second);
            else {
                if (btn.first != Button::CONTINUE)
                    target.draw(*btn.second);
            }
        }
        if(gameover)
        {
            target.draw(text);
        }
    }
};