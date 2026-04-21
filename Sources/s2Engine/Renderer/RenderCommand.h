// RenderCommand.h
//
#ifndef S2_RENDERER_RENDERCOMMAND_H
#define S2_RENDERER_RENDERCOMMAND_H

#include "s2Engine_API.h"

#include "RenderMaterial.h"

#include "RenderCore/RenderState.h"
#include "RenderCore/ClearState.h"
#include "Geometry/MeshData.h"


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
	ClearMode clearMode  = ClearMode::ColorAndDepth;
	Color     clearColor = Color::blue();
	float     clearDepthValue = 1.0f;
	uint32_t  clearStencilValue = 0;
};


/**
 * Represents a single draw call with all necessary state.
 * Will be processed by the renderer and translated into DrawState for the render pass.
 */
struct S2ENGINE_API RenderCommand
{
	RenderMode renderMode { RenderMode::Triangles };
	Material   material;
	ResourceID mesh { ResourceInvalidID };

	uint32_t      pickableID { 0 }; // For GPU picking - 0 means non-pickable

	// per object transform (model matrix)
	Math::dmat4 modelMatrix = Math::dmat4( 1.0 );

	// User data for custom rendering logic
	void* userData = nullptr;
};


// Helper functions to translate high-level commands to low-level GPU states
RenderCore::ClearState  S2ENGINE_API getClearState ( const ClearCommand& clearCmd );
RenderCore::RenderState S2ENGINE_API getRenderState( const RenderCommand& renderCmd );


}
}

#endif // !S2_RENDERER_RENDERCOMMAND_H