// World.cpp
//
#include "World.h"

#include "Entity.h"

#include <entt/entt.hpp>

using namespace s2::Scene;

// ------------------------------------------------------------------------------------------------
World::World() 
: _registry( std::make_unique<entt::registry>() )
{}

// ------------------------------------------------------------------------------------------------
World::~World() = default;

// ------------------------------------------------------------------------------------------------
Entity World::createEntity() 
{
    auto enttId = _registry->create();
    return Entity(static_cast<Entity::EntityID>(enttId), this);
}

// ------------------------------------------------------------------------------------------------
void World::destroyEntity(Entity entity) 
{
    _registry->destroy(static_cast<entt::entity>(entity.id()));
}

// ------------------------------------------------------------------------------------------------
void World::clear() 
{
    _registry->clear();
}