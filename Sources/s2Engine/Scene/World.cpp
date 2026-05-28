// World.cpp
//
#include "World.h"

#include <algorithm>

using namespace s2::Scene;

// ------------------------------------------------------------------------------------------------
World::World( const std::string& name )
    : _name( name )
{
}

// ------------------------------------------------------------------------------------------------
World::~World()
{
    clear();
}

// ------------------------------------------------------------------------------------------------
std::shared_ptr<Entity> World::createEntity( const std::string& name )
{
    EntityID id = generateEntityID();
    auto entity = std::make_shared<Entity>( id, name );

    _entities.push_back( entity );
    _entitiesById[id] = entity;
    _entitiesByName[name] = entity;

    return entity;
}

// ------------------------------------------------------------------------------------------------
void World::destroyEntity( EntityID id )
{
    auto it = _entitiesById.find( id );
    if( it != _entitiesById.end() )
    {
        auto entity = it->second;
        destroyEntity( entity );
    }
}

// ------------------------------------------------------------------------------------------------
void World::destroyEntity( std::shared_ptr<Entity> entity )
{
    if( !entity )
     return;

    // Remove from maps
    _entitiesById.erase( entity->id() );
    _entitiesByName.erase( entity->name() );

    // Remove from vector
    auto it = std::find( _entities.begin(), _entities.end(), entity );
    if( it != _entities.end() )
        _entities.erase( it );
}

// ------------------------------------------------------------------------------------------------
std::shared_ptr<Entity> World::findEntity( EntityID id ) const
{
    auto it = _entitiesById.find( id );
    if( it != _entitiesById.end() )
        return it->second;
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
std::shared_ptr<Entity> World::findEntity( const std::string& name ) const
{
    auto it = _entitiesByName.find( name );
    if( it != _entitiesByName.end() )
        return it->second;
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
void World::update( double deltaTime )
{
    // Update all active entities
    for( auto& entity : _entities )
    {
        if( !entity->isActive() )
            continue;

        // TODO: Call update on systems/components
        // For now, this is a placeholder for future system execution
    }
}

// ------------------------------------------------------------------------------------------------
void World::clear()
{
    _entities.clear();
    _entitiesById.clear();
    _entitiesByName.clear();
    _nextEntityID = 1;
}