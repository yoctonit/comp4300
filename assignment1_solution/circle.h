#ifndef COMP4300_CIRCLE_H
#define COMP4300_CIRCLE_H

#include "config.h"
#include "shape.h"


class Circle : public Shape {
private:
    sf::CircleShape m_shape;

public:
    explicit Circle(const CircleConfig& sc, const FontConfig &fc, sf::Font &font, WindowConfig wc);

    void Draw(sf::RenderWindow &window) const override;

    void Update() override;

    void SetColor() override;

    void CalculateNamePosition() override;
};

#endif //COMP4300_CIRCLE_H
