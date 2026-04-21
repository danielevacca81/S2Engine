#ifndef S2_RENDERER_RENDERMATERIAL_H
#define S2_RENDERER_RENDERMATERIAL_H

#include "s2Engine_API.h"

#include "ResourceManager.h"
#include "Graphics/Color.h"
#include "Math/Math.h"

#include <unordered_map>
#include <string>
#include <variant>
#include <memory>
#include <string_view>

namespace s2::Renderer {

enum class BlendMode { Opaque, AlphaBlend, Additive, Multiply };
enum class CullMode { Back, Front, None };

struct RenderState
{
    BlendMode blendMode { BlendMode::Opaque };
    CullMode  cullMode { CullMode::Back };
    bool      depthWrite { true };
    bool      depthTest { true };
    bool      shadowCaster { false };
};

class Material;

struct S2ENGINE_API MaterialDefinition
{
public:
    // factory method to create a material instance from this definition
    Material createMaterial();

public:
    using PropertyID = std::string;
    using Property = std::variant<
        bool, int, float, double, uint64_t,
        Color,// ResourceID,
        Math::fvec2, Math::fvec3, Math::fvec4,
        Math::dvec2, Math::dvec3, Math::dvec4,
        Math::fmat2, Math::fmat3, Math::fmat4,
        Math::dmat2, Math::dmat3, Math::dmat4
    >;

    RenderState state;
    ResourceID  shader { ResourceInvalidID };

    std::unordered_map<PropertyID, Property> defaultProperties;
};

#pragma region HelperFunctions
// ------------------------------------------------------------------------------------------------
// FNV-1a (32-bit) hash of the property name (limited collision risk, fast computation)
// ------------------------------------------------------------------------------------------------
//static constexpr inline MaterialDefinition::PropertyID hash( std::string_view name )
//{
//    constexpr uint32_t fnvOffsetBasis = 2166136261u;
//    constexpr uint32_t fnvPrime = 16777619u;
//    uint32_t hash = fnvOffsetBasis;
//    for( int8_t c : name )
//    {
//        hash ^= static_cast<uint32_t>( c );
//        hash *= fnvPrime;
//    }
//    return hash;
//}
// ------------------------------------------------------------------------------------------------
#pragma endregion


class S2ENGINE_API Material
{
public:
	Material() = default;
    Material( MaterialDefinition *def )
        : _definition( def )
    {}

    // parameter override
  //  template<typename T>
  //  Material& set( std::string_view name, T&& value )
  //  {
  //      //_overrides[hash( name )] = std::forward<T>( value );
        //_overrides[name] = std::forward<T>( value );
  //      return *this;
  //  }

    template<typename T>
    Material& set( const std::string& name, T&& value )
    {
        //_overrides[hash( name )] = std::forward<T>( value );
        _overrides[name] = std::forward<T>( value );
        return *this;
    }

    MaterialDefinition::Property property( const MaterialDefinition::PropertyID& id ) const;

    const MaterialDefinition& definition() const { return *_definition; }


    void apply( const ResourceManager& rm ) const;

private:
	MaterialDefinition* _definition { nullptr }; // not owned, reference to the original definition
    std::unordered_map<MaterialDefinition::PropertyID, MaterialDefinition::Property>  _overrides;
};

} // namespace s2::Renderer

#endif