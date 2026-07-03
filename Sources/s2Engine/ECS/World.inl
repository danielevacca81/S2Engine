// World.inl
//
#pragma once

#include <entt/entt.hpp>

namespace s2 {
namespace ECS {

// ------------------------------------------------------------------------------------------------
template<typename T, typename... Args>
T& World::addProperty(Entity entity, Args&&... args) 
{
    return _registry->emplace<T>(static_cast<entt::entity>(entity.id()), std::forward<Args>(args)...);
}

// ------------------------------------------------------------------------------------------------
template<typename T>
T& World::property(Entity entity) 
{
    return _registry->get<T>(static_cast<entt::entity>(entity.id()));
}

// ------------------------------------------------------------------------------------------------
template<typename T>
bool World::hasProperty(Entity entity) const 
{
    return _registry->all_of<T>(static_cast<entt::entity>(entity.id()));
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void World::removeProperty(Entity entity) 
{
    _registry->remove<T>(static_cast<entt::entity>(entity.id()));
}

// ------------------------------------------------------------------------------------------------
// View implementation: Converts EnTT's view callback to use our custom Entity class
template<typename... Properties>
void World::each(std::function<void(Entity, Properties&...)> func)
{
    auto view = _registry->view<Properties...>();
    for (auto enttId : view) 
    {
        Entity entity(static_cast<Entity::EntityID>(enttId), this);
        func(entity, view.template get<Properties>(enttId)...);
    }
}

} // namespace ECS
} // namespace s2