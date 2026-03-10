// RenderMaterial.h
//
#ifndef S2_RENDERER_RENDERMATERIAL_H
#define S2_RENDERER_RENDERMATERIAL_H

#include "s2Engine_API.h"

#include "ResourceManager.h"

#include "Graphics/Color.h"
#include "Math/Math.h"

#include <unordered_map>
#include <string>
#include <variant>

namespace s2 {
namespace RenderCore { class Shader; }
namespace Renderer {

// ------------------------------------------------------------------------------------------------
enum class BlendMode
{
    Opaque,
    AlphaBlend,
    Additive,
    Multiply
};

// ------------------------------------------------------------------------------------------------
enum class CullMode
{
    Back,
    Front,
    None
};

// ------------------------------------------------------------------------------------------------
struct S2ENGINE_API RenderMaterial
{
public:
    // ===== Material Properties (Uniforms) =====    
    using Property = std::variant<
        bool,
        int,
        float,
        double,
        uint64_t,   // For bindless texture handles
        Color,
        Math::fvec2,
        Math::fvec3,
        Math::fvec4,
        Math::fmat2,
        Math::fmat3,
        Math::fmat4,
        Math::dvec2,
        Math::dvec3,
        Math::dvec4,
        Math::dmat2,
        Math::dmat3,
        Math::dmat4
    >;

public:
    // ===== Render State Properties =====    
    BlendMode blendMode    { BlendMode::Opaque };
    CullMode  cullMode     { CullMode::Back };
    bool      depthWrite   { true };
    bool      depthTest    { true };
    bool      shadowCaster { false };

    // Shader handle
    ResourceHandle shader { InvalidHandle };

    void set        ( const std::string& name, const Property& value );
    
    // ===== Setters (Convenience API) =====
    void setBool    ( const std::string& name, bool value );
    void setInt     ( const std::string& name, int value );
    void setFloat   ( const std::string& name, float value );
    void setColor   ( const std::string& name, const Color& value );
    void setVec2    ( const std::string& name, const Math::fvec2& value );
    void setVec3    ( const std::string& name, const Math::fvec3& value );
    void setVec4    ( const std::string& name, const Math::fvec4& value );
    void setMatrix3 ( const std::string& name, const Math::fmat3& value );
    void setMatrix4 ( const std::string& name, const Math::fmat4& value );

    // Set texture by uniform name (not unit index!)
    void setTexture( const std::string& uniformName, ResourceHandle textureHandle );

	template<typename T>
    std::optional<T> property( const std::string& name ) const
    {
        auto it = _properties.find( name );
        if( it == _properties.end() )
			return std::nullopt; // Property not found

        if( auto value = std::get_if<T>( &it->second ) )
			return *value; // Successfully retrieved and converted
        return std::nullopt; // Property not found or type mismatch
	}

    // ===== Sorting for Render Order =====
    bool operator<( const RenderMaterial& other ) const;

private:
    // ===== Apply to Shader (DSA) =====
    void applyPropertiesToShader( RenderCore::Shader& shader ) const;

    // Apply textures to shader (Bindless)
    void applyTexturesToShader( RenderCore::Shader& shader, const ResourceManager& resourceManager ) const;


private:
    std::unordered_map<std::string, Property>       _properties;
    std::unordered_map<std::string, ResourceHandle> _textures;

	friend class ForwardPass; // Allow access to private members for rendering
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_RENDERMATERIAL_H