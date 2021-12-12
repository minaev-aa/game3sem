#pragma once


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
    bool gameover = false;

public:

    Game()
    {
        field.setSize(sf::Vector2f(field_x_size / 100 * wind_x_size, field_y_size / 100 * wind_y_size));
        field.setPosition(wind_x_size / 2 - field_x_size / 100 * wind_x_size / 2, 0);

        mouse_pos = std::vector <float>{ -10, -10 };
        gui = new GUI();

        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j += 2)
            {
                std::cout << field_x_size;
                std::vector<float> pos_right{ float((wind_x_size + j * field_x_size * 10) / 2 - j * rad_hole), float(wind_y_size / 2 + i * (wind_y_size / 2 - rad_hole)) };
                Hole hole_right(pos_right);
                hole_right.set_color(sf::Color::Yellow);
                holes.push_back(hole_right);
                Hole hole1(pos_right);
                hole_right.set_color(sf::Color::Yellow);
                holes.push_back(hole_right);
            }
        }
        std::vector<float> pos1{ float(wind_x_size) / 2, float(wind_y_size) / 2 };
        Ball ball1(pos1, field, &balls, &holes);
        ball1.set_color(sf::Color::Black);

        balls.push_back(ball1);

        for (int i = 0; i < number_balls_first_rad; i++)
        {
            float y = float(wind_y_size) / 2 - rad_ball * pow(3.0, 0.5) * i - i - 100;
            for (int j = 0; j <= i; j++)
            {
                if (i % 2 == 1)
                {
                    float x = float(wind_x_size) / 2 - (i + 1) / 2 * rad_ball * 2 + rad_ball + j * 2 * rad_ball;
                    std::vector<float> pos{ x,y };
                    Ball ball(pos, field, &balls, &holes);
                    balls.push_back(ball);

                }
                else
                {
                    float x = float(wind_x_size) / 2 - (i) / 2 * rad_ball * 2 + j * 2 * rad_ball;
                    std::vector<float> pos{ x,y };
                    Ball ball(pos, field, &balls, &holes);
                    balls.push_back(ball);

                }

            }
        }

        start_balls = balls;
        line = sf::VertexArray(sf::Lines, 2);
    };

    void render()
    {
        if (true) {
            std::vector<Ball> new_balls;
            for (Ball main_ball : balls)
            {
                main_ball.move();
                New_Score = main_ball.colabse_holes(New_Score);
                new_balls.push_back(main_ball);
                if (New_Score != 0)
                {
                    gui->addPoints(gui->getCurrentPlayer(), New_Score);
                    New_Score = 0;
                }
                for (Ball main_ball1 : balls) {
                    if (udar && main_ball1.ret_speed() == std::vector<float>(2) && main_ball1.ret_color() == sf::Color::Black)
                    {
                        gui->setCurrentPlayer(1 - gui->getCurrentPlayer().number);
                        udar = false;
                    }
                    if (main_ball1.ret_speed() != std::vector<float>(2) && main_ball1.ret_color() == sf::Color::Black)
                    {
                        gui->setCurrentPlayer(1 - gui->getCurrentPlayer().number);
                        udar = true;
                        break;
                    }
                }
            }
            balls = new_balls;
            window->clear(sf::Color(75, 0, 163));
        }
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
        restartGame = false;
        gameover = false;
        balls = start_balls;
        gui->restart();
    }

    void event_update()
    {
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
                if (!gui->gamePaused && !gameover) {

                    mouse_pos[0] = m_mouse.x;
                    mouse_pos[1] = m_mouse.y;

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {

                        for (int i = 0; i < balls.size(); i++)
                        {
                            if (balls[i].is_clicked(mouse_pos))
                            {
                                if (balls[i].ret_color() == sf::Color::Black)
                                    selected_ball = i;
                            }
                        }

                        if (selected_ball != -1) {
                            force_beat = std::min(norm((balls[selected_ball].ret_pos() - mouse_pos)) / 200, 1.0f) * max_force_of_beat;
                            std::vector <float> vect = (balls[selected_ball].ret_pos() - mouse_pos) * (1 / norm(balls[selected_ball].ret_pos() - mouse_pos));
                            line[0].position = sf::Vector2f((balls[selected_ball].ret_pos() + 2 * rad_ball * vect)[0],
                                (balls[selected_ball].ret_pos() + 2 * rad_ball * vect)[1]);
                            line[0].color = sf::Color::Blue;
                            line[1].position = sf::Vector2f((balls[selected_ball].ret_pos() + (2 + 7 * force_beat / max_force_of_beat) * rad_ball * vect)[0],
                                (balls[selected_ball].ret_pos() + (2 + 7 * force_beat / max_force_of_beat) * rad_ball * vect)[1]);
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
            int i = 0;
            for (auto ball : balls)
            {
                if (!ball.not_del)
                    i++;
            }
            if (i == number_balls_first_rad * (number_balls_first_rad + 1) / 2)
            {
                gameover = true;
                gui->gamePaused = true;
                gui->set_gameover(true);
            }

            render();
            event_update();

            gui->update(window, gui->gamePaused, restartGame, m_mouse, sfmlEvent);
            window->draw(*gui);
            window->display();

            if (restartGame)
            {
                restart_game();
                gui->setCurrentPlayer(gui->player2);
            }

        }
    }
};