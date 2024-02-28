#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <utility>

#include "Animation.h"
#include "Assets.h"

class Component {
public:
    bool has = false;
};

class CTransform : public Component {
public:
    vec2 pos = {0.0, 0.0};
    vec2 prevPos = {0.0, 0.0};
    vec2 velocity = {0.0, 0.0};
    vec2 scale = {1.0, 1.0};
    float angle = 0;

    CTransform() = default;

    explicit CTransform(const vec2 &p) : pos(p) {}

    CTransform(const vec2 &p, const vec2 &sp, const vec2 &sc, float a)
            : pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}
};

class CLifespan : public Component {
public:
    int lifespan = 0;
    int frameCreated = 0;

    CLifespan() = default;

    explicit CLifespan(int duration, int frame)
            : lifespan(duration), frameCreated(frame) {}
};

class CInput : public Component {
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shoot = false;
    bool canShoot = true;
    bool canJump = true;

    CInput() = default;
};

class CBoundingBox : public Component {
public:
    vec2 size;
    vec2 halfSize;

    CBoundingBox() = default;

    explicit CBoundingBox(const vec2 &s) : size(s), halfSize(s.x / 2.0f, s.y / 2.0f) {}
};

class CAnimation : public Component {
public:
    Animation animation;
    bool repeat = false;

    CAnimation() = default;

    CAnimation(Animation a, bool r) : animation(std::move(a)), repeat(r) {}
};

class CGravity : public Component {
public:
    float gravity = 0;

    CGravity() = default;

    explicit CGravity(float g) : gravity(g) {}
};

class CState : public Component {
public:
    std::string state = "jumping";

    CState() = default;

    explicit CState(std::string s) : state(std::move(s)) {}
};

#endif //COMPONENTS_H
