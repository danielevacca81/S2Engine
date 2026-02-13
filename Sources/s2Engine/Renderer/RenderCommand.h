// RenderCommand.h
//
#ifndef S2_RENDERER_RENDERCOMMAND_H
#define S2_RENDERER_RENDERCOMMAND_H

#include "s2Engine_API.h"

#include "Resources/Material.h"
#include "Geometry/MeshData.h"
#include "Graphics/Color.h"

#include <memory>
#include <functional>

namespace s2 {
namespace Renderer {

/**
* Represents a clear command with specified clear color and buffers to clear.
* Will be processed by the renderer and translated into ClearState for the render pass.
*/
struct S2ENGINE_API ClearCommand
{
	enum class ClearMode
	{
		ColorOnly,
		DepthOnly,
		ColorAndDepth,
		StencilOnly,
		DepthAndStencil,
		AllBuffers
	};
	
	ClearMode mode  = ClearMode::ColorAndDepth;
	Color     color = Color::blue();
	float     depth = 1.0f;
	uint32_t  stencil = 0;
};


/**
 * Represents a single draw call with all necessary state.
 * Will be processed by the renderer and translated into DrawState for the render pass.
 */
struct S2ENGINE_API RenderCommand
{
	enum class DrawMode
	{
		Points,
		Lines,
		Triangles,
	};
	DrawMode drawMode = DrawMode::Triangles;

	// Material provides shader + textures + pipeline state
	Resources::MaterialPtr material = nullptr;

	Resources::ModelPtr model = nullptr;
	
	// per object transform (model matrix)
	Math::dmat4 modelMatrix = Math::dmat4( 1.0 );


	// Geometry
	MeshData3D	   meshData;

	// User data for custom rendering logic
	void* userData = nullptr;
};

}
}

#endif // !S2_RENDERER_RENDERCOMMAND_H