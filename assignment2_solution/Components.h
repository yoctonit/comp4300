#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <SFML/Graphics.hpp>

#include "vec2.h"


class CTransform {
public:
    vec2 pos = {0.0, 0.0};
    vec2 velocity = {0.0, 0.0};
    float angle = 0;

    CTransform(const vec2 &p, const vec2 &v, float a)
            : pos(p), velocity(v), angle(a) {}
};


class CShape {
public:
    sf::CircleShape circle;

    CShape(float radius, int points, const sf::Color &fill, const sf::Color outline, float thickness)
            : circle(radius, points) {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
    }
};


class CCollision {
public:
    float radius = 0;

    explicit CCollision(float r) : radius(r) {}
};


class CScore {
public:
    int score = 0;

    explicit CScore(int s) : score(s) {}
};


class CLifespan {
public:
    int remaining = 0;  // amount of lifespan remaining on the entity
    int total = 0;      // the total initial amount of lifespan
    explicit CLifespan(int total)
            : remaining(total), total(total) {}
};


class CInput {
public:
    bool up = false;
    bool left = false;
    bool right = false;
    bool down = false;
    bool shoot = false;

    CInput() = default;
};

#endif //COMPONENTS_H
