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
    vec2 pos = {0.0f, 0.0f};
    vec2 prevPos = {0.0f, 0.0f};
    vec2 velocity = {0.0f, 0.0f};
    vec2 scale = {1.0f, 1.0f};
    vec2 facing = {0.0f, 1.0f};
    float angle = 0;

    CTransform() = default;

    explicit CTransform(const vec2 &p) : pos(p), prevPos(p) {}

    CTransform(const vec2 &p, const vec2 &speed, const vec2 &s, float a)
            : pos(p), prevPos(p), velocity(speed), scale(s), angle(a) {}
};

class CLifespan : public Component {
public:
    int lifespan = 0;
    int frameCreated = 0;

    CLifespan() = default;

    explicit CLifespan(int duration, int frame)
            : lifespan(duration), frameCreated(frame) {}
};

class CDamage : public Component {
public:
    int damage = 1;

    CDamage() = default;

    explicit CDamage(int d) : damage(d) {}
};

class CInvincibility : public Component {
public:
    int iframes = 1;

    CInvincibility() = default;

    explicit CInvincibility(int f) : iframes(f) {}
};

class CHealth : public Component {
public:
    int max = 1;
    int current = 1;

    CHealth() = default;

    CHealth(int m, int c) : max(m), current(c) {}
};

class CInput : public Component {
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool attack = false;
    // bool canAttack = true;

    CInput() = default;
};

class CBoundingBox : public Component {
public:
    vec2 size;
    vec2 halfSize;
    vec2 center;
    vec2 prevCenter;
    bool blockMove = false;
    bool blockVision = false;

    CBoundingBox() = default;

    CBoundingBox(const vec2 &c, const vec2 &s, bool m, bool v)
            : size(s), halfSize(s.x / 2.0f, s.y / 2.0f), center(c), prevCenter(c), blockMove(m), blockVision(v) {}
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
    std::string state = "stand";
    std::string previousState = "stand";
    bool changeAnimation = false;

    CState() = default;

    explicit CState(std::string s) : state(std::move(s)) {}
};

class CFollowPlayer : public Component {
public:
    vec2 home = {0.0f, 0.0f};
    float speed = 0.0f;

    CFollowPlayer() = default;

    CFollowPlayer(vec2 p, float s) : home(p), speed(s) {}
};

class CPatrol : public Component {
public:
    std::vector<vec2> positions;
    size_t currentPosition = 0;
    float speed = 0;

    CPatrol() = default;

    CPatrol(std::vector<vec2> &pos, float s) : positions(pos), speed(s) {}
};

class CDraggable : public Component {
public:
    bool dragging = false;

    CDraggable() = default;
};

#endif //COMPONENTS_H
