#ifndef PHYSICS_H
#define PHYSICS_H

#include <memory>
#include "Entity.h"
#include "vec2.h"


class Physics {
public:
    vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

    vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

};

#endif //PHYSICS_H
