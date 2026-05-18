// LightManager.cpp
//
#include "LightManager.h"
#include "World.h"
#include "Entity.h"


using namespace s2::Scene;


// ------------------------------------------------------------------------------------------------
LightManager::LightManager( World* world )
    : _world( world )
{
}

// ------------------------------------------------------------------------------------------------
void LightManager::updateFromWorld()
{
    clear();

    if( !_world )
        return;

    collectLights();
}

// ------------------------------------------------------------------------------------------------
void LightManager::collectLights()
{
    // Iterate through all entities in the world
    for( const auto& entity : _world->entities() )
    {
        if( !entity->isActive() )
            continue;

        // Check for DirectionalLight
        auto dirLight = entity->component<DirectionalLight>();
        if( dirLight && !_mainDirectionalLight )
            _mainDirectionalLight = dirLight.get();

        // Check for PointLight
        auto pointLight = entity->component<PointLight>();
        if( pointLight && _pointLights.size() < MaxPointLights )
            _pointLights.push_back( pointLight.get() );
    }
}

// ------------------------------------------------------------------------------------------------
void LightManager::clear()
{
    _mainDirectionalLight = nullptr;
    _pointLights.clear();
}