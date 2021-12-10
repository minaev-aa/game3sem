#include <algorithm>
#include <array>
#include <iostream>
#include <math.h>
#include <stdlib.h> 
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>
//#include "settings.cpp"


float speed_player = 0.1f;
int wind_x_size = 1000;
int wind_y_size = 700;
float rad_ball = 15; //20
float rad_hole = 20;
float dt = 0.02f;
float betta_tormoz = 0.3f; //0.1
float tormoz_V = 0.8f;
float time_of_full_beat = 0.7f;
float max_force_of_beat = 1150;
sf::VideoMode		videoMode = sf::VideoMode(wind_x_size, wind_y_size);
sf::RenderWindow* window = new sf::RenderWindow(videoMode, "Billiarrd", sf::Style::Default);


float field_y_size = 100; // a i?ioaioao io wind_y_size
float field_x_size = 0.4 * field_y_size;
int number_balls_first_rad = 5;

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
        font.loadFromFile("./PFAgoraSlabPro.ttf");
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
            	btn.second->update(mouse, event);
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
    enum sfxType
    {
        CIRCLES,
        CIRCLEBOARD,
        CIRCLEHOAL,
        COUNT
    };
    sf::SoundBuffer* buffer;
    std::array<sf::Sound, COUNT> sound;
    std::vector<float> pos = std::vector<float>(2); // aaeoi? eii?aeiao oaio?a oa?eea
    std::vector<float> V = std::vector<float>(2);   // aaeoi? nei?inoe oa?eea
    std::vector<float> a = std::vector<float>(2);   // aaeoi? onei?aiey oa?eea
    float rad = rad_ball;
    sf::CircleShape shape;
    sf::Color color;

    sf::RectangleShape field;  // iiea io eioi?iai ioneaeeaaao oa?ee
    float field_x1;
    float field_x2;
    float field_y1;
    float field_y2;
    std::vector<Ball> *silka;
    bool not_del = true;

public:
    void init_sound()
    {
        buffer = new sf::SoundBuffer;
        buffer->loadFromFile("./sfx/Circles.wav");
        sound[CIRCLES].setBuffer(*buffer);
        buffer = new sf::SoundBuffer;
        buffer->loadFromFile("./sfx/CircleBoard.wav");
        sound[CIRCLEBOARD].setBuffer(*buffer);
        buffer = new sf::SoundBuffer;
        buffer->loadFromFile("./sfx/Cue.wav");
        sound[CIRCLEHOAL].setBuffer(*buffer);
    }
        void playSFX(const int sfxType, Ball* obj)
            {
                auto vol = obj->ret_speed();
                float volume = sqrtf(vol[0] * vol[0] + vol[1] * vol[1]) / 5.f;
                sound[sfxType].setVolume(std::min(volume, 100.f));
                sound[sfxType].play();
            }
    Ball(std::vector<float> pos, sf::RectangleShape field, std::vector<Ball>* silka) :
        pos(pos), shape(rad), color(sf::Color::Red), field(field), silka(silka)
    {
        shape.setFillColor(color);
        shape.setPosition(pos[0] - rad, pos[1] - rad);

        field_x1 = field.getPosition().x;
        field_x2 = field.getPosition().x + field.getSize().x;
        field_y1 = field.getPosition().y;
        field_y2 = field.getPosition().y + field.getSize().y;
        init_sound();
    };

    std::vector<float> ret_pos()
    {
        return pos;
    }

    std::vector<float> ret_speed()
    {
        return V;
    }

    sf::Color ret_color()
    {
        return color;
    }

    void draw()
    {
        window->draw(shape);
        
    };

    void move()//std::vector<Ball> balls)
    {


        a = float(-1) * V * betta_tormoz;
        interaction();
        shape.setPosition(pos[0] - rad, pos[1] - rad);
        V = V + a * dt;
        if (norm(V) < tormoz_V) {
            V[0] = 0;
            V[1] = 0;
        }

        pos = pos + V * dt;
    };

    void interaction()
    {
        colapse_with_all_balls();
        colapse_wall();
    }

    void colapse_wall()
    {
        float x = pos[0];
        float y = pos[1];
        if (not_del)
        {
            if (x + rad > field_x2)
            {
                pos = std::vector<float>{ field_x2 - rad, y };
                V[0] *= -1;
                playSFX(CIRCLEBOARD, this);
                sf::sleep(sf::milliseconds(10));
            }
            if (x - rad < field_x1)
            {
                pos = std::vector<float>{ field_x1 + rad, y };
                V[0] *= -1;
                playSFX(CIRCLEBOARD, this);
                sf::sleep(sf::milliseconds(10));
            }

            if (y + rad > field_y2)
            {
                pos = std::vector<float>{ x, field_y2 - rad };
                V[1] *= -1;
                playSFX(CIRCLEBOARD, this);
                sf::sleep(sf::milliseconds(10));
            }
            if (y - rad < field_y1)
            {
                pos = std::vector<float>{ x, field_y1 + rad };
                V[1] *= -1;
                playSFX(CIRCLEBOARD, this);
                sf::sleep(sf::milliseconds(10));
            }
        }
    }

    void colapse_with_all_balls()
    {
        for (Ball ball2 : *silka)
        {
            
            if (ball2.pos != pos)
            {
                std::vector<float> x1 = pos;
                std::vector<float> x2 = ball2.ret_pos();
                
                if (x1[0] - x2[0] < 2 * rad)
                {
                    if (x1[1] - x2[1] < 2 * rad)
                    {
                        if (norm(x1 - x2) < 2 * rad)
                        {
                            std::cout << "COLAPSE";
                            colapse_ball(ball2);
                        }
                   }
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

        playSFX(CIRCLES, this);
        sf::sleep(sf::milliseconds(10));
    }

    void del()
    {
        not_del = false;
        std::vector<float> p23{ -rad,-rad };
        pos = p23;
        std::vector<float> V2{ 0, 0};
        V = V2;
    }

    bool is_clicked(std::vector <float> mouse_pos)
    {
        return norm(pos - mouse_pos) < rad_ball;
    }

    void set_V(std::vector<float> new_V)
    {
        V = new_V;
    }

    void set_color(sf::Color col2)
    {
        color = col2;
        shape.setFillColor(color);
    }

    void set_silka(std::vector<Ball>* silka2)
    {
        silka = silka2;
    }
};

class Hole
{
private:
    float rad = rad_hole;
    sf::CircleShape shape;
    sf::Color color;
    std::vector<float> pos = std::vector<float>(2); // Coords x, y
    std::vector<Ball>* silka;

public:
    Hole(std::vector<float> pos, std::vector<Ball>* silka) :
        pos(pos), shape(rad), color(sf::Color::Red), silka(silka)
    {
        shape.setFillColor(color);
        shape.setPosition(pos[0] - rad, pos[1] - rad);
    };


    std::vector<float> ret_pos()
    {
        return pos;
    }

    void draw()
    {
        window->draw(shape);

    };

    void set_color(sf::Color col2)
    {
        color = col2;
        shape.setFillColor(color);
    }

    int collabse_balls(int New_Score)
    {
        for (auto ball2 = (*silka).begin(); ball2 != (*silka).end(); ball2++)
        {
            std::vector<float> x1 = pos;
            std::vector<float> x2 = (*ball2).ret_pos();

            if (x1[0] - x2[0] < rad_hole)
            {
                if (x1[1] - x2[1] < rad_hole)
                {
                    if (norm(x1 - x2) < rad_hole)
                    {
                        if ((*ball2).ret_color() != sf::Color::Black)
                        {
                            std::cout << "COUTch";
                            //(silka).erase(ball2);
                            (*ball2).del();
                            New_Score++;
                        }
                        else
                        {
                            std::cout << "BLACK!!!";
                            //New_Score--;
                        }
                    }
                }
            }

        }
        return New_Score;
    }
};

class Game
{
private:
    sf::Vector2f		m_mouse;
    sf::RectangleShape field;
    bool restartGame = false;
    std::vector <float> mouse_pos;
    int selected_ball = -1; 
    float force_of_beat = 0;
    std::vector<Ball> balls;
    bool udar = false;
    GUI* gui;
    std::vector<Ball> start_balls;
    sf::Event   sfmlEvent;
    sf::VertexArray line;
    std::vector<Hole> holes;
    float force_beat;
    int New_Score = 0;
    
public:

    Game()
    {
        field.setSize(sf::Vector2f(field_x_size / 100 * wind_x_size, field_y_size / 100 * wind_y_size));
        field.setPosition(wind_x_size / 2 - field_x_size / 100 * wind_x_size / 2, 0);

        mouse_pos = std::vector <float>{ -10, -10 };
        //restartGame = false;

        gui = new GUI();

        std::vector<float> pos1{ float(wind_x_size) / 2, float(wind_y_size) / 2 };
        Ball ball1(pos1, field, &balls);
        ball1.set_color(sf::Color::Black);
        //std::vector<float> V{0, 300};
        //ball1.set_V(V);
        balls.push_back(ball1);
        
        for (int i = 0; i < number_balls_first_rad; i++)
        {
            float y = float(wind_y_size) / 2 -  rad_ball* pow(3.0,0.5) * i - i - 100;
            for (int j = 0; j <= i; j++)
            {
                if (i % 2 == 1)
                {
                    float x = float(wind_x_size) / 2 - (i+1) / 2 * rad_ball*2 + rad_ball + j * 2 * rad_ball;
                    std::vector<float> pos{ x,y };
                    Ball ball(pos, field, &balls);
                    balls.push_back(ball);
                    
                }
                else
                {
                    float x = float(wind_x_size) / 2 - (i) / 2 * rad_ball*2 + j * 2 * rad_ball;
                    std::vector<float> pos{ x,y };
                    Ball ball(pos, field, &balls);
                    balls.push_back(ball);
                    
                }
                
            }
        }

        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j += 2)
            {
                std::cout << field_x_size;
                std::vector<float> pos_right{ float((wind_x_size + j * field_x_size * 10) / 2 - j * rad_hole), float(wind_y_size / 2 + i * (wind_y_size / 2 - rad_hole)) };
                Hole hole_right(pos_right, &balls);
                hole_right.set_color(sf::Color::Yellow);
                holes.push_back(hole_right);
                Hole hole1(pos_right, &balls);
                hole_right.set_color(sf::Color::Yellow);
                holes.push_back(hole_right);
            }
        }

        std::vector<float> pos4{ 333, 333 };
       
        /*
        for (Ball ball : balls)
        {
            ball.set_silka(balls);
        }*/


        start_balls = balls;
        line = sf::VertexArray(sf::Lines, 2);
    };

    void render()
    {
        if (!gui->gamePaused) {
            std::vector<Ball> new_balls;
            for (Ball main_ball : balls)
            {
                main_ball.move();
                new_balls.push_back(main_ball);
                if (New_Score != 0)  
                {
                    gui->addPoints(gui->getCurrentPlayer(), New_Score);
                    New_Score = 0;
                }
                for (Ball main_ball1 : balls) {
                    if (udar && main_ball1.ret_speed() == std::vector<float>(2))
                    {
                        gui->setCurrentPlayer(1 - gui->getCurrentPlayer().number);
                        udar = false;
                    }
                    if (main_ball1.ret_speed() != std::vector<float>(2))
                    {
                        gui->setCurrentPlayer(1 - gui->getCurrentPlayer().number);
                        udar = true;
                        break;
                    }
                }
            }
            balls = new_balls;
            for (Hole main_hole : holes)
            {
                New_Score = main_hole.collabse_balls(New_Score);

            }
            window->clear(sf::Color(75, 0, 163));}
            window->draw(field);
            window->draw(line);
            for (Hole ball : holes)
            {
                ball.draw();
            }
            for (Ball ball : balls)
            {
                ball.draw();
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
        bool mouse_isnot_pressed = true;
        while (window->pollEvent(sfmlEvent))
        {
            switch (sfmlEvent.type)
            {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                if (sfmlEvent.key.code == sf::Keyboard::Escape)
                {
                    gui->gamePaused = 1 - gui->gamePaused;
                }

            case sf::Event::MouseMoved:
                m_mouse.x = sfmlEvent.mouseMove.x;
                m_mouse.y = sfmlEvent.mouseMove.y;

            case sf::Event::MouseButtonPressed:
                if (!gui->gamePaused) {

                    mouse_pos[0] = m_mouse.x;
                    mouse_pos[1] = m_mouse.y;

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {

                        for (int i = 0; i < balls.size(); i++)
                        {
                            if (balls[i].is_clicked(mouse_pos)) { selected_ball = i; }
                        }

                        if (selected_ball != -1) {
                            force_beat = std::min(norm((balls[selected_ball].ret_pos() - mouse_pos)) / 200, 1.0f) * max_force_of_beat;
                            std::vector <float> vect = (balls[selected_ball].ret_pos() - mouse_pos) * (1 / norm(balls[selected_ball].ret_pos() - mouse_pos));
                            line[0].position = sf::Vector2f((balls[selected_ball].ret_pos() + 2 * rad_ball * vect)[0],
                                (balls[selected_ball].ret_pos() + 2 * rad_ball * vect)[1]);
                            line[0].color = sf::Color::Blue;
                            line[1].position = sf::Vector2f((balls[selected_ball].ret_pos() + (2 + 7 * force_beat / max_force_of_beat) * rad_ball * vect)[0],
                                (balls[selected_ball].ret_pos() + (2 + 7 * force_beat / max_force_of_beat) * rad_ball * vect)[1]);
                            // градиент
                            line[1].color = sf::Color::Cyan;
                        }
                    }
                    else
                    {
                        if (selected_ball != -1)
                        {
                            std::cout << "Beat!";
                            std::vector <float> beat{ 0,0 };
                            force_beat = std::min(norm((balls[selected_ball].ret_pos() - mouse_pos)) / 200, 1.0f) * max_force_of_beat;
                            beat = ((balls[selected_ball].ret_pos() - mouse_pos)) * (1 / norm(balls[selected_ball].ret_pos() - mouse_pos)) * force_beat;
                            balls[selected_ball].set_V(balls[selected_ball].ret_speed() + beat);
                            udar = true;
                            //beat
                        }

                        selected_ball = -1;
                        line[0].position = sf::Vector2f(1.0, 1.0);
                        line[0].color = sf::Color::Blue;
                        line[1].position = sf::Vector2f(1.1, 1.1);
                        // градиент
                        line[1].color = sf::Color::Cyan;

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
