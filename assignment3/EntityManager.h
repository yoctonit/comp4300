#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <memory>
#include <vector>
#include <map>

#include "Entity.h"


typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;


class EntityManager {
    EntityVec m_entities;       // all entities
    EntityVec m_entitiesToAdd;  // entities to add next update
    EntityMap m_entityMap;      //  map from entity tag to vectors
    size_t m_totalEntities = 0; // total entities created

    // helper function to avoid repeated code
    void removeDeadEntities(EntityVec &vec);

public:
    EntityManager();

    void update();

    std::shared_ptr<Entity> addEntity(const std::string &tag);

    EntityVec &getEntities();

    EntityVec &getEntities(const std::string &tag);

    const EntityMap &getEntityMap();
};

#endif //ENTITY_MANAGER_H
