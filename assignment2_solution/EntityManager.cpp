#include "EntityManager.h"

EntityManager::EntityManager() = default;

// called at beginning of each frame by game engine
// entities added will now be available to use this frame
void EntityManager::update() {
    // TODO:
    // - add entities from m_entitiesToAdd to the proper location(s)
    // - add them to the vector of all entities
    // - add them to the vector inside the map, with the tag as a key
//    for (auto e: m_entitiesToAdd) {
//        m_entities.push_back(e);
//        m_entityMap[e->tag].push_back(e);
//    }

    // remove dead entities from the vector of all entities
    removeDeadEntities(m_entities);

    // remove dead entities from each vector in the entity map
    // C++20 way ot iterating through [key, value] pairs in a map
//    for (auto &[tag, entityVec]: m_entityMap) {
//        removeDeadEntities(entityVec);
//    }

    m_entitiesToAdd.clear();
}

void EntityManager::removeDeadEntities(EntityVec &vec) {
    // TODO: remove all dead entities from the input vecotr
    // this is called by the update() function

    // for (auto e: m_entities)
    // {
    //     // if e is dead, remove it from m_entities
    //     // if e is dead, remove it from m_entityMap[e->tag()]
    //     if (! e->m_alive)
    //     {
    //         // ...
    //     }
    // }
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
    // // create a new Entity object
    // // store it in the vector of all entities
    // // store it in the map of tag->entityvector
    // // return the shared pointer pointing to that entity
    // auto e = std::make_shared<Entity>(tag, m_totalEntities++);
    // m_entities.push_back(e);
    // m_entityMap[tag].push_back(e);
    // return e;

    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_entitiesToAdd.push_back(entity);
    return entity;
}
// note: above code doesn't handle some map-related edge cases

EntityVec & EntityManager::getEntities() {
    return m_entities;
}

EntityVec &EntityManager::getEntities(const std::string &tag) {
    // TODO: check edge cases
    return m_entityMap[tag];
}

const std::map<std::string, EntityVec> &EntityManager::getEntityMap() {
    return m_entityMap;
}

// Iterator Invalidation Example
// void sCollision()
// {
//     EntityVec bullets;
//     EntityVec tiles;
//     for (auto & b: bullets)
//         for (auto & t: tiles)
//             if(Physics::IsCollision(b,t)) bullets.erase(b);
// }
// Solution: Delayed Effects
// Idea: only add or remove entities at the beginning of a frame when it is safe
