#include"EntityManager.h"

EntityManager::EntityManager() {}

void EntityManager::Update() {

    for (auto e : m_entitiestoAdd) {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }
    m_entitiestoAdd.clear();

    removeDeadEntities(m_entities);
    
    for (auto& [tag, entityVec] : m_entityMap) {
        removeDeadEntities(entityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec& vec) {
    for (auto e : vec) {
        vec.erase(std::remove_if(vec.begin(), vec.end(), [](std::shared_ptr<Entity> e) {return !e->isAlive(); }), 
            vec.end());
    }
}

//bool not_alive(std::shared_ptr<Entity> e) {
    //return !e->isAlive();
//} lambda function above is essentially this

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {
    auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));
    m_entitiestoAdd.push_back(e);
    return e;
}

const EntityVec& EntityManager::getEntities() {
    return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag) {
    return m_entityMap[tag];
}