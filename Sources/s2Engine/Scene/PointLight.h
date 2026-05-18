// PointLight.h
//
#ifndef S2_SCENE_POINTLIGHT_H
#define S2_SCENE_POINTLIGHT_H

#include "s2Engine_API.h"
#include "LightComponent.h"
#include "Math/Math.h"

namespace s2 {
namespace Scene {

// Point light component (lamp, torch, etc.)
// Position from entity transform
// Range defines influence radius
// Supports quadratic attenuation
class S2ENGINE_API PointLight : public LightComponent
{
public:
    explicit PointLight( float range = 10.0f );

    // Position from entity transform
    Math::dvec3 direction() const override;  // Returns normalized direction from light to scene center
    float range() const override { return _range; }

    void setRange( float r ) { _range = r; }

    // Attenuation configuration
    void setAttenuationExponent( float exp ) { _config.attenuationExponent = exp; }
    float attenuationExponent() const { return _config.attenuationExponent; }

    void setUseInverseSquareLaw( bool use ) { _config.useInverseSquareLaw = use; }
    bool useInverseSquareLaw() const { return _config.useInverseSquareLaw; }

    // Compute attenuation at distance d
    // Formula: 1.0 / (1.0 + distance^exponent) or 1.0 / (distance^2) if inverse square law
    float computeAttenuation( float distance ) const;

private:
    float _range = 10.0f;  // Influence radius
};

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_POINTLIGHT_H
