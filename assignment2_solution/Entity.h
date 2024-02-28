#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <string>

#include "Components.h"


class Entity {
    friend class EntityManager;

    const size_t m_id = 0;
    const std::string m_tag = "default";
    bool m_active = true;

    Entity(size_t id, std::string tag);

public:
    // component pointers
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput> cInput;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CLifespan> cLifespan;

    [[nodiscard]] bool isActive() const;

    [[nodiscard]] const std::string &tag() const;

    [[nodiscard]] size_t id() const;

    void destroy();
};

#endif //ENTITY_H
