#pragma once
#include"Common.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager {
    EntityVec m_entities;
    EntityVec m_entitiestoAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

    void removeDeadEntities(EntityVec& vec);

public:
    EntityManager();

    void Update();

    std::shared_ptr<Entity> addEntity(const std::string& tag);

    const EntityVec& getEntities();
    const EntityVec& getEntities(const std::string& tag);
    
};