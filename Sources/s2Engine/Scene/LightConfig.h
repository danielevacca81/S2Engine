// LightConfig.h
//
#ifndef S2_SCENE_LIGHTCONFIG_H
#define S2_SCENE_LIGHTCONFIG_H

namespace s2 {
namespace Scene {

// Configuration structure for all lighting parameters
// Allows flexible, future-proof expansion (shadow bias, normal mapping, attenuation, etc.)
struct LightConfig
{
    // Shadow mapping parameters
    float shadowBias = 0.005f;
    float shadowNormalBias = 0.01f;
    int shadowMapResolution = 4096;
    bool castShadow = true;

    // Attenuation parameters (point lights)
    float attenuationExponent = 2.0f;      // quadratic falloff: 1/r^exp
    bool useInverseSquareLaw = true;

    // Normal mapping
    bool enableNormalMapping = true;
    float normalMapStrength = 1.0f;

    // Future expansions (reserved slots)
    float reserved1 = 0.0f;
    float reserved2 = 0.0f;
};

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_LIGHTCONFIG_H
