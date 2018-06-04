// DrawState.h
// 
#ifndef DRAWSTATE_ONCE
#define DRAWSTATE_ONCE

#include "Renderer_API.h"

#include "RenderState.h"
#include "ClearState.h"
#include "OpenGL/Program.h"


namespace OpenGL {

	struct DrawState
	{
		RenderState        renderState;
		OpenGL::ProgramPtr program;

		DrawState() {}

		DrawState( const OpenGL::ProgramPtr &shaderProgram, const RenderState &renderState = RenderState()  /*,VertexArray vertexArray*/ )
		: renderState( renderState )
		, program( shaderProgram )
		{}

		//VertexArray VertexArray { get; set; }
	};

}

#endif