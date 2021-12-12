#include <algorithm>
#include <array>
#include <iostream>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Settings.h"
#include "Operator.h"
#include "Ball.h"
#include "Hole.h"
#include "Button.h"
#include "Menu.h"
#include "Gui.h"
#include "Game.h"


int main()
{
    Game game = Game();
    game.manager();

    return 0;
}