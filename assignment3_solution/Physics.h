#ifndef PHYSICS_H
#define PHYSICS_H

#include <memory>
#include "Entity.h"
#include "vec2.h"


class Physics {
public:
    static vec2 GetOverlap(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b);

    static vec2 GetPreviousOverlap(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b);

};

#endif //PHYSICS_H
