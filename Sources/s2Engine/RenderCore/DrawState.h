// DrawState.h
// 
#ifndef S2_RENDERCORE_DRAWSTATE_H
#define S2_RENDERCORE_DRAWSTATE_H

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN //#undef DrawState // undefine DrawState macro from windows.h
#endif

#include "s2Engine_API.h"

#include "RenderState.h"
#include "TransformState.h"
#include "Shader.h"
#include "DefaultShaders.h"

namespace s2 {
namespace RenderCore {

struct DrawState
{
	// GPU state
	RenderState  renderState;

	// per draw call transform state (model, view, projection matrices)
	TransformState transform;
	
	// viewport and scissor state
	ViewportState  viewport;

	// shader program
	ShaderPtr      shader;

	DrawState( const ShaderPtr&s = DefaultShaders.Simple, const RenderState &renderState = {} )
	: shader( s )
	, renderState( renderState )
	{}
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_DRAWSTATE_H