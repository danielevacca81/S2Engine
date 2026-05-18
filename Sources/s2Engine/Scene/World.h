// World.h
//
#ifndef S2_SCENE_WORLD_H
#define S2_SCENE_WORLD_H

#include "s2Engine_API.h"
#include "Entity.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

namespace s2 {
namespace Scene {

class S2ENGINE_API World
{
public:
    World( const std::string& name = "World" );
    ~World();

    const std::string& name() const { return _name; }

    // Entity management
    std::shared_ptr<Entity> createEntity( const std::string& name = "Entity" );
    void destroyEntity( EntityID id );
    void destroyEntity( std::shared_ptr<Entity> entity );

    std::shared_ptr<Entity> findEntity( EntityID id ) const;
    std::shared_ptr<Entity> findEntity( const std::string& name ) const;

    const std::vector<std::shared_ptr<Entity>>& entities() const { return _entities; }

    size_t entityCount() const { return _entities.size(); }

    // Update - call once per frame
    void update( double deltaTime );

    // Clear all entities
    void clear();

private:
    EntityID generateEntityID() { return _nextEntityID++; }

private:
    std::string _name;
    std::vector<std::shared_ptr<Entity>> _entities;
    std::unordered_map<EntityID, std::shared_ptr<Entity>> _entitiesById;
    std::unordered_map<std::string, std::shared_ptr<Entity>> _entitiesByName;

    EntityID _nextEntityID = 1;
};

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_WORLD_H
