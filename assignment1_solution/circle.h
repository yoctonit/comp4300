#ifndef COMP4300_CIRCLE_H
#define COMP4300_CIRCLE_H

#include "config.h"
#include "shape.h"


class Circle : public Shape {
    CircleConfig m_config;
    sf::CircleShape m_shape;
public:
    explicit Circle(CircleConfig rc, sf::Font& font);

    void Draw(sf::RenderWindow& window) const override;
};

#endif //COMP4300_CIRCLE_H
