#include <SFML/Graphics.hpp>
#include "GameEngine.h"


int main() {
    GameEngine g("config/assets.txt");
    g.run();

    return 0;
}
