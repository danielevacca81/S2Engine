// ClearState.h
// 
#ifndef CLEARSTATE_ONCE
#define CLEARSTATE_ONCE

#include "graphics/Color.h"
#include "RenderState.h"

namespace OpenGL {

struct ClearState
{
	ClearState()
	: scissorTest()
	, colorMask(true,true,true,true)
	, depthMask(true)
	, frontStencilMask(~0)
	, backStencilMask(~0)
	, buffers(All)
	, color( Color::black() )
	, depth( 1.f )
	, stencil( 0 )
	{}

	ScissorTest  scissorTest;
	ColorMask    colorMask;
	bool         depthMask;
	int          frontStencilMask;
	int          backStencilMask;
	ClearBuffers buffers;
	Color    color;
	float        depth;
	int          stencil;
};

}

#endif