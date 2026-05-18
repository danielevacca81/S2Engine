// Entity.cpp
//
#include "Entity.h"

using namespace s2::Scene;

// ------------------------------------------------------------------------------------------------
Entity::Entity( EntityID id, const std::string& name )
    : _id( id )
    , _name( name )
{
    // Every entity has a Transform by default
    _transform = std::make_shared<Transform>();
    _transform->_owner = this;
    _transform->onAttach();
    _components.push_back( _transform );
    _componentsByType[typeid( Transform ).hash_code()] = _transform;
}

// ------------------------------------------------------------------------------------------------
Entity::~Entity()
{
    for( auto& component : _components )
        component->onDetach();
    // _components.clear();
    // _componentsByType.clear();
}
