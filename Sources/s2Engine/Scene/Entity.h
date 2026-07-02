// Entity.h
//
#ifndef S2_SCENE_ENTITY_H
#define S2_SCENE_ENTITY_H

#include "s2Engine_API.h"

#include <cstdint>

namespace s2 {
namespace Scene {

class World;

class S2ENGINE_API Entity 
{
public:
    using EntityID = uint32_t;
    inline static const EntityID EntityInvalidID = 0xFFFFFFFF; // Matches entt::null

public:
    Entity() = default;
    Entity(EntityID id, World* world) 
    : _id(id)
    , _world(world)
    {}

    EntityID id() const { return _id; }
    void destroy();
    bool isValid() const;
    bool operator==(const Entity& other) const { return _id == other._id && _world == other._world; }
    bool operator!=(const Entity& other) const { return !(*this == other); }

    template<typename T, typename... Args>
    T& addProperty(Args&&... args);

    template<typename T>
    T& property();

    template<typename T>
    bool hasProperty() const;

    template<typename T>
    void removeProperty();


private:
    EntityID _id { EntityInvalidID };
    World* _world {nullptr};
};

}
}

#endif // !S2_SCENE_ENTITY_H