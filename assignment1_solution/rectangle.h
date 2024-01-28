#ifndef COMP4300_RECTANGLE_H
#define COMP4300_RECTANGLE_H

#include "config.h"
#include "shape.h"


class Rectangle : public Shape {
    RectangleConfig m_config;
    sf::RectangleShape m_shape;
public:
    explicit Rectangle(const RectangleConfig& sc, const FontConfig &fc, sf::Font &font, WindowConfig wc);

    void Draw(sf::RenderWindow &window) const override;

    void Update() override;

    void SetColor() override;

    void CalculateNamePosition() override;
};

#endif //COMP4300_RECTANGLE_H
