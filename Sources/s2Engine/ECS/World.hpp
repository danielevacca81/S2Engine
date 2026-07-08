// World.hpp
//
#pragma once

#include <entt/entt.hpp>

namespace s2 {
namespace ECS {

// ------------------------------------------------------------------------------------------------
template<typename T, typename... Args>
decltype(auto) World::addProperty(Entity entity, Args&&... args) 
{
    return _registry->emplace<T>(static_cast<entt::entity>(entity.id()), std::forward<Args>(args)...);
}

// ------------------------------------------------------------------------------------------------
template<typename T>
decltype(auto) World::property(Entity entity) 
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
    each<std::function<void(Entity, Properties&...)>, Properties...>(std::move(func));
}

// ------------------------------------------------------------------------------------------------
template<typename... Properties, typename Func>
void World::each(Func&& func)
{
    auto view = _registry->view<Properties...>();
    view.each([this, &func](auto enttId, auto&... components) {
        
        Entity entity(static_cast<Entity::EntityID>(enttId), this);
        func(entity, components...);
        
    });


    // for (auto enttId : view)
    // {
    //     Entity entity(static_cast<Entity::EntityID>(enttId), this);
    //     std::forward<Func>(func)(entity, view.template get<Properties>(enttId)...);
    // }
}

} // namespace ECS
} // namespace s2