#ifndef PHYSICS_H
#define PHYSICS_H

#include <memory>

#include "Entity.h"
#include "vec2.h"


struct Intersect {
    bool intersect = false;
    vec2 position = {0.0f, 0.0f};
};

enum struct ODirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

struct RectOverlap {
    ODirection direction = ODirection::NONE;
    vec2 overlap = {0.0f, 0.0f};
};

class Physics {
public:
    static vec2 GetOverlap(const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b);

    static vec2 GetPreviousOverlap(const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b);

    static bool IsInside(const vec2 &pos, const std::shared_ptr<Entity> &entity);

    static Intersect LineIntersect(const vec2 &a, const vec2 &b, const vec2 &c, const vec2 &d);

    static bool EntityIntersect(const vec2 &a, const vec2 &b, const std::shared_ptr<Entity> &entity);

    static RectOverlap AisNearB(const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b, const vec2 &maxDist);

    static vec2 getSpeedAB(const vec2 &posA, const vec2 &posB, float speed);
};

#endif //PHYSICS_H
