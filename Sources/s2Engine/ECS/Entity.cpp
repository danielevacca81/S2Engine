// Entity.cpp
//
#include "Entity.h"

#include "World.h"

using namespace s2::ECS;

// ------------------------------------------------------------------------------------------------
bool Entity::isValid() const 
{
    return _id != EntityInvalidID && _world != nullptr; // Add custom registry validation if needed
}

// ------------------------------------------------------------------------------------------------
void Entity::destroy() 
{
    _world->destroyEntity(*this);
    _id = EntityInvalidID;
    _world = nullptr;
}

// ------------------------------------------------------------------------------------------------
template<typename T, typename... Args>
T& Entity::addProperty(Args&&... args) { return _world->addProperty<T>(*this, std::forward<Args>(args)...); }

// ------------------------------------------------------------------------------------------------
template<typename T>
T& Entity::property() { return _world->property<T>(*this); }

// ------------------------------------------------------------------------------------------------
template<typename T>
bool Entity::hasProperty() const { return _world->hasProperty<T>(*this); }

// ------------------------------------------------------------------------------------------------
template<typename T>
void Entity::removeProperty() { _world->removeProperty<T>(*this); }