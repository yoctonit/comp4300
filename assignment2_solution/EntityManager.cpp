#include <algorithm>
#include "EntityManager.h"

EntityManager::EntityManager() = default;

// called at beginning of each frame by game engine
// entities added will now be available to use this frame
void EntityManager::update() {
    // - add entities from m_entitiesToAdd to the proper location(s)
    // - add them to the vector of all entities
    // - add them to the vector inside the map, with the tag as a key
    for (const auto &entity: m_entitiesToAdd)
    {
        m_entities.push_back(entity);
        m_entityMap[entity->tag()].push_back(entity);
    }

    // remove dead entities from the vector of all entities
    removeDeadEntities(m_entities);

    // remove dead entities from each vector in the entity map
    // C++20 way ot iterating through [key, value] pairs in a map
    for (auto &[tag, entityVec]: m_entityMap)
    {
        removeDeadEntities(entityVec);
    }

    m_entitiesToAdd.clear();
}

void EntityManager::removeDeadEntities(EntityVec &vec) {
    // remove all dead entities from the input vector
    // this is called by the update() function
    vec.erase(std::remove_if(
                      vec.begin(),
                      vec.end(),
                      [](const pEntity &e)
                      { return !e->isActive(); }
              ),
              vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
    // create a new Entity object
    // store it in the vector of all entities
    // store it in the map of tag->entityVector
    // return the shared pointer pointing to that entity
    // auto e = std::make_shared<Entity>(tag, m_totalEntities++);
    // m_entities.push_back(e);
    // m_entityMap[tag].push_back(e);
    // return e;

    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_entitiesToAdd.push_back(entity);
    return entity;
}
// note: above code doesn't handle some map-related edge cases

EntityVec &EntityManager::getEntities() {
    return m_entities;
}

EntityVec &EntityManager::getEntities(const std::string &tag) {
    // check edge cases
    return m_entityMap[tag];
}

const std::map<std::string, EntityVec> &EntityManager::getEntityMap() {
    return m_entityMap;
}
