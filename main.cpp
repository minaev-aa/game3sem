#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <algorithm>
#include <math.h>
#include <vector>
//#include "settings.cpp"

float speed_player = 0.1;
int wind_x_size = 1000;
int wind_y_size = 700;
float rad_ball = 20;
float dt = 0.01;
float betta_tormoz = 0.02;
sf::RenderWindow window(sf::VideoMode(wind_x_size, wind_y_size), "SFML works!");

float field_y_size = 100; // в процентах от wind_y_size
float field_x_size = 0.4 * field_y_size;
//sf::RectangleShape field;

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
void print_vec(std::vector<T> vec)
{
    for (auto el : vec)
    {
        std::cout << el << " ";
    }
}


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
        std::cout << "field_x1 " << field_x1;
    };

    void draw()
    {
        window.draw(shape);
    };

    void move()
    {
        pos = pos + V * dt;
        V = V + a * dt;
        a = float(-1) * V * betta_tormoz;
        colapse_wall();
        shape.setPosition(pos[0] - rad, pos[1] - rad);
    };

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

    void set_V(std::vector<float> new_V )
    {
        V = new_V;
    }
};

int main()
{
    sf::RectangleShape field;
    field.setSize(sf::Vector2f(field_x_size/100 * wind_x_size, field_y_size / 100 * wind_y_size));
    field.setPosition(wind_x_size / 2 - field_x_size/100 * wind_x_size / 2, 0);
    
    std::vector<float> pos1{ 500, 100 };
    std::vector<float> V1{ 50, 100 };
    Ball ball1(pos1, field);
    ball1.set_V(V1);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            //pl.move(speed_player*(-1), 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            //pl.move(speed_player * (1), 0);
        }

        ball1.move();
       

        window.clear();
        window.draw(field);
        ball1.draw();
        window.display();
    }

    return 0;
}