// RenderMaterial.h
//
#ifndef S2_RENDERER_RENDERMATERIAL_H
#define S2_RENDERER_RENDERMATERIAL_H

#include "s2Engine_API.h"

#include "ResourceManager.h"

#include "Graphics/Color.h"

#include <unordered_map>
#include <string>
#include <variant>

namespace s2 {
namespace RenderCore { class Shader;}
namespace Renderer {

enum class BlendMode
{
    Opaque,
    AlphaBlend,
};

enum class CullMode
{
    Back,
    Front,
    None,
};


struct S2ENGINE_API RenderMaterial
{
	// Render state properties
	BlendMode blendMode    { BlendMode::Opaque };
	CullMode  cullMode     { CullMode::Back };
	bool      depthWrite   { true };
	bool      depthTest    { true };
	bool      shadowCaster { false };
	// colorMask { true, true, true, true }


	ResourceHandle shader { InvalidHandle };
	
	// instead of public properties variant, use:
	// setFloat("roughness", 0.5f);
	// setColor("baseColor", Color::red());
	// setVec3("emissiveColor", Math::vec3(1.f, 0.f, 0.f));
	// setVec4("lightPosition", Math::vec4(1.f, 0.f, 0.f));
	// setTexture("albedoMap", texturePtr);
	// 
	// ... etc.
	// 
	// with corresponding getters.

	//void setBool   ( const std::string& name, bool value )               { properties[name] = value; }
	//void setFloat  ( const std::string& name, float value )              { properties[name] = value; }
	//void setInt    ( const std::string& name, int value )                { properties[name] = value; }
	//void setColor  ( const std::string& name, const Color& value )       { properties[name] = value; }
	//void setVec2   ( const std::string& name, const Math::fvec2& value ) { properties[name] = value; }
	//void setVec3   ( const std::string& name, const Math::fvec3& value ) { properties[name] = value; }
	//void setVec4   ( const std::string& name, const Math::fvec4& value ) { properties[name] = value; }
	//void setTexture( const std::string& name, TextureHandle texture )    { textures[name]   = texture; }

	using Property = std::variant<
		bool,
		int,
		float,
		double,
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
		Math::dmat4>;
	std::unordered_map<std::string, Property> properties; // uniform name -> value

	std::unordered_map<uint16_t, ResourceHandle> textures; // texture unit -> texture handle

	bool operator<( const RenderMaterial& other ) const
	{
		// if this material is opaque and the other is transparent
		// this material should be rendered before the other
		return blendMode == BlendMode::Opaque
			&& other.blendMode == BlendMode::AlphaBlend;
	}

	// Applies the material properties to the shader uniforms based on matching names and compatible types
	void applyPropertiesToShader( RenderCore::Shader& shader ) const;
};

} // namespace Renderer
} // namespace s2



#endif // !S2_RENDERER_RENDERMATERIAL_H