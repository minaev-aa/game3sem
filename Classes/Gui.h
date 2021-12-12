#pragma once


class GUI : public sf::Drawable
{
public:
    bool gamePaused;
    sf::Font font;
    Menu menu;

    GUI() {
        font.loadFromFile("./PFAgoraSlabPro.ttf");
        player1.points = 0;
        player1.number = 0;
        player1.numberText.setFont(font);
        player1.pointsText.setFont(font);
        player1.numberText.setString("Player 1");
        player1.numberText.setPosition(100, 100);
        player1.numberText.setCharacterSize(40);
        player1.pointsText.setCharacterSize(35);
        player1.pointsText.setString(std::to_string(player1.points));
        player1.pointsText.setPosition(150, 170);
        player2.points = 0;
        player2.number = 1;
        player2.numberText.setFont(font);
        player2.pointsText.setFont(font);
        player2.numberText.setString("Player 2\n");
        player2.numberText.setPosition(750, 100);
        player2.numberText.setCharacterSize(40);
        player2.pointsText.setCharacterSize(35);
        player2.pointsText.setString(std::to_string(player2.points));
        player2.pointsText.setPosition(800, 170);
        currentPlayer = player1;
        gamePaused = false;
    }
    ~GUI() {}

    struct Player
    {
        sf::Text numberText;
        sf::Text pointsText;
        int points;
        int number;
    };

    Player player1;
    Player player2;
    Player currentPlayer;

    void setCurrentPlayer(Player player)
    {
        currentPlayer = player;
    }
    void setCurrentPlayer(int number)
    {
        if (number == 0)
        {
            currentPlayer = player1;
        }
        if (number == 1)
        {
            currentPlayer = player2;
        }
    }

    Player getCurrentPlayer()
    {
        return currentPlayer;
    }

    void addPoints(Player player, int count)
    {
        if (player.number == 0)
        {
            player1.points += count;
        }
        if (player.number == 1)
        {
            player2.points += count;
        }
    }

    void set_gameover(bool game_over)
    {
        menu.set_gameover(game_over);
    }

    void restart()
    {
        gamePaused = false;
        set_gameover(false);
        player1.points = 0;
        player2.points = 0;
        currentPlayer = player1;
    }

    virtual void update(sf::RenderWindow* window, bool& isPaused, bool& isRestart, sf::Vector2f mouse, sf::Event event)
    {
        player1.numberText.setString("Player 1");
        player1.pointsText.setString(std::to_string(player1.points));
        player2.numberText.setString("Player 2");
        player2.pointsText.setString(std::to_string(player2.points));
        player1.numberText.setOutlineThickness(0.f);
        player2.numberText.setOutlineThickness(0.f);
        if (currentPlayer.number == 0)
        {
            player1.numberText.setOutlineThickness(3.f);
            player1.numberText.setOutlineColor(sf::Color(0, 0, 0, 100));
        }
        if (currentPlayer.number == 1)
        {
            player2.numberText.setOutlineThickness(3.f);
            player2.numberText.setOutlineColor(sf::Color(0, 0, 0, 100));
        }
        menu.update(window, isPaused, isRestart, mouse, event);

    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (gamePaused)
        {
            target.draw(menu);
        }
        target.draw(player1.numberText);
        target.draw(player1.pointsText);
        target.draw(player2.numberText);
        target.draw(player2.pointsText);
    }
};