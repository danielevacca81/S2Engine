// RenderCommand.h
//
#ifndef S2_RENDERER_RENDERCOMMAND_H
#define S2_RENDERER_RENDERCOMMAND_H

#include "s2Engine_API.h"
#include "Math/Math.h"
#include "Resources/Material.h"
#include "RenderCore/VertexData.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/PrimitiveType.h"

#include <memory>
#include <functional>

namespace s2 {
namespace Renderer {

/**
 * Represents a single draw call with all necessary state.
 * Decouples high-level rendering logic from OpenGL specifics.
 * Uses RenderCore::DrawState for encapsulating render state and shader.
 */
struct S2ENGINE_API RenderCommand
{
	// Material & Resources
	Resources::MaterialPtr material = nullptr;

	// Geometry
	MeshData3D	   meshData;    // Optional mesh data (for dynamic meshes)
	
	// Primitive type for drawing
	RenderCore::PrimitiveType primitiveType = RenderCore::PrimitiveType::Triangles;

	// Draw state (encapsulates shader, render state, view state, textures)
	RenderCore::DrawState drawState;

	// Sorting and filtering
	uint32_t layerMask = 0;
	float    sortKey   = 0.0f;  // For sorting (depth, material, etc.)

	// Shadow flags
	bool castsShadows    = true;
	bool receiveShadows  = true;

	// Custom uniform setter (optional)
	// Allows entities to set custom uniforms before drawing
	// Takes DrawState instead of Program directly
	std::function<void(RenderCore::DrawState&)> customUniformSetter = nullptr;

	// User data for custom rendering logic
	void* userData = nullptr;
};

}
}

#endif // S2_RENDERER_RENDERCOMMAND_H