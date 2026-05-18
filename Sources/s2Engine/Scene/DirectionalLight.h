// DirectionalLight.h
//
#ifndef S2_SCENE_DIRECTIONALLIGHT_H
#define S2_SCENE_DIRECTIONALLIGHT_H

#include "s2Engine_API.h"
#include "LightComponent.h"
#include "Math/Math.h"

namespace s2 {
namespace Scene {

// Directional light component (sun, moon, etc.)
// Direction is derived from transform.forward()
// Supports shadow mapping for real-time shadows
class S2ENGINE_API DirectionalLight : public LightComponent
{
public:
    DirectionalLight();

    // Direction from entity transform (forward vector)
    Math::dvec3 direction() const override;
    float range() const override { return std::numeric_limits<float>::infinity(); }

    // Shadow map binding
    void setShadowMapID( uint64_t id ) { _shadowMapID = id; }
    uint64_t shadowMapID() const { return _shadowMapID; }

    // Light-space matrix for shadow rendering
    Math::dmat4 lightSpaceMatrix() const;

    // Configure shadow map resolution
    void setShadowMapResolution( int resolution )
    {
        _config.shadowMapResolution = resolution;
    }

    int shadowMapResolution() const { return _config.shadowMapResolution; }

private:
    void updateLightSpaceMatrix() const;

private:
    uint64_t _shadowMapID = 0;  // ResourceID for shadow depth texture

    // Cached light-space matrix (computed from position/direction)
    mutable Math::dmat4 _cachedLightSpaceMatrix;
    mutable bool _lightSpaceMatrixDirty = true;

};

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_DIRECTIONALLIGHT_H
