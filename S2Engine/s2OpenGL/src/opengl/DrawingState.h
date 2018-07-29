// DrawingState.h
// 
#ifndef DRAWSTATE_ONCE
#define DRAWSTATE_ONCE

#include "s2OpenGL_API.h"

#include "RenderState.h"
#include "Program.h"

namespace s2 {
namespace OpenGL {

	struct DrawingState
	{
		RenderState  renderState;
		ProgramPtr   shaderProgram;

		// TODO: add texture units and bounding textures
		// Add FBO?
		DrawingState()
		{}

		DrawingState( const ProgramPtr &shaderProgram, const RenderState &renderState = RenderState()  )
		: renderState( renderState )
		, shaderProgram( shaderProgram )
		{}
	};
}
}
#endif