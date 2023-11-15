// DrawState.h
// 
#ifndef DRAWSTATE_ONCE
#define DRAWSTATE_ONCE

#include "RenderSystem_API.h"

#include "RenderState.h"
#include "Program.h"
#include "Resources.h"
#include "TextureUnit.h"

namespace RenderSystem {

// struct ViewState
//{
//  viewport;
//  modelMatrix;
//  viewMatrix;
//  projectionMatrix;
//}

struct DrawState
{
	bool         shadowingEnabled { true };

	RenderState  renderState;
	// ViewState viewState; ??
	ProgramPtr   shaderProgram;
	TextureUnits textureUnits;


	DrawState( const ProgramPtr &shader = Resources::DefaultShaders.Simple, const RenderState &renderState = {} )
	: shadowingEnabled( true )
	, shaderProgram( shader )
	, renderState( renderState )
	{}
};

}
#endif