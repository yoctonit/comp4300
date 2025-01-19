#ifndef COMP4300_SHAPE_H
#define COMP4300_SHAPE_H

#include <SFML/Graphics.hpp>
#include <utility>


class Shape {
protected:
    std::string m_type;
    std::string m_name;
    bool m_visible = true;

public:
    float m_posX, m_posY;
    float m_speedX, m_speedY;
    int m_R, m_G, m_B;
    float m_sizeX, m_sizeY;
    float m_radius{}; // used for circle shape
    float m_scale = 1.0f;

public:
    Shape(
            std::string type,
            std::string name,
            float posX, float posY,
            float speedX, float speedY,
            int red, int green, int blue,
            float sizeX, float sizeY,
            float radius
    ) : m_type(std::move(type)), m_name(std::move(name)),
        m_posX(posX), m_posY(posY),
        m_speedX(speedX), m_speedY(speedY),
        m_R(red), m_G(green), m_B(blue),
        m_sizeX(sizeX), m_sizeY(sizeY), m_radius(radius) {}

    [[nodiscard]] const std::string &Type() const { return m_type; }

    [[nodiscard]] bool IsVisible() const { return m_visible; }

    void SetVisible(bool visible) { m_visible = visible; }

    [[nodiscard]] float GetScale() const { return m_scale; }

    void SetScale(float scale) { m_scale = scale; }

    [[nodiscard]] const std::string &Name() const { return m_name; }

    void SetName(const std::string &name) { m_name = name; }

};

#endif //COMP4300_SHAPE_H
