#ifndef COMP4300_SHAPE_H
#define COMP4300_SHAPE_H

#include <SFML/Graphics.hpp>


class Shape {
public:
    bool m_visible = true;
    sf::Text m_text;

    Shape() = default;

    virtual void Draw(sf::RenderWindow& window) const = 0;
};

#endif //COMP4300_SHAPE_H
