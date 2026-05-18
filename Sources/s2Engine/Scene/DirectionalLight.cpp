// DirectionalLight.cpp
//
#include "DirectionalLight.h"
#include "Entity.h"
#include "Transform.h"

using namespace s2::Scene;

// ------------------------------------------------------------------------------------------------
DirectionalLight::DirectionalLight()
    : _shadowMapID( 0 )
{
    // Defaults for directional light
    _config.castShadow = true;
    _config.shadowMapResolution = 4096;
    _config.shadowBias = 0.005f;
    _config.shadowNormalBias = 0.01f;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 DirectionalLight::direction() const
{
    if( !_owner )
        return Math::dvec3( 0, -1, 0 );  // Default down

    // Get forward direction from owning entity's transform
    return _owner->transform().forward();
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 DirectionalLight::lightSpaceMatrix() const
{
    if( _lightSpaceMatrixDirty )
        updateLightSpaceMatrix();
    return _cachedLightSpaceMatrix;
}

// ------------------------------------------------------------------------------------------------
void DirectionalLight::updateLightSpaceMatrix() const
{
    if( !_owner )
    {
        _cachedLightSpaceMatrix = Math::dmat4( 1.0 );
        _lightSpaceMatrixDirty = false;
        return;
    }

    // For directional light, create orthographic projection in light-space
    // Direction is from transform.forward()
    const auto lightDir = direction();
    const auto lightPos = _owner->transform().worldPosition();

    // View matrix: look from light position in light direction
    const auto view = Math::lookAt(
        lightPos,                    // eye
        lightPos + lightDir,         // center (look in direction)
        Math::dvec3( 0, 1, 0 )      // up
    );

    // Orthographic projection (tunable for frustum size)
    // Typical for shadow maps: [-50, 50] for x/y, [0.1, 100] for z
    const auto projection = Math::ortho(
        -50.0, 50.0,    // left, right
        -50.0, 50.0,    // bottom, top
        0.1, 100.0      // near, far
    );

    _cachedLightSpaceMatrix = projection * view;
    _lightSpaceMatrixDirty = false;
}