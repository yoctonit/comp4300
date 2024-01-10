#ifndef COMP4300_CONFIG_H
#define COMP4300_CONFIG_H

#include <string>
#include <vector>


struct WindowConfig
{
    int width, height;
};

struct FontConfig
{
    std::string file;
    int size;
    int R, G, B; // color
};

struct RectangleConfig
{
    std::string name;
    float posX, posY;
    float speedX, speedY;
    int R, G, B; // color
    float sizeX, sizeY;
};

struct CircleConfig
{
    std::string name;
    float posX, posY;
    float speedX, speedY;
    int R, G, B; // color
    float radius;
};

class Config
{
private:
    WindowConfig m_windowConfig;
    FontConfig m_fontConfig;
    std::vector<RectangleConfig> m_rectangles;
    std::vector<CircleConfig> m_circles;

public:
    explicit Config(const std::string &file);

    [[nodiscard]] WindowConfig Window() const;

    [[nodiscard]] FontConfig Font() const;

    [[nodiscard]] const std::vector<RectangleConfig> &Rectangles() const;

    [[nodiscard]] const std::vector<CircleConfig> &Circles() const;

    void Print() const;
};

#endif //COMP4300_CONFIG_H
