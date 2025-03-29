#include "Game.h"


int main() {
    // Game g(R"(C:\Users\Ivan\projects\comp4300\cmake-build-release\bin\config_a2.txt)");
    Game g("config_a2.txt");
    g.run();

    return 0;
}
