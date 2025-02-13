#pragma once
#include"Common.h"

class Entity {

    friend class EntityManager;

    const size_t m_ID = 0;
    const std::string m_tag = "default";
    bool m_alive = true;
    
    Entity(const std::string& tag, const size_t ID);

public:
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CInput> cInput;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CLifespan> cLifespan;
    //if want collection based: tuple<CTransform, CName, CShape, CBBox> m_components;
  
    bool isAlive() const;
    const std::string& tag() const;
    const size_t& ID() const;
    void destroy();
};