#include <cmath>
#include "Physics.h"


vec2 Physics::GetOverlap(const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b) {
    // return the overlap rectangle size of the bounding boxes of entity a and b
    vec2 posA = a->get<CTransform>().pos;
    vec2 posB = b->get<CTransform>().pos;
    vec2 halfSizeA = a->get<CBoundingBox>().halfSize;
    vec2 halfSizeB = b->get<CBoundingBox>().halfSize;

    vec2 delta{std::abs(posA.x - posB.x), std::abs(posA.y - posB.y)};
    return halfSizeA + halfSizeB - delta;
}

vec2 Physics::GetPreviousOverlap(const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b) {
    // return the previous overlap rectangle size of the bounding boxes of entity a and b
    // previous overlap uses the entity's previous position
    vec2 posA = a->get<CTransform>().prevPos;
    vec2 posB = b->get<CTransform>().prevPos;
    vec2 halfSizeA = a->get<CBoundingBox>().halfSize;
    vec2 halfSizeB = b->get<CBoundingBox>().halfSize;

    vec2 delta{std::abs(posA.x - posB.x), std::abs(posA.y - posB.y)};
    return halfSizeA + halfSizeB - delta;
}

bool Physics::IsInside(const vec2 &pos, const std::shared_ptr<Entity> &entity) {
    vec2 eSize = entity->get<CAnimation>().animation.getSize();
    vec2 ePos = entity->get<CTransform>().pos;

    if (pos.x > ePos.x - eSize.x / 2 &&
        pos.x < ePos.x + eSize.x / 2 &&
        pos.y > ePos.y - eSize.y / 2 &&
        pos.y < ePos.y + eSize.y / 2) {
        // std::cout << entity->getComponent<CAnimation>().animation.name() << "\n";
        return true;
    }

    return false;
}

Intersect Physics::LineIntersect(const vec2 &a, const vec2 &b, const vec2 &c, const vec2 &d) {
    vec2 r = (b - a);
    vec2 s = (d - c);

    float rxs = cross(r, s);
    vec2 cma = c - a;

    float t = cross(cma, s) / rxs;
    float u = cross(cma, r) / rxs;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
        // return {true, vec2(a.x + t * r.x, a.y + t * r.y)};
        return {true, a + t * r};
    else
        return {false, vec2(0, 0)};
}

bool Physics::EntityIntersect(const vec2 &a, const vec2 &b, const std::shared_ptr<Entity> &entity) {
    auto boxC = entity->get<CBoundingBox>().center;
    auto box = entity->get<CBoundingBox>().halfSize;

    vec2 e1{boxC.x - box.x, boxC.y - box.y};
    vec2 e2{boxC.x + box.x, boxC.y - box.y};
    vec2 e3{boxC.x + box.x, boxC.y + box.y};
    vec2 e4{boxC.x - box.x, boxC.y + box.y};

    if (LineIntersect(a, b, e1, e2).intersect ||
        LineIntersect(a, b, e2, e3).intersect ||
        LineIntersect(a, b, e3, e4).intersect ||
        LineIntersect(a, b, e4, e1).intersect) {
        return true;
    }
    return false;
}

vec2 Physics::getSpeedAB(const vec2 &posA, const vec2 &posB, float speed) {
    float theta = std::atan2(posB.y - posA.y, posB.x - posA.x);
    return vec2{speed * std::cos(theta), speed * std::sin(theta)};
}

RectOverlap Physics::AisNearB(const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b, const vec2 &maxDist) {
    ODirection dir = ODirection::NONE;
    vec2 overlap = GetOverlap(a, b);
    vec2 pOverlap = GetPreviousOverlap(a, b);

    float dy = b->get<CTransform>().pos.y - a->get<CTransform>().pos.y;
    if (0 < overlap.x && -maxDist.y < overlap.y && 0 < overlap.y && pOverlap.y <= 0) {
        if (dy > 0) {
            dir = ODirection::UP;
        } else if (dy < 0) {
            dir = ODirection::DOWN;
        }
    }

    float dx = b->get<CTransform>().pos.x - a->get<CTransform>().pos.x;
    if (0 < overlap.y && -maxDist.x < overlap.x && 0 < overlap.x && pOverlap.x <= 0) {
        if (dx > 0) {
            dir = ODirection::LEFT;
        } else if (dx < 0) {
            dir = ODirection::RIGHT;
        }
    }

    return {dir, overlap};
}
