// LightManager.h
//
#ifndef S2_SCENE_LIGHTMANAGER_H
#define S2_SCENE_LIGHTMANAGER_H

#include "s2Engine_API.h"
#include "LightComponent.h"
#include "DirectionalLight.h"
#include "PointLight.h"

#include <vector>
#include <memory>

namespace s2 {
namespace Scene {

class World;

// Manages active lights in the scene
// Collects and exposes lights to the renderer
// Can be extended with light culling, LOD, etc.
class S2ENGINE_API LightManager
{
public:
    static constexpr size_t MaxPointLights = 4;  // Max point lights supported by shader

public:
    explicit LightManager( World* world = nullptr );

    // Update from world entities
    void updateFromWorld();

    // Get active lights
    DirectionalLight* mainDirectionalLight() const { return _mainDirectionalLight; }
    const std::vector<PointLight*>& pointLights() const { return _pointLights; }

    // Clear all light references
    void clear();

private:
    void collectLights();


private:
    World* _world;
    DirectionalLight* _mainDirectionalLight = nullptr;
    std::vector<PointLight*> _pointLights;
};

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_LIGHTMANAGER_H
