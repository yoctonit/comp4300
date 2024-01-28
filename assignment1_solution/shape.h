#ifndef COMP4300_SHAPE_H
#define COMP4300_SHAPE_H

#include <SFML/Graphics.hpp>


class Shape {
protected:
    std::string m_type;
    std::string m_name;

    float m_posX, m_posY;
    float m_sizeX, m_sizeY; // we use sizeX and sizeY as a radius for circle shape
    float m_speedX, m_speedY;
    float m_windowWidth, m_windowHeight;

    unsigned char m_R, m_G, m_B;
    float m_color[3];

    int m_fontSize;
    unsigned char m_fontColorR, m_fontColorG, m_fontColorB;

    bool m_visible = true;
    sf::Text m_text;

    float m_scale = 1.0f;

public:
    Shape() = default;

    virtual void Draw(sf::RenderWindow &window) const = 0;

    virtual void Update() = 0;

    std::string Name() const { return m_type + ": " + m_name; }

    bool &Visibility() { return m_visible; }

    float *Color() { return m_color; }

    float *ScaleRef() { return &m_scale; }

    float *VelocityXRef() { return &m_speedX; }
    float *VelocityYRef() { return &m_speedY; }

    virtual void SetColor() = 0;

    void SetName(const char * n) {
        m_text.setString(std::string(n));
    }

    virtual void CalculateNamePosition() {}

//    sf::Drawable getShape();
};

#endif //COMP4300_SHAPE_H
