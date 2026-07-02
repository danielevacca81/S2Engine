// World.h
//
#ifndef S2_SCENE_WORLD_H
#define S2_SCENE_WORLD_H

#include "s2Engine_API.h"

#include "Entity.h"

#include <entt/fwd.hpp>

#include <memory>
#include <functional>

namespace s2 {
namespace Scene {

class S2ENGINE_API World 
{
public:
    World();
    ~World();

    Entity createEntity();
    void destroyEntity(Entity entity);
    void clear();

    // Component Management (Called by Entity or directly by Systems)
    template<typename T, typename... Args>
    T& addProperty(Entity entity, Args&&... args);

    template<typename T>
    T& property(Entity entity);

    template<typename T>
    bool hasProperty(Entity entity) const;

    template<typename T>
    void removeProperty(Entity entity);

    // System View Utility
    template<typename... Properties>
    void each(std::function<void(Entity, Properties&...)> func);

private:
    std::unique_ptr<entt::registry> _registry;
};

} // namespace Scene
} // namespace s2

#include "World.inl"

#endif // !S2_SCENE_WORLD_H