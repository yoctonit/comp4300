#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <tuple>

#include "Components.h"

class EntityManager;

typedef std::tuple<
        CTransform,
        CLifespan,
        CDamage,
        CInvincibility,
        CHealth,
        CInput,
        CBoundingBox,
        CAnimation,
        CGravity,
        CState,
        CFollowPlayer,
        CPatrol,
        CDraggable
> ComponentTuple;

class Entity {
    friend class EntityManager;

    bool m_active = true;
    const size_t m_id = 0;
    const std::string m_tag = "default";
    ComponentTuple m_components;

    // constructor is private, so we can never create entities
    // outside the EntityManager which had friend access
    Entity(size_t id, std::string tag);

public:
    void destroy();

    [[nodiscard]] size_t id() const;

    [[nodiscard]] bool isActive() const;

    [[nodiscard]] const std::string &tag() const;

    template<class T>
    bool has() const {
        return get<T>().has;
    }

    template<class T, typename... TArgs>
    T &add(TArgs &&... mArgs) {
        auto &component = get<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<class T>
    T &get() {
        return std::get<T>(m_components);
    }

    template<class T>
    const T &get() const {
        return std::get<T>(m_components);
    }

    template<class T>
    void remove() const {
        get<T>() = T();
    }
};

#endif //ENTITY_H