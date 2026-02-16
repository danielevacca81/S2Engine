// RenderMaterial.h
//
#ifndef S2_RENDERER_RENDERMATERIAL_H
#define S2_RENDERER_RENDERMATERIAL_H

#include "s2Engine_API.h"

#include "Graphics/Color.h"

#include <unordered_map>
#include <string>

namespace s2 {
namespace Renderer {

enum class BlendMode
{
    Opaque,
    AlphaBlend,
    Additive,
    Multiply,
};

enum class CullMode
{
    Back,
    Front,
    None,
};

struct S2ENGINE_API DepthWrite
{
    bool enabled = true;
};

struct S2ENGINE_API DepthTest
{
    bool enabled = true;
};

struct S2ENGINE_API ShadowCaster
{
	bool enabled = false;	
};

struct S2ENGINE_API RenderMaterial
{
	// Color properties
	Color baseColor { Color::blue() };
	Color emissiveColor { Color::black() };

	// Material properties
	float roughness = 0.5f;
	float metallic = 0.0f;
	float opacity = 1.0f;

	// Render state properties
	BlendMode  blendMode  { BlendMode::Opaque };
	CullMode   cullMode   { CullMode::Back };
	DepthWrite depthWrite { true };
	DepthTest  depthTest  { true };
	ShadowCaster shadowCaster { false };

	// Texture slots 
    //    key: texture slot (e.g., albedo, normal, roughness)
    //    value: texture resource identifier (e.g., texture ID or name)
	//std::unordered_map<uint32_t, std::string> textures;

	// shader program
};

} // namespace Renderer
} // namespace s2



#endif // !S2_RENDERER_RENDERMATERIAL_H