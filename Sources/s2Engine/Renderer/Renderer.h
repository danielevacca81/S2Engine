// Renderer.h
//
#ifndef S2_RENDERER_RENDERER_H
#define S2_RENDERER_RENDERER_H

#include "s2Engine_API.h"

#include "RenderCommand.h"
#include "MaterialBinder.h"

#include <memory>
#include <vector>

namespace s2 {
namespace Renderer {

/**
 * High-level mesh rendering abstraction.
 * Renders to a Surface.
 * Handles transformation, material binding, and submission to RenderCore.
 */
class S2ENGINE_API Renderer
{
public:
	Renderer();
	~Renderer() = default;

	///**
	// * Set the default shader program for rendering.
	// */
	//void setDefaultProgram( RenderCore::ProgramPtr program );

	/**
	 * Set the render target to draw into.
	 */
	void setSurface( /**/ );

	/**
	 * Submit a render command to be executed.
	 */
	void submit( const RenderCommand& command );

	/**
	 * Execute all submitted commands in optimal order.
	 * Renders to the active RenderTarget.
	 */
	void flush();

	/**
	 * Clear all pending commands.
	 */
	void clear();

	/**
	 * Get rendering statistics.
	 */
	struct Stats
	{
		size_t drawCalls = 0;
		size_t triangles = 0;
		size_t vertices = 0;
		size_t materialSwitches = 0;
	};

	Stats statistics() const { return _stats; }
	void resetStatistics();

//private:
//	void applyTransforms( const RenderCommand& cmd, RenderCore::DrawState& drawState );
//	void sortCommands();
//
//private:
//	RenderCore::ProgramPtr  _defaultProgram;
//	RenderCore::RenderTarget* _renderTarget = nullptr;
//
	MaterialBinder _materialBinder;
	std::vector<RenderCommand> _commands;

	Stats _stats;
};

}
}

#endif // S2_RENDERER_MESHRENDERER_H