#ifndef COMP4300_RECTANGLE_H
#define COMP4300_RECTANGLE_H

#include "config.h"
#include "shape.h"


class Rectangle : public Shape {
    RectangleConfig m_config;
    sf::RectangleShape m_shape;
public:
    explicit Rectangle(RectangleConfig rc, sf::Font& font);

    void Draw(sf::RenderWindow& window) const override;
};

#endif //COMP4300_RECTANGLE_H