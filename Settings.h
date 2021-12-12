#pragma once
#include <SFML/Graphics.hpp>

float speed_player = 0.1f;
int wind_x_size = 1000;
int wind_y_size = 700;
float rad_ball = 15; //20
float rad_hole = 20;
float dt = 0.02f;
float betta_tormoz = 0.6f; //0.1
float tormoz_V = 5.0f;
float time_of_full_beat = 0.7f;
float max_force_of_beat = 1150;
sf::VideoMode		videoMode = sf::VideoMode(wind_x_size, wind_y_size);
sf::RenderWindow* window = new sf::RenderWindow(videoMode, "Billiarrd", sf::Style::Default);


float field_y_size = 100;
float field_x_size = 0.4 * field_y_size;
int number_balls_first_rad = 5;