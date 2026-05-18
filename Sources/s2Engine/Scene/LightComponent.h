// LightComponent.h
//
#ifndef S2_SCENE_LIGHTCOMPONENT_H
#define S2_SCENE_LIGHTCOMPONENT_H

#include "s2Engine_API.h"
#include "Component.h"
#include "LightConfig.h"
#include "Graphics/Color.h"
#include "Math/Math.h"

#include <memory>

namespace s2 {
namespace Scene {

// Base class for all light types
class S2ENGINE_API LightComponent : public Component
{
public:
    virtual ~LightComponent() = default;

    // Light appearance
    void setColor( const Color& col ) { _color = col; }
    const Color& color() const { return _color; }

    void setIntensity( float i ) { _intensity = i; }
    float intensity() const { return _intensity; }

    // Configuration (shadow bias, attenuation, normal mapping, etc.)
    LightConfig& config() { return _config; }
    const LightConfig& config() const { return _config; }

    // Light properties
    virtual Math::dvec3 direction() const = 0;  // World-space direction
    virtual float range() const = 0;             // Influence range (inf for directional)

protected:
    Color _color { 1.0f, 1.0f, 1.0f };
    float _intensity = 1.0f;
    LightConfig _config;
};

using LightComponentPtr = std::shared_ptr<LightComponent>;

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_LIGHTCOMPONENT_H
