﻿// DrawingState.h
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

	DrawingState( const ProgramPtr &shader, const RenderState &renderState = {} )
	: shaderProgram( shader )
	, renderState( renderState )
	{}
};
}
}
#endif