// RenderMaterial.h
//
#ifndef S2_RENDERER_RENDERMATERIAL_H
#define S2_RENDERER_RENDERMATERIAL_H

#include "s2Engine_API.h"

#include "Graphics/Color.h"
#include "RenderCore/Program.h"
#include "RenderCore/Texture.h"

#include <unordered_map>
#include <string>
#include <variant>

namespace s2 {
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


	RenderCore::ProgramPtr shader;
	
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
	std::unordered_map<std::string, Property> properties;

	std::unordered_map<uint32_t, RenderCore::Texture2DPtr> textures;

	bool operator<( const RenderMaterial& other ) const
	{
		// if this material is opaque and the other is transparent
		// this material should be rendered before the other
		return blendMode == BlendMode::Opaque
			&& other.blendMode == BlendMode::AlphaBlend;
	}
};

} // namespace Renderer
} // namespace s2



#endif // !S2_RENDERER_RENDERMATERIAL_H