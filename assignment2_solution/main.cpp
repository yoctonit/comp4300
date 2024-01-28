#include <SFML/Graphics.hpp>

#include "Game.h"


int main() {
    Game g("config.txt");
    g.run();

    return 0;
}
