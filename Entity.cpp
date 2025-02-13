#include"Entity.h"

Entity::Entity(const std::string& tag, const size_t id) : m_ID(id), m_tag(tag) {};

bool Entity::isAlive() const{
	return m_alive;
}

const std::string& Entity::tag() const {
	return m_tag;
}

const size_t& Entity::ID() const {
	return m_ID;
}

void Entity::destroy() {
	m_alive = 0;
}

