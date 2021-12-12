#include <algorithm>
#include <array>
#include <iostream>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Settings.h"
#include "Classes/Operator.h"
#include "Classes/Ball.h"
#include "Classes/Hole.h"
#include "Classes/Button.h"
#include "Classes/Menu.h"
#include "Classes/Gui.h"
#include "Classes/Game.h"


int main()
{
    Game game = Game();
    game.manager();

    return 0;
}