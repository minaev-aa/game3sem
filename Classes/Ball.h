#pragma once
#include "Classes/Operator.h"
#include "Settings.h"
#include "Classes/Hole.h"


class Ball
{
private:
    enum sfxType
    {
        CIRCLES,
        CIRCLEHOAL,
        COUNT
    };
    sf::SoundBuffer* buffer;
    std::array<sf::Sound, COUNT> sound;

    std::vector<float> pos = std::vector<float>(2);
    std::vector<float> V = std::vector<float>(2);
    std::vector<float> a = std::vector<float>(2);
    float rad = rad_ball;
    sf::CircleShape shape;
    sf::Color color;

    sf::RectangleShape field;
    float field_x1;
    float field_x2;
    float field_y1;
    float field_y2;
    std::vector<Ball>* silka;
    std::vector<Hole>* holes;


public:
    bool not_del = true;
    void init_sound()
    {
        buffer = new sf::SoundBuffer;
        buffer->loadFromFile("C:/Users/snigu/Downloads/game/game/sfx/Circles.wav");
        sound[CIRCLES].setBuffer(*buffer);
        buffer = new sf::SoundBuffer;
        buffer->loadFromFile("C:/Users/snigu/Downloads/game/game/sfx/CircleHole.wav");
        sound[CIRCLEHOAL].setBuffer(*buffer);
    }
    void playSFX(const int sfxType, Ball* obj)
    {
        auto vol = obj->ret_speed();
        float volume = sqrtf(vol[0] * vol[0] + vol[1] * vol[1]) / 5.f;
        sound[sfxType].setVolume(std::min(volume, 100.f));
        sound[sfxType].play();
    }
    Ball(std::vector<float> pos, sf::RectangleShape field, std::vector<Ball>* silka, std::vector<Hole>* holes) :
        pos(pos), shape(rad), color(sf::Color::Red), field(field), silka(silka), holes(holes)
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

    void move()
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
    }

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
        playSFX(CIRCLEHOAL, this);
        sf::sleep(sf::milliseconds(10));
        not_del = false;
        std::vector<float> p23{ -rad,-rad };
        pos = p23;
        std::vector<float> V2{ 0, 0 };
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
    void set_pos(std::vector<float> mouse_pos)
    {
        playSFX(CIRCLEHOAL, this);
        sf::sleep(sf::milliseconds(10));
        pos = mouse_pos;
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

    void colabse_with_black_ball()
    {
        std::cout << "BLACK!!!";
        //(*ball2).del();
        sf::Event e;
        window->pollEvent(e);
        float x;
        float y;
        while (e.type != sf::Event::MouseButtonPressed) {
            while (window->pollEvent(e))
            {
                switch (e.type) {
                case sf::Event::Closed: window->close(); break;
                case sf::Event::MouseMoved:
                    x = e.mouseMove.x;
                    y = e.mouseMove.y;
                case sf::Event::MouseButtonPressed:
                    std::vector<float> array1 = { x , y };
                    std::vector<float> array = { 0, 0 };
                    set_pos(array1);
                    set_V(array);
                    break;


                }
            }
        }
    }

    int colabse_holes(int New_Score)
    {
        for (Hole hole : *holes)
        {
            std::vector<float> x1 = pos;
            std::vector<float> x2 = hole.ret_pos();

            if (x1[0] - x2[0] < rad_hole)
            {
                if (x1[1] - x2[1] < rad_hole)
                {
                    if (norm(x1 - x2) < rad_hole)
                    {
                        if ((color) != sf::Color::Black)
                        {
                            std::cout << "COUTch";

                            del();
                            New_Score++;
                        }
                        else
                        {
                            colabse_with_black_ball();
                            New_Score--;

                        }
                    }
                }
            }
            
        }
        return New_Score;
    }
};
