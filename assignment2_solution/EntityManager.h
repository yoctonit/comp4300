#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <memory>
#include <vector>
#include <map>

#include "Entity.h"


typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;


class EntityManager {
    EntityVec m_entities;
    EntityVec m_entitiesToAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

    void removeDeadEntities(EntityVec &vec);

    // void init();

public:
    EntityManager();

    void update();

    std::shared_ptr<Entity> addEntity(const std::string &tag);

    EntityVec & getEntities();

    EntityVec &getEntities(const std::string &tag);

    const std::map<std::string, EntityVec> &getEntityMap();
};

#endif //ENTITY_MANAGER_H
