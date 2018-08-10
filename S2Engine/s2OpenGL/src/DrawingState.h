// DrawingState.h
// 
#ifndef DRAWSTATE_ONCE
#define DRAWSTATE_ONCE

#include "s2OpenGL_API.h"

#include "RenderState.h"
#include "Program.h"
#include "TextureUnit.h"

namespace s2 {
namespace OpenGL {

struct DrawingState
{
	RenderState  renderState;
	ProgramPtr   shaderProgram;
	TextureUnits textureUnits;

	DrawingState()
	{}

	DrawingState( const ProgramPtr &shaderProgram, const RenderState &renderState = {} )
	: shaderProgram( shaderProgram )
	, renderState( renderState )
	{}
};
}
}
#endif