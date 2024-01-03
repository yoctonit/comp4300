#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <map>


typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;


// ------------------------------
// EntityManager
// ------------------------------
// - m_entities: vector<Entity>
// - m_entityMap: map<string, Entity>
// - m_toAdd: vector<Entity>
// ------------------------------
// - init: void
// + update: void
// + addEntity(args): sp<Entity>
// + getEntities(): vector<sp<Entity>>&
// + getEntities(s): vector<sp<Entity>>&
// ------------------------------
class EntityManager
{
    EntityVec m_entities;
    EntityVec m_entitiesToAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

    void removeDeadEntities(EntityVec & vec);

public:
    EntityManager();

    void update();

    std::shared_ptr<Entity> addEntity(const std::string& tag);
    
    EntityVec& getEntities();
    EntityVec& getEntities(const std::string& tag);
    const std::map<std::string, EntityVec>& getEntityMap();
};

#endif //ENTITY_MANAGER_H
