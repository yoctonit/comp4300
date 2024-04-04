#include "Entity.h"


Entity::Entity(const size_t i, std::string t)
        : m_id(i), m_tag(std::move(t)) {}

bool Entity::isActive() const {
    return m_active;
}

const std::string &Entity::tag() const {
    return m_tag;
}

size_t Entity::id() const {
    return m_id;
}

void Entity::destroy() {
    m_active = false;
}