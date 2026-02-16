// RenderCommand.h
//
#ifndef S2_RENDERER_RENDERCOMMAND_H
#define S2_RENDERER_RENDERCOMMAND_H

#include "s2Engine_API.h"

#include "Geometry/MeshData.h"

#include "RenderMaterial.h"

#include <memory>
#include <functional>

namespace s2 {
namespace Renderer {

enum class RenderMode
{
	Points,
	Lines,
	Triangles,
};

enum class ClearMode
{
	ColorOnly,
	DepthOnly,
	ColorAndDepth,
	StencilOnly,
	DepthAndStencil,
	AllBuffers
};


/**
* Represents a clear command with specified clear color and buffers to clear.
* Will be processed by the renderer and translated into ClearState for the render pass.
*/
struct S2ENGINE_API ClearCommand
{
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
	RenderMode     renderMode { RenderMode::Triangles };
	RenderMaterial material;
	RenderCore::ShaderPtr shader; // Optional: specify a shader for this command, otherwise use default from material
	//RenderObject   object;   // @todo: add geometry and material for this drawcall

	// per object transform (model matrix)
	Math::dmat4 modelMatrix = Math::dmat4( 1.0 );

	// User data for custom rendering logic
	void* userData = nullptr;
};

}
}

#endif // !S2_RENDERER_RENDERCOMMAND_H