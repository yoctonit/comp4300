#include <algorithm>
#include "EntityManager.h"

EntityManager::EntityManager() = default;

// called at beginning of each frame by game engine
// entities added will now be available to use this frame
void EntityManager::update() {
    // Add entities from m_entitiesToAdd to the proper location(s):
    // - add them to the vector of all entities
    // - add them to the vector inside the map, with the tag as a key
    for (const auto &entity: m_entitiesToAdd) {
        m_entities.push_back(entity);
        m_entityMap[entity->tag()].push_back(entity);
    }
    m_entitiesToAdd.clear();

    // remove dead entities from the vector of all entities
    removeDeadEntities(m_entities);

    // remove dead entities from each vector in the entity map
    // C++20 way ot iterating through [key, value] pairs in a map
    for (auto &[tag, entityVec]: m_entityMap) {
        removeDeadEntities(entityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec &vec) {
    // remove all dead entities from the input vector
    // this is called by the update() function
    std::erase_if(
            vec,
            [](const std::shared_ptr<Entity> &entity) {
                return !entity->isActive();
            }
    );

// My solution from internet :)
//    vec.erase(std::remove_if(vec.begin(),
//                             vec.end(),
//                             [](const Entity &e) { return !e.isActive(); }),
//              vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_entitiesToAdd.push_back(entity);

    return entity;
}

EntityVec &EntityManager::getEntities() {
    return m_entities;
}

EntityVec &EntityManager::getEntities(const std::string &tag) {
    return m_entityMap[tag];
}

const EntityMap &EntityManager::getEntityMap() {
    return m_entityMap;
}
