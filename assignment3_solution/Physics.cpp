#include "Physics.h"


vec2 Physics::GetOverlap(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) {
    // return the overlap rectangle size of the bounding boxes of entity a and b
    vec2 posA = a->getComponent<CTransform>().pos;
    vec2 posB = b->getComponent<CTransform>().pos;
    vec2 halfSizeA = a->getComponent<CBoundingBox>().halfSize;
    vec2 halfSizeB = b->getComponent<CBoundingBox>().halfSize;

    vec2 delta{ std::abs(posA.x - posB.x), std::abs(posA.y - posB.y) };
//    float overlapX = halfSizeA.x + halfSizeB.x - delta.x;
//    float overlapY = halfSizeA.y + halfSizeB.y - delta.y;
//
//    return {overlapX, overlapY};
    return halfSizeA + halfSizeB - delta;
}

vec2 Physics::GetPreviousOverlap(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) {
    // return the previous overlap rectangle size of the bounding boxes of entity a and b
    // previous overlap uses the entity's previous position
    vec2 posA = a->getComponent<CTransform>().prevPos;
    vec2 posB = b->getComponent<CTransform>().prevPos;
    vec2 halfSizeA = a->getComponent<CBoundingBox>().halfSize;
    vec2 halfSizeB = b->getComponent<CBoundingBox>().halfSize;

    vec2 delta{ std::abs(posA.x - posB.x), std::abs(posA.y - posB.y) };
//    float overlapX = halfSizeA.x + halfSizeB.x - delta.x;
//    float overlapY = halfSizeA.y + halfSizeB.y - delta.y;
//
//    return {overlapX, overlapY};
    return halfSizeA + halfSizeB - delta;
}
