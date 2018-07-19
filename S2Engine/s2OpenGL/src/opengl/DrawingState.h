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
		RenderState        renderState;
		OpenGL::ProgramPtr program;

		DrawingState()
		{}

		DrawingState( const OpenGL::ProgramPtr &shaderProgram, const RenderState &renderState = RenderState()  /*,VertexArray vertexArray*/ )
		: renderState( renderState )
		, program( shaderProgram )
		{}

		//VertexArray VertexArray { get; set; }
	};

}
}
#endif