#include <algorithm>
#include <array>
#include <iostream>
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>
//#include "settings.cpp"


float speed_player = 0.1;
int wind_x_size = 1000;
int wind_y_size = 700;
float rad_ball = 20; //20
float dt = 0.01;
float betta_tormoz = 0.1; //0.1
float tormoz_V = 0.8;
float time_of_full_beat = 0.7;
float max_force_of_beat = 1150;
sf::VideoMode		videoMode = sf::VideoMode(wind_x_size, wind_y_size);
sf::RenderWindow* window = new sf::RenderWindow(videoMode, "Billiarrd", sf::Style::Default);


float field_y_size = 100; // в процентах от wind_y_size
float field_x_size = 0.4 * field_y_size;


template <typename T>
std::vector<T> operator+(std::vector<T> vec1, std::vector<T> vec2)
{
    std::vector<T> prod(vec1.size());
    std::transform(vec1.begin(), vec1.end(), vec2.begin(), prod.begin(), std::plus<T>());
    return prod;
}

template <typename T>
std::vector<T> operator-(std::vector<T> vec1, std::vector<T> vec2)
{
    std::vector<T> prod(vec1.size());
    std::transform(vec1.begin(), vec1.end(), vec2.begin(), prod.begin(), std::minus<T>());
    return prod;
}

template <typename T>
std::vector<T> operator*(std::vector<T> vec1, T skal)
{
    std::vector<T> prod(vec1.size());
    std::transform(vec1.begin(), vec1.end(), prod.begin(), [skal](T& el) { return el * skal; });
    return prod;
}

template <typename T>
std::vector<T> operator*(T skal, std::vector<T> vec1)
{
    std::vector<T> prod(vec1.size());
    std::transform(vec1.begin(), vec1.end(), prod.begin(), [skal](T& el) { return el * skal; });
    return prod;
}

template <typename T>
T skalar_prod(std::vector<T> vec1, std::vector<T> vec2)
{
    T sum = 0;
    for (int i = 0; i < vec1.size(); i++)
    {
        sum += vec1[i] * vec2[i];
    }
    return sum;
}

template <typename T>
T norm_squared(std::vector<T> vec)
{
    return skalar_prod(vec, vec);
}

//template <typename T>
float norm(std::vector<float> vec)
{
    return sqrt(norm_squared(vec));
}


template <typename T>
void print_vec(std::vector<T> vec)
{
    for (auto el : vec)
    {
        std::cout << el << " ";
    }
}

class Button: public sf::Drawable
{
public:
	bool pressed;
    Button(std::string str, sf::Vector2f pos)
    {
        shape.setPosition(pos);
        shape.setSize(sf::Vector2f(200.f, 50.f));
        shape.setFillColor(sf::Color(0, 0, 0, 150));
        font.loadFromFile("C:/Users/1/C++/game/game/game/Abbieshire.ttf");
        text.setFont(font);
        text.setCharacterSize(30);
        text.setString(str);
        text.setPosition(sf::Vector2f(
            shape.getPosition().x + (shape.getLocalBounds().width / 2.f - text.getLocalBounds().width / 2.f),
            shape.getPosition().y + (shape.getLocalBounds().height / 2.f - text.getLocalBounds().height)));
        pressed = false;
    }
    ~Button(){}

    void setPosition(float x, float y)
    {
        shape.setPosition(x, y);
        text.setPosition(shape.getPosition().x + 10.f, shape.getPosition().y + 10.f);
    }
    bool mouseCheck(sf::Vector2f mouse)
    {
        if (shape.getGlobalBounds().left <= mouse.x &&
            mouse.x <= (shape.getGlobalBounds().left + shape.getGlobalBounds().width) &&
            shape.getGlobalBounds().top <= mouse.y &&
            mouse.y <= (shape.getGlobalBounds().top + shape.getGlobalBounds().height))
        {
            shape.setFillColor(sf::Color(0, 90, 170, 180));
            return true;
        }

        shape.setFillColor(sf::Color(0, 0, 0, 150));
        return false;
    }
    
    void updateEvent(sf::Vector2f mouse, sf::Event event)
    {
        if (event.mouseButton.button == sf::Mouse::Left && mouseCheck(mouse))
        {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                shape.setOutlineThickness(-2);
                pressed = true;
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                shape.setOutlineThickness(0);
                pressed = false;
            }
        }
    }
    void update(sf::Vector2f mouse, sf::Event event)
    {
        mouseCheck(mouse);
        updateEvent(mouse, event);
    }

    enum btnType
    {
        CONTINUE,
        NEW_GAME,
        EXIT
    };

private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(shape);
        target.draw(text);
    }
};

class Menu: public sf::Drawable
{
public:
    std::map <Button::btnType, Button*> buttons;
    Menu() {
        background.setSize(sf::Vector2f(400.f, 450.f));
        background.setPosition(300.f, 125.f);
        background.setFillColor(sf::Color(0, 100, 0, 100));

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

    virtual void update(sf::RenderWindow* window, bool& isPaused, bool& isRestart, sf::Vector2f mouse, sf::Event event)
    {
        for (auto btn : buttons)
        {
            if (btn.first == Button::EXIT && btn.second->pressed)
            {
                window->close();
            }
            if (btn.first == Button::CONTINUE && btn.second->pressed)
            {
                isPaused = false;
            }
            if (btn.first == Button::NEW_GAME && btn.second->pressed)
            {
                isRestart = true;
            }
            btn.second->update(mouse, event);
        }
    }

private:
    sf::RectangleShape background;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(background);
        for (auto btn : buttons)
        {
            target.draw(*btn.second);
        }
    }
};
class GUI : public sf::Drawable
{
public:
    bool gamePaused;
    sf::Font font;
    Menu menu;

    GUI() {
        font.loadFromFile("C:/Users/1/C++/game/game/game/Abbieshire.ttf");

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
    void addPoints(Player player)
    {
        if (player.number == 0)
        {
            player1.points++;
        }
        if (player.number == 1)
        {
            player2.points++;
        }
    }

    void restart()
    {
        gamePaused = false;
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

class Ball
{
private:
    std::vector<float> pos = std::vector<float>(2); // вектор координат центра шарика
    std::vector<float> V = std::vector<float>(2);   // вектор скорости шарика
    std::vector<float> a = std::vector<float>(2);   // вектор ускорения шарика
    float rad = rad_ball;
    sf::CircleShape shape;
    sf::Color color;

    sf::RectangleShape field;  // поле от которого отскакивает шарик
    float field_x1;
    float field_x2;
    float field_y1;
    float field_y2;

public:
    Ball(std::vector<float> pos, sf::RectangleShape field) :
        pos(pos), shape(rad), color(sf::Color::Red), field(field)
    {
        shape.setFillColor(color);
        shape.setPosition(pos[0] - rad, pos[1] - rad);

        field_x1 = field.getPosition().x;
        field_x2 = field.getPosition().x + field.getSize().x;
        field_y1 = field.getPosition().y;
        field_y2 = field.getPosition().y + field.getSize().y;
    };

    std::vector<float> ret_pos()
    {
        return pos;
    }

    std::vector<float> ret_speed()
    {
        return V;
    }

    void draw()
    {
        window->draw(shape);
    };

    void move(std::vector<Ball> balls)
    {


        a = float(-1) * V * betta_tormoz;
        interaction(balls);
        shape.setPosition(pos[0] - rad, pos[1] - rad);
        V = V + a * dt;
        if (norm(V) < tormoz_V) {
            V[0] = 0;
            V[1] = 0;
        }

        pos = pos + V * dt;
    };

    void interaction(std::vector<Ball> balls)
    {
        colapse_with_all_balls(balls);
        colapse_wall();
    }

    void colapse_wall()
    {
        float x = pos[0];
        float y = pos[1];
        if (x + rad > field_x2)
        {
            pos = std::vector<float>{ field_x2 - rad, y };
            V[0] *= -1;
        }
        if (x - rad < field_x1)
        {
            pos = std::vector<float>{ field_x1 + rad, y };
            V[0] *= -1;
        }

        if (y + rad > field_y2)
        {
            pos = std::vector<float>{ x, field_y2 - rad };
            V[1] *= -1;
        }
        if (y - rad < field_y1)
        {
            pos = std::vector<float>{ x, field_y1 + rad };
            V[1] *= -1;
        }
    }

    void colapse_with_all_balls(std::vector<Ball> balls)
    {
        for (Ball ball2 : balls)
        {
            if (ball2.pos != pos)
            {
                std::vector<float> x1 = pos;
                std::vector<float> x2 = ball2.ret_pos();

                if (norm(x1 - x2) <= 2 * rad)
                {
                    std::cout << "COLAPSE";
                    colapse_ball(ball2);
                }
            }
        }
    }

    void colapse_ball(Ball ball2)
    {
        std::vector<float> V1 = V;
        std::vector<float> V2 = ball2.ret_speed();
        std::vector<float> x1 = pos;
        std::vector<float> x2 = ball2.ret_pos();
        if (skalar_prod(V2 - V1, x1 - x2) >= 0)
        {
            V = V - skalar_prod(V1 - V2, x1 - x2) / norm_squared(x1 - x2) * (x1 - x2);
        }
    }

    bool is_clicked(std::vector <float> mouse_pos)
    {
        return norm(pos - mouse_pos) < rad_ball;
    }

    void set_V(std::vector<float> new_V)
    {
        V = new_V;
    }
};

class Game
{
private:
    sf::Vector2f		m_mouse;
    sf::RectangleShape field;
    bool restartGame = false;
    std::vector <float> mouse_pos;
    int selected_ball = -1; //значение -1, если не выбран.
    float force_of_beat = 0;
    std::vector<Ball> balls;
    //bool restartGame; //= false;
    GUI* gui;
    std::vector<Ball> start_balls;
    sf::Event   sfmlEvent;
    
public:
    Game()
    {
        field.setSize(sf::Vector2f(field_x_size / 100 * wind_x_size, field_y_size / 100 * wind_y_size));
        field.setPosition(wind_x_size / 2 - field_x_size / 100 * wind_x_size / 2, 0);

        mouse_pos = std::vector <float>{ -10, -10 };
        //restartGame = false;

        gui = new GUI();

        std::vector<float> pos1{ 300, 100 };
        std::vector<float> V1{ 10, 10 };
        Ball ball1(pos1, field);
        ball1.set_V(V1);
        std::vector<float> pos2{ 400, 50 };
        std::vector<float> V2{ 5, 10 };
        Ball ball2(pos2, field);
        ball2.set_V(V2);
        std::vector<float> pos3{ 400, 200 };
        std::vector<float> V3{ 0, -10 };
        Ball ball3(pos3, field);
        ball3.set_V(V3);
        std::vector<float> pos4{ 400, 600 };
        std::vector<float> V4{ 300, -100 };
        Ball ball4(pos4, field);
        ball4.set_V(V4);

        balls = { ball1, ball2, ball3, ball4 };
        start_balls = balls;
    };

    void render()
    {
        if (!gui->gamePaused) {
            std::vector<Ball> new_balls;
            for (Ball main_ball : balls)
            {
                main_ball.move(balls);
                new_balls.push_back(main_ball);
                /*if (x)  //Добавить булевскую элемент забития мяча
                {
                    gui->addPoints(gui->getCurrentPlayer());
                    x = false;
                }*/
                if (main_ball.ret_speed() == std::vector<float>(2))
                {
                    gui->setCurrentPlayer(1 - gui->getCurrentPlayer().number);
                }
            }
            balls = new_balls;
            window->clear(sf::Color(75, 0, 163));
            window->draw(field);
            for (Ball ball : balls)
            {
                ball.draw();
            }
        }
    }

    void restart_game()
    {
        gui->restart();
        restartGame = false;
        balls = start_balls;
    }

    void event_update()
    {
        while (window->pollEvent(sfmlEvent))
        {
            //можно добавить управление шариком
            switch (sfmlEvent.type)
            {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                if (sfmlEvent.key.code == sf::Keyboard::Escape)
                {
                    std::cout << 45678;
                    gui->gamePaused = 1 - gui->gamePaused;
                }

            case sf::Event::MouseMoved:
                m_mouse.x = sfmlEvent.mouseMove.x;
                m_mouse.y = sfmlEvent.mouseMove.y;

            case sf::Event::MouseButtonPressed:
                if (true) {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (force_of_beat < time_of_full_beat) {
                            force_of_beat += dt;
                        }

                        for (int i = 0; i < balls.size(); i++)
                        {
                            //sf::Vector2i pos_m = sf::Mouse::getPosition(window);
                            mouse_pos[0] = m_mouse.x;
                            mouse_pos[1] = m_mouse.y;
                            if (balls[i].is_clicked(mouse_pos)) { selected_ball = i; }
                        }
                    }
                    else
                    {
                        if (selected_ball != -1)
                        {
                            std::cout << "Beat!";
                            std::vector <float> beat{ 0,0 };
                            beat = ((balls[selected_ball].ret_pos() - mouse_pos) * (1 / norm(balls[selected_ball].ret_pos() - mouse_pos))) * force_of_beat * max_force_of_beat;
                            balls[selected_ball].set_V(balls[selected_ball].ret_speed() + beat);
                            //beat
                        }

                        selected_ball = -1;
                        force_of_beat = 0;
                    }
                }
            }
        }
    }

    void manager()
    {
        while (window->isOpen())
        {
            render();

            if (restartGame)
            {
                restart_game();
            }
            
            event_update();

            gui->update(window, gui->gamePaused, restartGame, m_mouse, sfmlEvent);
            window->draw(*gui);

            window->display();
        }
    }
};


int main()
{
    Game game = Game();
    game.manager();

    return 0;
}