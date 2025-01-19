#ifndef COMP4300_A1_CONFIG_H
#define COMP4300_A1_CONFIG_H

#include <iostream>
#include <string>
#include <vector>


struct WindowConfig {
    int width, height;
};

struct FontConfig {
    std::string file;
    int size;
    int R, G, B;
};

struct ShapeConfig {
    std::string type;
    std::string name;
    float posX, posY;
    float speedX, speedY;
    int R, G, B;
    float sizeX, sizeY{};
    float radius{};
};

class Config {
public:
    explicit Config(const std::string &file);

    friend std::ostream &operator<<(std::ostream &os, const Config &config);

    WindowConfig window;
    FontConfig font;
    std::vector<ShapeConfig> shapes;
};

#endif //COMP4300_A1_CONFIG_H
