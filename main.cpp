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
float rad_ball = 20; //20
float dt = 0.01;
float betta_tormoz = 0.1; //0.1
float tormoz_V = 0.8;
sf::RenderWindow window(sf::VideoMode(wind_x_size, wind_y_size), "SFML works!");

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
        window.draw(shape);
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
                    //std::cout << "COLAPSE";
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

    std::vector<Ball> balls = { ball1, ball2, ball3, ball4 };

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
        
        std::vector<Ball> new_balls;
        for (Ball main_ball : balls)
        {
            main_ball.move(balls);
            new_balls.push_back(main_ball);
        }
        balls = new_balls;
       

        window.clear();
        window.draw(field);
        for (Ball ball : balls)
        {
            ball.draw();
        }
        window.display();
    }

    return 0;
}