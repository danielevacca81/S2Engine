// DrawState.h
// 
#ifndef DRAWSTATE_ONCE
#define DRAWSTATE_ONCE

#include "RenderSystem_API.h"

#include "RenderState.h"
#include "ViewState.h"
#include "Program.h"
#include "Resources.h"
#include "TextureUnit.h"

namespace RenderSystem {

struct DrawState
{
	bool         shadowingEnabled { true }; // @todo:remove. hide shadowing to the user forcing to true

	RenderState  renderState;
	ViewState    viewState;
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