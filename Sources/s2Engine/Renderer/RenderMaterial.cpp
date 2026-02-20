// RenderMaterial.cpp
//
#include "RenderMaterial.h"

#include "RenderCore/Shader.h"
#include "RenderCore/Uniform.h"

#include <optional>

using namespace s2::Renderer;

// --------------------------------------------------------------------------------------------
// Helper function to convert RenderMaterial::Property to RenderCore::UniformValue based on target uniform type
static inline std::optional<RenderCore::UniformValue> convert( const RenderMaterial::Property& property, const RenderCore::UniformValue& targetUniformType )
{
    return std::visit( [&] ( auto&& targetType ) -> std::optional<RenderCore::UniformValue>
    {
        using TargetType = std::decay_t<decltype( targetType )>;

        return std::visit( [&]<typename T>( const T & value ) -> std::optional<RenderCore::UniformValue>
        {
            using ValueType = std::decay_t<T>;

			// 1. no conversion needed (types match)
            if constexpr( std::is_same_v<ValueType, TargetType> )   return value;
            // 2. numeric conversion (int->float, float->double, etc.)
            else if constexpr( std::is_arithmetic_v<ValueType> && std::is_arithmetic_v<TargetType> ) return static_cast<TargetType>( value );
            // 3. Color -> vec4
            else if constexpr( std::is_same_v<ValueType, Color> && std::is_same_v<TargetType, Math::fvec4> ) return Math::fvec4( value.r(), value.g(), value.b(), value.a() );
            else if constexpr( std::is_same_v<ValueType, Color> && std::is_same_v<TargetType, Math::fvec3> ) return Math::fvec3( value.r(), value.g(), value.b() );
            // 4. vec3 -> vec4 (add w=1.0)
            else if constexpr( std::is_same_v<ValueType, Math::fvec3> && std::is_same_v<TargetType, Math::fvec4> ) return Math::fvec4( value.x, value.y, value.z, 1.0f );
            // 5. vec4 -> vec3 (ignore w)
            else if constexpr( std::is_same_v<ValueType, Math::fvec4> && std::is_same_v<TargetType, Math::fvec3> ) return Math::fvec3( value.x, value.y, value.z );
            // 6. vec2 -> vec3 (add z=0.0)
            else if constexpr( std::is_same_v<ValueType, Math::fvec2> && std::is_same_v<TargetType, Math::fvec3> ) return Math::fvec3( value.x, value.y, 0.0f );
            // 7. vec2 -> vec4 (add z=0.0, w=1.0)
            else if constexpr( std::is_same_v<ValueType, Math::fvec2> && std::is_same_v<TargetType, Math::fvec4> ) return Math::fvec4( value.x, value.y, 0.0f, 1.0f );
            // 8. Double -> Float vector conversions
            else if constexpr( std::is_same_v<ValueType, Math::dvec2> && std::is_same_v<TargetType, Math::fvec2> ) return Math::fvec2( static_cast<float>( value.x ), static_cast<float>( value.y ) );
            else if constexpr( std::is_same_v<ValueType, Math::dvec3> && std::is_same_v<TargetType, Math::fvec3> ) return Math::fvec3( static_cast<float>( value.x ), static_cast<float>( value.y ), static_cast<float>( value.z ) );
            else if constexpr( std::is_same_v<ValueType, Math::dvec4> && std::is_same_v<TargetType, Math::fvec4> ) return Math::fvec4( static_cast<float>( value.x ), static_cast<float>( value.y ), static_cast<float>( value.z ), static_cast<float>( value.w ) );
            // 9. Float -> Double vector conversions
            else if constexpr( std::is_same_v<ValueType, Math::fvec2> && std::is_same_v<TargetType, Math::dvec2> ) return Math::dvec2( static_cast<double>( value.x ), static_cast<double>( value.y ) );
            else if constexpr( std::is_same_v<ValueType, Math::fvec3> && std::is_same_v<TargetType, Math::dvec3> ) return Math::dvec3( static_cast<double>( value.x ), static_cast<double>( value.y ), static_cast<double>( value.z ) );
            else if constexpr( std::is_same_v<ValueType, Math::fvec4> && std::is_same_v<TargetType, Math::dvec4> ) return Math::dvec4( static_cast<double>( value.x ), static_cast<double>( value.y ), static_cast<double>( value.z ), static_cast<double>( value.w ) );
            // 10. Matrix conversions (double <-> float)
            else if constexpr( std::is_same_v<ValueType, Math::dmat2> && std::is_same_v<TargetType, Math::fmat2> ) return Math::fmat2( value );
            else if constexpr( std::is_same_v<ValueType, Math::dmat3> && std::is_same_v<TargetType, Math::fmat3> ) return Math::fmat3( value );
            else if constexpr( std::is_same_v<ValueType, Math::dmat4> && std::is_same_v<TargetType, Math::fmat4> ) return Math::fmat4( value );
            else if constexpr( std::is_same_v<ValueType, Math::fmat2> && std::is_same_v<TargetType, Math::dmat2> ) return Math::dmat2( value );
            else if constexpr( std::is_same_v<ValueType, Math::fmat3> && std::is_same_v<TargetType, Math::dmat3> ) return Math::dmat3( value );
            else if constexpr( std::is_same_v<ValueType, Math::fmat4> && std::is_same_v<TargetType, Math::dmat4> ) return Math::dmat4( value );
            else
            {
				// not supported conversion
                return std::nullopt;
            }
        }, property );
    }, targetUniformType );
}

// --------------------------------------------------------------------------------------------
void RenderMaterial::applyPropertiesToShader( RenderCore::Shader &shader ) const
{
    for( const auto& [name, property] : properties )
    {
        auto* uniform = shader.uniform( name );
        if( !uniform )
			continue; // this uniform was not found in shader, skip

        auto convertedValue = convert( property, uniform->value() );
        if( convertedValue )
            uniform->setValue( *convertedValue );
        else
        {
            //assert( false && "RenderMaterialPropertyApplier: Cannot convert material property to shader uniform type." );
        }
    }
}