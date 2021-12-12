#pragma once
#include "Settings.h"
#include "Operator.h"
#include "Ball.h"


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
                    if (norm(x1 - x2) < rad_hole / 2)
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
                            //(*ball2).del();
                            sf::Event e;
                            window->pollEvent(e);
                            float   x;
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
                                        (*ball2).set_pos(array1);
                                        (*ball2).set_V(array);
                                        break;


                                    }
                                }
                            }
                            New_Score--;

                        }
                    }
                }
            }
        }
        return New_Score;
    }
};
