#pragma once
#include "Settings.h"
#include "Classes/Operator.h"
#include "Classes/Ball.h"


class Hole
{
private:
    float rad = rad_hole;
    sf::CircleShape shape;
    sf::Color color;
    std::vector<float> pos = std::vector<float>(2); // Coords x, y


public:
    Hole(std::vector<float> pos) :
        pos(pos), shape(rad), color(sf::Color::Red)
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


};
