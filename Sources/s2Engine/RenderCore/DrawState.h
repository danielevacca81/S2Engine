// DrawState.h
// 
#ifndef DRAWSTATE_H
#define DRAWSTATE_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN //#undef DrawState // undefine DrawState macro from windows.h
#endif

#include "s2Engine_API.h"

#include "RenderState.h"
#include "ViewState.h"
#include "Program.h"
#include "RenderCore.h"
#include "TextureUnit.h"


namespace RenderCore {

struct DrawState
{
	bool         shadowingEnabled { true }; // @todo:remove. hide shadowing to the user forcing to true

	RenderState  renderState;
	ViewState    viewState;
	ProgramPtr   shader;
	TextureUnits textureUnits;


	DrawState( const ProgramPtr &s = DefaultShaders.Simple, const RenderState &renderState = {} )
	: shadowingEnabled( true )
	, shader( s )
	, renderState( renderState )
	{}
};

}
#endif