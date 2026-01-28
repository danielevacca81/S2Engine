// DrawState.h
// 
#ifndef DRAWSTATE_ONCE
#define DRAWSTATE_ONCE

#include "Framework_Exports.h"

#include "RenderState.h"
#include "ClearState.h"
#include "OpenGL/Program.h"


namespace OpenGL {

struct DrawState
{
	RenderState renderState;
	Program     program;

	DrawState() {}

    DrawState( const RenderState &renderState, const Program &shaderProgram /*,VertexArray vertexArray*/ )
    : renderState( renderState )
	, program( shaderProgram )
    {}

    //VertexArray VertexArray { get; set; }
};

}

#endif