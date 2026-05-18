// PointLight.cpp
//
#include "PointLight.h"
#include "Entity.h"
#include "Transform.h"

#include <cmath>
#include <algorithm>

using namespace s2::Scene;
 
// ------------------------------------------------------------------------------------------------
PointLight::PointLight( float range )
    : _range( range )
{
    // Defaults for point light
    _config.castShadow = false;  // Can be enabled for cube map shadows later
    _config.attenuationExponent = 2.0f;  // Quadratic
    _config.useInverseSquareLaw = true;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 PointLight::direction() const
{
    if( !_owner )
        return Math::dvec3( 0, -1, 0 );

    // For point lights, return position (not direction)
    // This is used for shading calculations
    return Math::normalize( _owner->transform().worldPosition() );
}

// ------------------------------------------------------------------------------------------------
float PointLight::computeAttenuation( float distance ) const
{
    // Clamp distance to range
    distance = std::max( 0.0f, std::min( distance, _range ) );

    if( _config.useInverseSquareLaw )
    {
        // Physically-based: 1 / r^2
        if( distance < 0.001f )
            return 1.0f;
        float att = 1.0f / (distance * distance);
        return std::min( 1.0f, att );  // Clamp to [0, 1]
    }
    else
    {
        // Custom exponent: 1 / (1 + distance^exp)
        float exp = _config.attenuationExponent;
        float dist_exp = std::pow( distance, exp );
        return 1.0f / (1.0f + dist_exp);
    }
}