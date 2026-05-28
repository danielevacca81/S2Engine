// Entity.h
//
#ifndef S2_SCENE_ENTITY_H
#define S2_SCENE_ENTITY_H

#include "s2Engine_API.h"
#include "Component.h"
#include "Transform.h"

#include <vector>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <algorithm>
#include <string>

namespace s2 {
namespace Scene {

// Unique entity identifier
using EntityID = uint32_t;
static constexpr EntityID InvalidEntityID = 0;

class S2ENGINE_API Entity
{
public:
    explicit Entity( EntityID id, const std::string& name = "Entity" );
    ~Entity();

    EntityID           id() const { return _id; }
    const std::string& name() const { return _name; }
    void               setName( const std::string& name ) { _name = name; }

    bool isActive() const { return _active; }
    void setActive( bool active ) { _active = active; }

    // Component management
    template<typename T, typename... Args>
    std::shared_ptr<T> addComponent( Args&&... args )
    {
        static_assert( std::is_base_of_v<Component, T>, "T must derive from Component" );

        // Check if component already exists
        auto existing = component<T>();
        if( existing ) return existing;

        auto component = std::make_shared<T>( std::forward<Args>( args )... );
        component->_owner = this;
        component->onAttach();

        _components.push_back( component );
        _componentsByType[typeid( T ).hash_code()] = component;

        return component;
    }

    template<typename T>
    std::shared_ptr<T> component() const
    {
        auto it = _componentsByType.find( typeid( T ).hash_code() );
        if( it != _componentsByType.end() )
            return std::static_pointer_cast<T>( it->second );
        return nullptr;
    }

    template<typename T>
    bool hasComponent() const
    {
        return _componentsByType.find( typeid( T ).hash_code() ) != _componentsByType.end();
    }

    template<typename T>
    void removeComponent()
    {
        auto it = _componentsByType.find( typeid( T ).hash_code() );
        if( it != _componentsByType.end() )
        {
            auto component = it->second;
            component->onDetach();
            
            auto vecIt = std::find( _components.begin(), _components.end(), component );
            if( vecIt != _components.end() )
                _components.erase( vecIt );

            _componentsByType.erase( it );
        }
    }

    // Convenience: Get transform component (always exists)
    [[nodiscard]] Transform& transform() { return *_transform; }
    [[nodiscard]] const Transform& transform() const { return *_transform; }

    // Get all components
    const std::vector<ComponentPtr>& components() const { return _components; }

private:
    EntityID _id;
    std::string _name;
    bool _active {true};

    std::shared_ptr<Transform> _transform;
    std::vector<ComponentPtr> _components;
    std::unordered_map<size_t, ComponentPtr> _componentsByType;  // typeid hash -> component
};

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_ENTITY_H
