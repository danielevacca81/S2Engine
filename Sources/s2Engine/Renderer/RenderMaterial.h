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
    // ===== Render State Properties =====
    
    BlendMode blendMode    { BlendMode::Opaque };
    CullMode  cullMode     { CullMode::Back };
    bool      depthWrite   { true };
    bool      depthTest    { true };
    bool      shadowCaster { false };

    // Shader handle
    ResourceHandle shader { InvalidHandle };

    // ===== Material Properties (Uniforms) =====
    
    using Property = std::variant<
        bool,
        int,
        float,
        double,
        uint64_t,
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

    std::unordered_map<std::string, Property> properties;     // uniform name -> value
    std::unordered_map<std::string, ResourceHandle> textures; // uniform name -> texture handle

    // ===== Sorting for Render Order =====
    
    bool operator<( const RenderMaterial& other ) const
    {
        // Opaque materials rendered before transparent (front-to-back)
        if( blendMode != other.blendMode )
            return blendMode == BlendMode::Opaque && other.blendMode != BlendMode::Opaque;
        
        // Then sort by shader (minimize shader switches)
        if( shader != other.shader )
            return shader < other.shader;
        
        // Then by material properties (arbitrary but consistent)
        return properties.size() < other.properties.size();
    }

    // ===== Setters (Convenience API) =====
    
    void setProperty( const std::string& name, const Property& value )
    {
        properties[name] = value;
    }

    void setBool( const std::string& name, bool value )
    {
        properties[name] = value;
    }

    void setInt( const std::string& name, int value )
    {
        properties[name] = value;
    }

    void setFloat( const std::string& name, float value )
    {
        properties[name] = value;
    }

    void setColor( const std::string& name, const Color& value )
    {
        properties[name] = value;
    }

    void setVec2( const std::string& name, const Math::fvec2& value )
    {
        properties[name] = value;
    }

    void setVec3( const std::string& name, const Math::fvec3& value )
    {
        properties[name] = value;
    }

    void setVec4( const std::string& name, const Math::fvec4& value )
    {
        properties[name] = value;
    }

    void setMatrix3( const std::string& name, const Math::fmat3& value )
    {
        properties[name] = value;
    }

    void setMatrix4( const std::string& name, const Math::fmat4& value )
    {
        properties[name] = value;
    }

    // Set texture by uniform name (not unit index!)
    void setTexture( const std::string& uniformName, ResourceHandle textureHandle )
    {
        textures[uniformName] = textureHandle;
    }

    // ===== Apply to Shader (DSA) =====
    
    // Apply all material properties to shader uniforms (DSA - no binding required)
    void applyPropertiesToShader( RenderCore::Shader& shader ) const;
    
    // Apply textures to shader (Bindless)
    void applyTexturesToShader( RenderCore::Shader& shader, const ResourceManager& resourceManager ) const;
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_RENDERMATERIAL_H